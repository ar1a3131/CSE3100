/* Take a user input n 
 * Compute sum of all numbers from 1 to n
 */

#include <stdio.h>

int main(void) {
    int num = 0;
    printf("Please enter a number: \n");
    scanf("%d", &num);
    printf("The number read is %d\n", num);

    int sum = 0;
    int i = 1;

    while(i <= num) {
        sum = sum + i;
        i = i + 1;
    }

    printf("The sum of 1 to %d is %d\n", num, sum);

    /* TODO: Calculate and print the sum from 1 to n using a for loop */
    
    return 0;

}