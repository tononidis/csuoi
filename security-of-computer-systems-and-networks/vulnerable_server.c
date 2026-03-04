// Stefanos Tononidis 2833
// Drivas Panagiotis 2690

#ifdef _WIN32
#error "This program is not supported on Windows."
#endif

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 7890
#define BUF_SIZE 1024
#define MAX_PENDING_CONNECTIONS 1

/*
 * @name vulnerableFunction - A function that demonstrates a buffer overflow vulnerability.
 *
 * @param input The input string that will be copied into a smaller buffer, potentially causing an overflow.
 * @param inputLen The length of the input string.
 */
static void vulnerableFunction(void* input, size_t inputLen)
{
    char smallBuff[BUF_SIZE / 2] = { 0 };
    memcpy(smallBuff, input, inputLen);
}

/*
 * @name main - The main routine.
 *
 * @return 0 on success, 1 on error.
 */
int main()
{
    int serverSocketFd = -1;
    if ((serverSocketFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Safe choice for testing, only allows local connections
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocketFd, MAX_PENDING_CONNECTIONS) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening for new connection on port %d ...\n", PORT);

    char buf[BUF_SIZE] = { 0 };
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocketFd = -1;
    while (1) {
        bzero(buf, sizeof(buf));

        clientSocketFd = accept(serverSocketFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocketFd == -1) {
            perror("accept failed");
            continue;
        }

        printf("Got connection from client [%s:%d]\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        while (1) {
            int bytesReceived = recv(clientSocketFd, buf, sizeof(buf), 0);

            if (bytesReceived == -1) {
                perror("recv failed");
                break;
            }

            else if (bytesReceived == 0) {
                printf("Client disconnected.\n");
                break;
            }

            else {
                printf("Received message with length [%d] bytes from client calling vulnerableFunction ...\n", bytesReceived);
                vulnerableFunction(buf, bytesReceived);
            }
        }
    }

    printf("Accept failed server will exit, cleaning up resources... \n");
    close(serverSocketFd);

    return EXIT_SUCCESS;
}