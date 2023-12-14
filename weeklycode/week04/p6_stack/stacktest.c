#include "stack.h"
#include <stdio.h>

int main() {
    StackOfInt* s = makeStack(4);
    for(int i=0; i<32;i++)
        pushInt(s, i);
    while(!isStackEmpty(s)) {
        int v = popInt(s);
        printf("Popped %d\n", v);
    }
    freeStack(s);
    return 0;
}