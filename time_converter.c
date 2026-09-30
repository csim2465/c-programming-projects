#include <stdio.h>

int convertToSeconds(int hours, int minutes, int seconds)
{
    return (hours * 3600) + (minutes * 60) + seconds;
}

int main(void)
{
    int hours;
    int minutes;
    int seconds;
    int totalSeconds;

    printf("Enter hours: ");
    scanf("%d", &hours);

    printf("Enter minutes: ");
    scanf("%d", &minutes);

    printf("Enter seconds: ");
    scanf("%d", &seconds);

    totalSeconds = convertToSeconds(hours, minutes, seconds);

    printf("Total seconds: %d\n", totalSeconds);

    return 0;
}
