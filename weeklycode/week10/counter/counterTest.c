/* Incrementing a shared counter that is not a global variable */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "counter.h"

typedef struct Input {
    SCounter* shared;       // pointer to a shared counter
    long cnt;               // argument passed to the thread
} Input;

void increase(Input* input) {
    for(int i=0; i< input->cnt; i++)
        incCounter(input->shared);
}

int main(int argc, char* argv[]) {
    pthread_t tid1, tid2;
    int status;
    
    long cnt = atol(argv[1]);
    Input input = {makeCounter(), cnt};     // resides on the stack of the first thread

    status = pthread_create(&tid1, NULL, (void *(*)(void *))increase, (void *)&input);
    status = pthread_create(&tid2, NULL, (void *(*)(void *))increase, (void *)&input);

    void *v1, *v2;
    pthread_join(tid1, &v1);
    pthread_join(tid2, &v2);

    printf("Counter is %d\n", input.shared->val);

    freeCounter(input.shared);
    
    return 0;
}