#include <stdio.h>
#include <pthread.h>

#define STACK_SIZE 100

typedef struct {
    int data[STACK_SIZE];
    int top;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Stack;

void initStack(Stack *stack) {
    stack->top = -1;
    pthread_mutex_init(&stack->mutex, NULL);
    pthread_cond_init(&stack->cond, NULL);
}

void push(Stack *stack, int value) {
    pthread_mutex_lock(&stack->mutex);

    while (stack->top == STACK_SIZE - 1) {
        pthread_cond_wait(&stack->cond, &stack->mutex);
    }

    stack->data[++stack->top] = value;

    pthread_cond_signal(&stack->cond);
    pthread_mutex_unlock(&stack->mutex);
}

int pop(Stack *stack) {
    pthread_mutex_lock(&stack->mutex);

    while (stack->top == -1) {
        pthread_cond_wait(&stack->cond, &stack->mutex);
    }

    int value = stack->data[stack->top--];

    pthread_cond_signal(&stack->cond);
    pthread_mutex_unlock(&stack->mutex);

    return value;
}

void destroyStack(Stack *stack) {
    pthread_mutex_destroy(&stack->mutex);
    pthread_cond_destroy(&stack->cond);
}

int main() {
    Stack myStack;
    initStack(&myStack);

    // Example usage
    push(&myStack, 10);
    push(&myStack, 20);
    printf("Popped: %d\n", pop(&myStack));
    printf("Popped: %d\n", pop(&myStack));

    destroyStack(&myStack);

    return 0;
}