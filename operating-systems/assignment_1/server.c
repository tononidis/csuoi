// Stefanos Tononidis 2833
// Drivas Panagiotis 2690

#include "common.h"
#include "kissdb/kissdb.h"

#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MY_PORT 6767
#define BUF_SIZE 1024 + 10
#define HASH_SIZE 1024
#define MAX_PENDING_CONNECTIONS 10

#define NUMBER_OF_CONSUMER_THREADS 6

struct ClientInfo {
    int fd;
    struct timeval tv;
};

struct Queue {
    struct ClientInfo clients[MAX_PENDING_CONNECTIONS];
    int head;
    int tail;
    int currentSize;
};

// Globals
static struct Queue s_g_queue;
static int serverSocketFd = -1;

static long int total_waiting_time = 0;
static long int total_service_time = 0;
static int completed_requests = 0;

static pthread_t producer_thread;
static pthread_t consumer_threads[NUMBER_OF_CONSUMER_THREADS];

static pthread_cond_t non_empty_queue = PTHREAD_COND_INITIALIZER;
static pthread_cond_t non_full_queue = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t statistics_access_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t db_access_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t queue_access_mutex = PTHREAD_MUTEX_INITIALIZER;

static KISSDB* db = NULL;

static void doCleanUp(void)
{
    printf("Cleaning up resources...\n");
    pthread_kill(producer_thread, SIGTERM);

    for (uint i = 0; i < NUMBER_OF_CONSUMER_THREADS; i++) {
        pthread_kill(consumer_threads[i], SIGTERM);
    }

    KISSDB_close(db);
    if (db != NULL) {
        free(db);
        db = NULL;
    }

    pthread_mutex_destroy(&queue_access_mutex);
    pthread_mutex_destroy(&db_access_mutex);
    pthread_mutex_destroy(&statistics_access_mutex);

    pthread_cond_destroy(&non_empty_queue);
    pthread_cond_destroy(&non_full_queue);

    close(serverSocketFd);
}

static void signalhandler(int sig)
{
    pthread_mutex_lock(&statistics_access_mutex);
    if (completed_requests == 0) {
        printf("No requests were handled.\n");
    }

    else {
        printf("Total requests handled = %d\nAverage waiting time = %ld microseconds\nAverage service time = %ld microseconds\n", completed_requests, total_waiting_time / completed_requests, total_service_time / completed_requests);
    }

    pthread_mutex_unlock(&statistics_access_mutex);

    doCleanUp();

    exit(EXIT_SUCCESS);
}

static void closedPipeSignalHandler(int sig)
{
    printf("A client closed the connection unexpectedly.\n");
}

static int processRequests(const int clientSocketFd)
{
    while (1) {
        struct Request request;
        memset(&request, 0, sizeof(request));
        int readDataSize = read(clientSocketFd, &request, sizeof(request));
        if (readDataSize == -1) {
            perror("read failed");
            return -1;
        }

        debug_print("Received request type [%s], key [%s], value [%s]\n", operationTypeStr[request.operation], request.key, request.value);

        if (request.operation == END) {
            return 0;
        }

        else if (request.operation == GET) {
            pthread_mutex_lock(&db_access_mutex);
            int ret = KISSDB_get(db, request.key, request.value);
            pthread_mutex_unlock(&db_access_mutex);

            if (ret == 0) {
                request.operation = GET_OK;
            }

            else {
                memset(&request, 0, sizeof(request));
                request.operation = GET_ERROR;
            }
        }

        else if (request.operation == PUT) {
            pthread_mutex_lock(&db_access_mutex);
            int ret = KISSDB_put(db, request.key, request.value);
            pthread_mutex_unlock(&db_access_mutex);

            if (ret == 0) {
                request.operation = PUT_OK;
            }

            else {
                memset(&request, 0, sizeof(request));
                request.operation = PUT_ERROR;
            }
        }

        else {
            printf("Unknown/Unhandled operation [%s]\n", operationTypeStr[request.operation]);
            memset(&request, 0, sizeof(request));
            request.operation = __UNKNOWN_OPERATION;
        }

        if (write(clientSocketFd, &request, sizeof(request)) == -1) {
            perror("write failed");
            return -1;
        }

        pthread_mutex_lock(&statistics_access_mutex);
        completed_requests++;
        pthread_mutex_unlock(&statistics_access_mutex);
    }

    return 0;
}

static int queueSize(struct Queue* queue)
{
    return queue->currentSize;
}

static int queueInsert(struct Queue* queue, int value)
{
    if (queue->currentSize == MAX_PENDING_CONNECTIONS) {
        debug_print("Queue is full at the moment\n");
        return -1;
    }

    gettimeofday(&queue->clients[queue->tail].tv, NULL);
    queue->clients[queue->tail].fd = value;

    queue->tail = (queue->tail + 1) % MAX_PENDING_CONNECTIONS;

    queue->currentSize++;
    return 0;
}

