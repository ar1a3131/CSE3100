/* Fahrenheit to Celsius conversion - for loop */

#include <stdio.h>

int main()
{
    int celsius, fahr;
    int lower, upper;
    int step;

    lower = 0;
    upper = 300;
    step = 10;

    for(fahr = lower; fahr <= upper; fahr += step) {
        celsius = 5 * (fahr - 32) / 9;
        printf("From %d F to %d C degrees \n", fahr, celsius);
    }
    return 0;
}
