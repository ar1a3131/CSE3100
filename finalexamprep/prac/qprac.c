#include <stdio.h>
#include <pthread.h>

#define QUEUE_SIZE 100

typedef struct {
    int data[QUEUE_SIZE];
    int front, rear, size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

void initQueue(Queue *queue) {
    queue->front = queue->rear = -1;
    queue->size = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

void enqueue(Queue *queue, int value) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->size == QUEUE_SIZE) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    if (queue->front == -1) {
        queue->front = 0;
    }

    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->data[queue->rear] = value;
    queue->size++;

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

int dequeue(Queue *queue) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->size == 0) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    int value = queue->data[queue->front];

    if (queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % QUEUE_SIZE;
    }

    queue->size--;

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);

    return value;
}

void destroyQueue(Queue *queue) {
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
}

int main() {
    Queue myQueue;
    initQueue(&myQueue);

    // Example usage
    enqueue(&myQueue, 10);
    enqueue(&myQueue, 20);
    printf("Dequeued: %d\n", dequeue(&myQueue));
    printf("Dequeued: %d\n", dequeue(&myQueue));

    destroyQueue(&myQueue);

    return 0;
}


