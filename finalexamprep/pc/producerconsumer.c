#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 2

//command that compiles: gcc -g -pthread producerconsumer.c -o pc

pthread_mutex_t mutexBuffer; 

int buffer[10];
int count = 0;

void* producer(void* args){
    while (1){
        //Produce step
        int x = rand() % 100; //any # btwn 0 and 99

        pthread_mutex_lock(&mutexBuffer); //ADDING TO COUNT
        // Add to the buffer
        if (count > 10){
            buffer[count] = x;
            count++;
        } else {
            printf("Skipped %d\n", x);
        }
        pthread_mutex_unlock(&mutexBuffer);
    }
}

void* consumer(void* args) {
    while (1){
        int y = -1;

        pthread_mutex_lock(&mutexBuffer); //SUBTRACTING FROM COUNT
        //remove from the buffer
        if (count > 0){
            y = buffer[count - 1]; //taking the last element that has been added to buffer
                                    //...you can make it like a stack (last in , first out),
                                    //but that involves moving elements right or left
            count--;
        }
        pthread_mutex_unlock(&mutexBuffer);
        //"Consume"
        printf("Got %d from producer.\n", y);
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL)); // the producer generates a random number
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);
    int i;
    for (i = 0; i < THREAD_NUM; i++){
        if (i % 2 == 0){
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0){ //half of the                                                         
                perror("Failed to create thread");                   //threads are
                                                                     //PRODUCERS
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0){ //half of the
                perror("Failed to create thread");                   //threads are
                                                                     //CONSUMERS
            }
        }
    }
    for (i = 0; i < THREAD_NUM; i++){
        if (pthread_join(th[i], NULL) != 0){
                perror("Failed to join thread"); //joining the producers and consumers
            }
    }
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}

/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 8

sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

int buffer[10];
int count = 0;

void* producer(void* args) {
    while (1) {
        // Produce
        int x = rand() % 100;
        sleep(1);

        // Add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        buffer[count] = x;
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* args) {
    while (1) {
        int y;

        // Remove from the buffer
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        y = buffer[count - 1];
        count--;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        // Consume
        printf("Got %d\n", y);
        sleep(1);
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[THREAD_NUM];
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i > 0) {
            if (pthread_create(&th[i], NULL, &producer, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}*/