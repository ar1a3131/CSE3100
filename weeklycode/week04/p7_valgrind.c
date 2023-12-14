/* Using valgrind - to catch memory leaks */

#include <stdio.h>
#include <stdlib.h>

int main() {
    int* p = (int*)malloc(sizeof(int)*8);
    for(int i=0; i<=8; i++)
        p[i] = 0;
    
    return 0;
}
