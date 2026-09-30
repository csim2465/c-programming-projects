#include <stdio.h>

int main(void)
{
    int myBigArray[50] = {
        5, 8, 3, 5, 12, 7, 5, 20, 9, 3,
        15, 5, 6, 11, 8, 5, 14, 2, 19, 5,
        7, 13, 5, 10, 4, 18, 5, 1, 16, 8,
        5, 12, 3, 17, 5, 9, 6, 5, 20, 11,
        4, 5, 14, 7, 2, 5, 18, 10, 5, 13
    };

    int searchValue;
    int count = 0;

    printf("Enter a value to search for: ");
    scanf("%d", &searchValue);

    for (int i = 0; i < 50; i++)
    {
        if (myBigArray[i] == searchValue)
        {
            count++;
        }
    }

    printf("%d appears %d time(s) in the array.\n",
           searchValue, count);

    return 0;
}
