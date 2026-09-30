#include <stdio.h>

void printBinary(unsigned int number)
{
    unsigned int exponent = 128;

    while (exponent >= 1)
    {
        if (number >= exponent)
        {
            printf("1");
            number = number - exponent;
        }
        else
        {
            printf("0");
        }

        exponent = exponent / 2;
    }
}

int main(void)
{
    unsigned int number;

    printf("Enter a number from 0 to 255: ");
    scanf("%u", &number);

    printf("Binary: ");
    printBinary(number);
    printf("\n");

    return 0;
}