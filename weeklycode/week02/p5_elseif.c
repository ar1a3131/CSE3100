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
    int w = fact(5);
    /* adding 2 ifs and 2 else blocks */
    if(w==0)
        printf("Base!\n");
    else if(w==1)
        printf("Case 1\n");
    else printf("Other\n");

    printf("answer: %d\n", w);

    return 0;
}
