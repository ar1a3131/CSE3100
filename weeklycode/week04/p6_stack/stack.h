#ifndef __STACK_H
#define __STACK_H

#define BOOL char

struct Stack {
    int* _t;
    int _mxs;
    int _sz;
};

typedef struct Stack StackOfInt;

StackOfInt* makeStack(int mxs);
void freeStack(StackOfInt* s);
void pushInt(StackOfInt* s, int v);
int popInt(StackOfInt* s);
BOOL isStackEmpty(StackOfInt* s);

#endif