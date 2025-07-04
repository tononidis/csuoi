#include "common.h"

#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 6767
#define BUF_SIZE 1024
#define MAX_STATION_ID 128

#define NUMBER_OF_PUT_CLIENTS 50
#define NUMBER_OF_GET_CLIENTS 50

enum ModeType {
    AUTO_PUT_REQUEST,
    AUTO_GET_REQUEST,
    MANY_CLIENTS_REQUESTS,
    MANUAL_USER_REQUEST,
    __UNKNOWN_REQUEST
};

static pthread_t putClientThreads[NUMBER_OF_PUT_CLIENTS];
static pthread_t getClientThreads[NUMBER_OF_GET_CLIENTS];

static void printHelp()
{
    printf("Usage: client [OPTIONS]\n");
    printf("Available Options:\n");
    printf("-h:             Print this help message.\n");
    printf("-a <address>:   Specify the server address or hostname.\n");
    printf("-o <operation>: Send a single operation to the server.\n");
    printf("\t\t<operation>: PUT:<key>:<value> or GET:<key>\n");
    printf("-i <count>:     Specify the number of iterations.\n");
    printf("-g:             Send GET operations for all available stations (keys).\n");
    printf("-p:             Send PUT operations for all available stations (keys).\n");
    printf("-m:             Repeatedly send PUT and GET operations from multiple connections\n");
}

static int waitForResponse(int socket, struct Request* request_out, uint size)
{
    if (request_out == NULL) {
        fprintf(stderr, "Request is NULL\n");
        return -1;
    }

    int readDataSize = read(socket, request_out, size);
    if (readDataSize == -1) {
        perror("read failed");
        return -1;
    }

    debug_print("Received response type [%s], key [%s], value [%s]\n", operationTypeStr[request_out->operation], request_out->key, request_out->value);
    // if(request_out->operation == PUT_OK) {
    // }

    return 0;
}

static int sendRequest(int socket, struct Request* request_in, uint size)
{
    if (request_in == NULL) {
        fprintf(stderr, "Request is NULL\n");
        return -1;
    }

    debug_print("Send request type [%s], key [%s], value [%s]\n", operationTypeStr[request_in->operation], request_in->key, request_in->value);

    if (write(socket, request_in, size) == -1) {
        perror("write failed");
        return -1;
    }

    return 0;
}

