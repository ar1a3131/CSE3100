/* Functions in C - tool to build procedural abstractions */
/* A function must ONLY depend on its arguments and local variables - no statics or globals should be used */
/* Declare the function prototype*/ 
/* Define the function implementation */

#include <stdio.h>

#define INIT 0
#define END 300
#define STEP 10

int fahrToCelcius(int degF);

int main()
{
    for(int fahr = INIT; fahr <= END; fahr += STEP) {
        int celsius = fahrToCelcius(fahr);
        printf("From %d F to %d C degrees \n", fahr, celsius);
    }
    return 0;
}

int fahrToCelcius(int degF) {
    return 5 * (degF - 32) / 9;
}
