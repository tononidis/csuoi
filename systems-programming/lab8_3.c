#ifdef _WIN32
#error "This program is not supported on Windows."
#endif

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * @brief Signal handler for SIGPIPE signal.
 * This function is called when the gnuplot process terminates unexpectedly.
 * 
 *
 * @param signum The signal number.
 */
static void signalHandler(int signum)
{
    if (signum == SIGPIPE) {
        printf("Received SIGPIPE signal, gnuplot process may have terminated.\n");
    }

    else {
        printf("Received unexpected signal %d\n", signum);
    }

    exit(EXIT_SUCCESS);
}

/**
 * @brief This program sends commands to gnuplot using a pipe.
 * The child process executes gnuplot, while the parent process sends commands to it.
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
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        close(pfd[1]);
        printf("I am the child (%d) - about to exec gnuplot\n", getpid());

        if (dup2(pfd[0], STDIN_FILENO) < 0) {
            perror("dup2 failed");
            exit(EXIT_FAILURE);
        }

        if (execl("/usr/bin/gnuplot", "gnuplot", NULL) < 0) {
            perror("execl failed to execute gnuplot");
            exit(EXIT_FAILURE);
        }

        close(pfd[0]);
    }

    else {
        signal(SIGPIPE, signalHandler);

        close(pfd[0]);
        printf("I am the parent (%d) - sending commands to gnuplot\n", getpid());

        char cmd[50];

        sleep(1); // Give the child process time to set up
        printf("Type \"exit\" to finish.\n");
        while (1) {
            printf("Enter command for gnuplot: ");
            fgets(cmd, sizeof(cmd), stdin);
            if (write(pfd[1], cmd, strnlen(cmd, sizeof(cmd))) < 0) {
                perror("write to pipe failed");
                close(pfd[1]);
                exit(EXIT_FAILURE);
            }

            if (strncmp(cmd, "exit\n", sizeof(cmd)) == 0) {
                break;
            }

            else if (strncmp(cmd, "exit;\n", sizeof(cmd)) == 0) {
                break;
            }
        }

        close(pfd[1]);
        waitpid(-1, NULL, 0);
    }

    return 0;
}