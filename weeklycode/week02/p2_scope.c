/* Fahrenheit to Celsius conversion - for loop */
/* Using macros to make the code more readable */
/* Understanding scope of variables */

#include <stdio.h>

#define INIT 0
#define END 300
#define STEP 10

int main()
{
    int celsius, fahr;  /* what is the scope of these variables? */

    for(fahr = INIT; fahr <= END; fahr += STEP) {
        celsius = 5 * (fahr - 32) / 9;
        printf("From %d F to %d C degrees \n", fahr, celsius);
    }

    return 0;
}
