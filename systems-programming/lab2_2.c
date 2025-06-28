#include <stdio.h>

/**
 * @brief This program prints the bytes of an integer.
 *
 * @return int
 */
int main(void)
{
    if (sizeof(int) != 4 || sizeof(unsigned char) != 1) {
        printf("This program is only valid for 4 byte integers and 1 byte unsigned characters\n");
        return 1;
    }

    int number = 260;

    char* pNumber = (char*)&number;

    unsigned char firstByte = *pNumber;
    unsigned char secondByte = *(pNumber + 1);
    unsigned char thirdByte = *(pNumber + 2);
    unsigned char lastByte = *(pNumber + 3);

    printf("First byte [%u]\nSecond byte [%u]\nThird byte [%u]\nLast byte [%u]\n", firstByte, secondByte, thirdByte, lastByte);

    return 0;
}