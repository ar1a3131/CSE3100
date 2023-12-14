/* Converting a string of numbers into its decimal representation: Switch case */
/* Beware: the blocks in switch case are chained */
/* Break gets you out of the innermost control flow primitive - switch or loop */

#include <stdio.h>

int main()
{
    char s[7] = "123456";    /* a string is '/0' == 0 terminated */
    int i = 0;               /* counter to read each character in the string */
    int n = 0;               /* variable to hold the decimal value */
    while(s[i] != 0) {  
        n = n*10;
        switch(s[i]) {
            case '0': break;
            case '1': n+=1; break;
            case '2': n+=2; break;
            case '3': n+=3; break;
            case '4': n+=4; break;
            case '5': n+=5; break;
            case '6': n+=6; break;
            case '7': n+=7; break;
            case '8': n+=8; break;
            case '9': n+=9; break;
        }
        i++;
    }
    printf("Converted: %d\n", n);

    return 0;
}
