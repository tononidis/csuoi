#ifdef _WIN32
#error "This program is not supported on Windows."
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief This program reads integers from the user, calculates their sum and average,
 * it communicates this information between a parent and child process using two pipes.
 * The first pipe is used for sending integers from the parent to the child,
 * the second pipe is used for sending the sum and average from the child back to the parent.
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

    int pfd1[2];
    if (pipe(pfd1) < 0) {
        perror("pipe error\n");
        exit(EXIT_FAILURE);
    }

    int pfd2[2];
    if (pipe(pfd2) < 0) {
        perror("pipe error\n");
        exit(EXIT_FAILURE);
    }

    if (fork() != 0) {
        close(pfd1[0]);
        close(pfd2[1]);

        printf("I am parent with id %d \n", getpid());

        printf("Input integers, to end enter 0\n");

        int input = -1;
        while (1) {
            scanf("%d", &input);

            if (write(pfd1[1], &input, sizeof(input)) < 0) {
                perror("Error writing to child");
                exit(EXIT_FAILURE);
            }

            if (input == 0) {
                break;
            }
        }

        close(pfd1[1]);

        int sum = 0;
        if (read(pfd2[0], &sum, sizeof(sum)) < 0) {
            perror("Error reading sum from child");
            exit(EXIT_FAILURE);
        }

        double avg;
        if (read(pfd2[0], &avg, sizeof(avg)) < 0) {
            perror("Error reading average from child");
            exit(EXIT_FAILURE);
        }

        close(pfd2[0]);

        printf("Got from child sum [%d] and average [%f]\n", sum, avg);
        waitpid(-1, NULL, 0);
    }

    else {
        close(pfd1[1]);
        close(pfd2[0]);

        printf("I am child with id %d \n", getpid());

        int readData = -1, sum = 0, readDataCounter = 0;
        while (1) {
            if (read(pfd1[0], &readData, sizeof(readData)) < 0) {
                perror("Error reading from parent");
                exit(EXIT_FAILURE);
            }

            if (readData == 0) {
                break;
            }

            sum += readData;
            readDataCounter++;
        }

        close(pfd1[0]);

        double avg = (double)sum / readDataCounter;

        if (write(pfd2[1], &sum, sizeof(sum)) < 0) {
            perror("Error writing sum to parent");
            exit(EXIT_FAILURE);
        }

        if (write(pfd2[1], &avg, sizeof(avg)) < 0) {
            perror("Error writing average to parent");
            exit(EXIT_FAILURE);
        }

        close(pfd2[1]);
    }
}