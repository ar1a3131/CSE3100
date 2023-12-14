/* Factorial function: If-then-else */

#include <stdio.h>

int fact(int n) {
    if(n==0)        /* notice you may skip {} if the block consists of a single statement*/
        return 1;
    else 
        return n * fact(n-1);
}

int main()
{
    int w = fact(5); /* call by value */
    printf("answer: %d\n", w);

    return 0;
}
