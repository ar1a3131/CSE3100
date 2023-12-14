#include "stack.h"
#include <stdlib.h>

StackOfInt* makeStack(int mxs) {
    StackOfInt* s = (StackOfInt*)malloc(sizeof(StackOfInt));
    s->_mxs = mxs;
    s->_sz = 0;
    s->_t = (int*)malloc(sizeof(int)*mxs);
    return s;
}

void freeStack(StackOfInt* s) {
    free(s->_t);
    free(s);
}

void pushInt(StackOfInt* s, int v) {
    if(s->_sz == s->_mxs) {
        s->_mxs = s->_mxs * 2;
        s->_t = realloc(s->_t, sizeof(int)*s->_mxs);
    }
    s->_t[s->_sz++] = v;
}

int popInt(StackOfInt* s) {
    return s->_t[--s->_sz];
}

BOOL isStackEmpty(StackOfInt* s) {
    return s->_sz == 0;
}