/* Fahrenheit to Celsius conversion - while loop */

#include <stdio.h>

#define INIT 0
#define END 300
#define STEP 10

int main()
{
    int fahr = INIT;  

    while(fahr <= END) {
        int celsius = 5 * (fahr - 32) / 9;
        printf("From %d F to %d C degrees \n", fahr, celsius);
        fahr += STEP;
    }

    return 0;
}
