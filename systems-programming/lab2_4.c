#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 128 + 1

/**
 * @brief This program reverses a string.
 *
 * @return int
 */
int main(void)
{
    char input[MAX_LENGTH];
    char result[MAX_LENGTH];

    memset(input, 0, MAX_LENGTH);
    memset(result, 0, MAX_LENGTH);

    printf("Input string to reverse\n");
    fgets(input, MAX_LENGTH, stdin);

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    for (int i = 0; i < len; i++) {
        result[len - i - 1] = input[i];
    }

    printf("Your reversed string is [%s]\n", result);

    return 0;
}