static int doConnect(struct hostent* host_info)
{
    if (host_info == NULL) {
        fprintf(stderr, "Error: Host information is NULL.\n");
        return -1;
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr = *((struct in_addr*)host_info->h_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    int socket_fd = -1;
    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        return -1;
    }

    if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect failed");
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

static void* manyPuts(void* ctx)
{
    struct hostent* host_info = (struct hostent*)ctx;

    int socket_fd = doConnect(host_info);
    if (socket_fd < 0) {
        fprintf(stderr, "Thread failed to connect to server.\n");
        return NULL;
    }

    struct Request request;
    request.operation = PUT;

    int stationId = rand() % MAX_STATION_ID;
    int value = rand() % 65 + (-20);

    snprintf(request.key, KEY_SIZE, "station.%d", stationId);
    snprintf(request.value, VALUE_SIZE, "%d", value);

    sendRequest(socket_fd, &request, sizeof(request));

    memset(&request, 0, sizeof(request));
    waitForResponse(socket_fd, &request, sizeof(request));

    memset(&request, 0, sizeof(request));
    request.operation = END;
    sendRequest(socket_fd, &request, sizeof(request));

    close(socket_fd);

    return NULL;
}

static void* manyGets(void* ctx)
{
    struct hostent* host_info = (struct hostent*)ctx;

    int socket_fd = doConnect(host_info);
    if (socket_fd < 0) {
        fprintf(stderr, "Thread failed to connect to server.\n");
        return NULL;
    }

    struct Request request;
    request.operation = GET;

    int stationId = rand() % MAX_STATION_ID;
    snprintf(request.key, KEY_SIZE, "station.%d", stationId);
    sendRequest(socket_fd, &request, sizeof(request));

    memset(&request, 0, sizeof(request));
    waitForResponse(socket_fd, &request, sizeof(request));

    memset(&request, 0, sizeof(request));
    request.operation = END;
    sendRequest(socket_fd, &request, sizeof(request));

    close(socket_fd);

    return NULL;
}

int main(int argc, char** argv)
{
    char* host = NULL;
    char* manualRequest = NULL;
    uint operation = __UNKNOWN_REQUEST;
    uint numOfIterations = 1;

    int option = 0;
    while ((option = getopt(argc, argv, "i:hgpmo:a:")) != -1) {
        switch (option) {
        case 'h': {
            printHelp();
            return EXIT_SUCCESS;
        }

        case 'a': {
            host = optarg;
            break;
        }

        case 'i': {
            numOfIterations = atoi(optarg);
            break;
        }

        case 'g': {
            if (operation == __UNKNOWN_REQUEST) {
                operation = AUTO_GET_REQUEST;
            }

            else {
                fprintf(stderr, "You can only specify one of the following: -g, -p, -m, -o\n");
                return EXIT_FAILURE;
            }

            break;
        }

        case 'p': {
            if (operation == __UNKNOWN_REQUEST) {
                operation = AUTO_PUT_REQUEST;
            }

            else {
                fprintf(stderr, "You can only specify one of the following: -g, -p, -m, -o\n");
                return EXIT_FAILURE;
            }

            break;
        }

        case 'm': {
            if (operation == __UNKNOWN_REQUEST) {
                operation = MANY_CLIENTS_REQUESTS;
            }

            else {
                fprintf(stderr, "You can only specify one of the following: -g, -p, -m, -o\n");
                return EXIT_FAILURE;
            }

            break;
        }

        case 'o': {
            if (operation == __UNKNOWN_REQUEST) {
                operation = MANUAL_USER_REQUEST;
            }

            manualRequest = optarg;

            break;
        }

        default: {
            printHelp();
            return EXIT_FAILURE;
        }
        }
    }

    printf("Operation = %d\nHost = %s\n", operation, host);

    if (operation == __UNKNOWN_REQUEST) {
        fprintf(stderr, "Error: One of -g, -p, -m, -o is required.\n\n");
        printHelp();
        return EXIT_FAILURE;
    }

    if (host == 0) {
        fprintf(stderr, "Error: -a <address> is required.\n\n");
        printHelp();
        return EXIT_FAILURE;
    }

    struct hostent* host_info = gethostbyname(host);
    if (host_info == NULL) {
        perror("gethostbyname failed");
        return EXIT_FAILURE;
    }

    if (operation == MANY_CLIENTS_REQUESTS) {
        for (uint i = 0; i < NUMBER_OF_PUT_CLIENTS; i++) {
            if (pthread_create(&putClientThreads[i], NULL, manyPuts, host_info) != 0) {
                perror("Failed to create put client thread");
                return EXIT_FAILURE;
            }
        }

        for (uint i = 0; i < NUMBER_OF_GET_CLIENTS; i++) {
            if (pthread_create(&getClientThreads[i], NULL, manyGets, host_info) != 0) {
                perror("Failed to create get client thread");
                return EXIT_FAILURE;
            }
        }

        for (uint i = 0; i < NUMBER_OF_PUT_CLIENTS; i++) {
            pthread_join(putClientThreads[i], NULL);
        }

        for (uint i = 0; i < NUMBER_OF_GET_CLIENTS; i++) {
            pthread_join(getClientThreads[i], NULL);
        }

        return EXIT_SUCCESS;
    }

    int socket_fd = doConnect(host_info);
    if (socket_fd == -1) {
        fprintf(stderr, "Failed to connect to server.\n");
        return EXIT_FAILURE;
    }

    struct Request request;
    memset(&request, 0, sizeof(request));

    if (operation == MANUAL_USER_REQUEST) {
        char* operation = strtok(manualRequest, ":");
        char* key = strtok(NULL, ":");
        char* value = strtok(NULL, ":");

        if (operation == NULL || key == NULL) {
            fprintf(stderr, "Invalid request format.\n");
            return EXIT_FAILURE;
        }

        if (strncmp(operation, "GET", strlen("GET")) == 0) {
            request.operation = GET;
        }

        else if (strncmp(operation, "PUT", strlen("PUT")) == 0) {
            request.operation = PUT;
            snprintf(request.value, VALUE_SIZE, "%s", value);
        }

        else {
            fprintf(stderr, "Invalid operation format.\n");
            return EXIT_FAILURE;
        }

        snprintf(request.key, KEY_SIZE, "%s", key);

        sendRequest(socket_fd, &request, sizeof(request));

        memset(&request, 0, sizeof(request));
        waitForResponse(socket_fd, &request, sizeof(request));
    }

    else {
        while (numOfIterations != 0) {
            for (int station = 0; station <= MAX_STATION_ID; station++) {

                memset(&request, 0, sizeof(request));
                if (operation == AUTO_GET_REQUEST) {
                    request.operation = GET;
                    snprintf(request.key, KEY_SIZE, "station.%d", station);
                }

                else if (operation == AUTO_PUT_REQUEST) {
                    int value = rand() % 65 + (-20);
                    request.operation = PUT;
                    snprintf(request.key, KEY_SIZE, "station.%d", station);
                    snprintf(request.value, VALUE_SIZE, "%d", value);
                }

                else {
                    fprintf(stderr, "Unknown operation type: %d\n", operation);
                    close(socket_fd);
                    return EXIT_FAILURE;
                }

                sendRequest(socket_fd, &request, sizeof(request));

                memset(&request, 0, sizeof(request));
                waitForResponse(socket_fd, &request, sizeof(request));
            }

            numOfIterations--;
        }
    }

    memset(&request, 0, sizeof(request));
    request.operation = END;
    sendRequest(socket_fd, &request, sizeof(request));

    close(socket_fd);
    return EXIT_SUCCESS;
}