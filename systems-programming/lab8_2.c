#ifdef _WIN32
#error "This program is not supported on Windows."
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief This program reads integers from the user and writes them to a file.
 * The child process reads input from the user and writes it to a pipe,
 * while the parent process reads from the pipe and writes only the positive integers to a file.
 * 
 * @note The program ends when the user enters 0.
 *
 * @warning This program is not supported on Windows.
 * 
 * @return int
 */
int main()
{
#ifdef _WIN32
    printf("This program is not supported on Windows.\n");
    return 1;
#endif

    int pfd[2];
    if (pipe(pfd) < 0) {
        perror("pipe error\n");
        exit(1);
    }

    if (fork() == 0) {
        close(pfd[0]);

        printf("Input integer numbers, to end enter 0\n");
        int input = -1;
        while (1) {
            scanf("%d", &input);
            if (write(pfd[1], &input, sizeof(input)) < 0) {
                perror("Error writing to pipe");
                exit(EXIT_FAILURE);
            }

            if (input == 0) {
                break;
            }
        }

        close(pfd[1]);
    }

    else {
        close(pfd[1]);

        FILE* fp = fopen("positives.txt", "w");
        if (fp == NULL) {
            perror("File open error\n");
            exit(1);
        }

        int readNumber = -1;
        while (read(pfd[0], &readNumber, sizeof(int)) > 0) {
            if (readNumber > 0) {
                fprintf(fp, "%d\n", readNumber);
            }

            else if (readNumber == 0) {
                break;
            }
        }

        waitpid(-1, NULL, 0);

        close(pfd[0]);
        fclose(fp);
    }

    return 0;
}