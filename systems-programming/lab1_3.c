#include <stdio.h>

/**
 * @brief This program checks if a 3x3 matrix is a magic square.
 *
 * A magic square is a square matrix in which the sum of every row, column, and diagonal is the same.
 *
 * @return int
 */
int main(void)
{
    int arr[3][3] = {
        { 2, 7, 6 },
        { 9, 5, 1 },
        { 4, 3, 8 }
    };

    int magicHorizontalSum = 0;
    int magicVerticalSum = 0;
    int magicDiagonalSum = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                magicDiagonalSum += arr[i][j];
            }

            magicHorizontalSum += arr[i][j];
            magicVerticalSum += arr[j][i];
        }

        if (magicHorizontalSum != magicVerticalSum) {
            printf("Not a magic square\n");
            return 0;
        }
    }

    if (magicHorizontalSum != magicDiagonalSum * 3 && magicVerticalSum != magicDiagonalSum * 3) {
        printf("Not a magic square\n");
    }

    else {
        printf("Is a magic square with magic sum of [%d]\n", magicDiagonalSum);
    }

    return 0;
}