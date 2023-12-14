/* Converting a string of numbers into its decimal representation: using ASCII values */

#include <stdio.h>

int main()
{
    char s[7] = "123456";    /* a string is '/0' == 0 terminated */
    int i = 0;              /* counter to read each character in the string */
    int n = 0;              /* variable to hold the decimal value */
    while(s[i] != 0) {  
        n = n*10;
        n = n + s[i] - '0';
        i++;
    }

    printf("Converted: %d\n", n);

    return 0;
}
