/* Side effects of using static and global variables in functions */

#include <stdio.h>

int silly(int x);

int main()
{
    for(int i=0; i<4; i++) {
        printf("silly on %d is %d\n", i, silly(i));
    }
    for(int i=0; i<4; i++) {
        printf("silly on %d is %d\n", i, silly(i));
    }
    return 0;
}

int silly(int x) {
    static int hidden = 0;
    return x * 2 + hidden++;
}