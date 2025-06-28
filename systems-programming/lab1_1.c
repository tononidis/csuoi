#include <stdio.h>

/**
 * @brief This program calculates the final score of a student based on their test scores and attendance.
 *
 * @return int
 */
int main(void)
{
    int missedDays;

    float attendanceRes, firstExamRes, secondExamRes, finalExamRes;

    printf("Input how many days you missed\n");
    scanf("%d", &missedDays);
    if (missedDays == 0) {
        attendanceRes = 10;
    }

    else if (missedDays == 1) {
        attendanceRes = 5;
    }

    else {
        attendanceRes = 0;
    }

    printf("Input first exams score\n");
    scanf("%f", &firstExamRes);

    if (firstExamRes < 0 || firstExamRes > 10) {
        printf("Invalid first exam score\n");
        return 1;
    }

    printf("Input second exams score\n");
    scanf("%f", &secondExamRes);

    if (secondExamRes < 0 || secondExamRes > 10) {
        printf("Invalid second exam score\n");
        return 1;
    }

    printf("Input final exams score\n");
    scanf("%f", &finalExamRes);

    if (finalExamRes < 0 || finalExamRes > 10) {
        printf("Invalid final exam score\n");
        return 1;
    }

    float midTermRes = (attendanceRes * 0.2) + (firstExamRes * 0.4) + (secondExamRes * 0.4);

    if (midTermRes < 5) {
        printf("You failed to pass the class mid term exams, score is [%f]\n", midTermRes);
    }

    else {
        float result = (midTermRes * 0.4) + (finalExamRes * 0.6);
        if (result < 5) {
            printf("You failed to pass the class, score is [%f]\n", result);
        }

        else {
            printf("You passed the class, score is [%f]\n", result);
        }
    }

    return 0;
}