static int queuePop(struct Queue* queue, struct ClientInfo* clientInfo_out)
{
    if (queue->currentSize == 0) {
        debug_print("Queue is empty at the moment\n");
        return -1;
    }

    memcpy(clientInfo_out, &queue->clients[queue->head], sizeof(struct ClientInfo));
    queue->head = (queue->head + 1) % MAX_PENDING_CONNECTIONS;

    queue->currentSize--;
    return 1;
}

static void* producerThreadHandler(void* ctx)
{
    int clientSocketFd = -1;
    struct sockaddr_in client_addr;
    socklen_t clen = sizeof(client_addr);
    while (1) {
        clientSocketFd = accept(serverSocketFd, (struct sockaddr*)&client_addr, &clen);
        if (clientSocketFd == -1) {
            perror("accept failed");
            continue;
        }

        pthread_mutex_lock(&queue_access_mutex);

        if (queueInsert(&s_g_queue, clientSocketFd) == -1) {
            pthread_cond_wait(&non_full_queue, &queue_access_mutex);

            if (queueInsert(&s_g_queue, clientSocketFd) == -1) {
                fprintf(stderr, "Queue is full after receiving signal non_full_queue");
                exit(EXIT_FAILURE);
            }

            pthread_mutex_unlock(&queue_access_mutex);
            continue;
        }

        if (queueSize(&s_g_queue) == 1) {
            pthread_cond_broadcast(&non_empty_queue);
        }

        pthread_mutex_unlock(&queue_access_mutex);
    }

    return NULL;
}

static void* consumerThreadHandler(void* ctx)
{
    // pthread_mutex_lock(&queue_access_mutex);
    // pthread_cond_wait(&non_empty_queue, &queue_access_mutex);
    // pthread_mutex_unlock(&queue_access_mutex);

    struct ClientInfo clientInfo;
    struct timeval currentTime;
    while (1) {
        pthread_mutex_lock(&queue_access_mutex);
        if (queuePop(&s_g_queue, &clientInfo) == -1) {
            pthread_cond_wait(&non_empty_queue, &queue_access_mutex);
            pthread_mutex_unlock(&queue_access_mutex);
            continue;
        }

        if (queueSize(&s_g_queue) == MAX_PENDING_CONNECTIONS - 1) {
            pthread_cond_broadcast(&non_full_queue);
        }

        pthread_mutex_unlock(&queue_access_mutex);

        gettimeofday(&currentTime, NULL);

        pthread_mutex_lock(&statistics_access_mutex);
        total_waiting_time += (currentTime.tv_sec - clientInfo.tv.tv_sec) * 1000000 + (currentTime.tv_usec - clientInfo.tv.tv_usec);
        pthread_mutex_unlock(&statistics_access_mutex);

        gettimeofday(&currentTime, NULL);
        clientInfo.tv = currentTime;

        if (processRequests(clientInfo.fd) == -1) {
            printf("Error processing request\n");
        }

        gettimeofday(&currentTime, NULL);

        pthread_mutex_lock(&statistics_access_mutex);
        total_service_time += (currentTime.tv_sec - clientInfo.tv.tv_sec) * 1000000 + (currentTime.tv_usec - clientInfo.tv.tv_usec);
        pthread_mutex_unlock(&statistics_access_mutex);

        close(clientInfo.fd);
    }

    return NULL;
}

/*
 * @name main - The main routine.
 *
 * @return 0 on success, 1 on error.
 */
int main()
{
    if ((serverSocketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    signal(SIGPIPE, closedPipeSignalHandler);
    signal(SIGTSTP, signalhandler);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(MY_PORT);

    if (bind(serverSocketFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocketFd, MAX_PENDING_CONNECTIONS) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("(Info) main: Listening for new connections on port %d ...\n", MY_PORT);

    if ((db = (KISSDB*)malloc(sizeof(KISSDB))) == NULL) {
        perror("malloc failed for database creation");
        exit(1);
    }

    if (KISSDB_open(db, "mydb.db", KISSDB_OPEN_MODE_RWCREAT, HASH_SIZE, KEY_SIZE, VALUE_SIZE)) {
        perror("KISSDB_open failed");
        exit(1);
    }

    pthread_create(&producer_thread, NULL, producerThreadHandler, NULL);

    for (uint i = 0; i < NUMBER_OF_CONSUMER_THREADS; i++) {
        pthread_create(&consumer_threads[i], NULL, consumerThreadHandler, NULL);
    }

    pthread_join(producer_thread, NULL);

    for (uint i = 0; i < NUMBER_OF_CONSUMER_THREADS; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    doCleanUp();

    return EXIT_SUCCESS;
}