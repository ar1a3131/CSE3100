#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


pthread_mutex_t mutexFuel;
pthread_cond_t condFuel; //condition variable: an identifier
                         //for a certain signal
/*three operations for cond variables...
1. pthread_cond_wait
2. pthread_cond_broadcast
3. pthread_cond_signal*/

int fuel = 0;


void* fuel_filling(void* arg){
    for (int i  = 0; i < 6; i++){
        pthread_mutex_lock(&mutexFuel);
        fuel += 15;
        printf("Filled %d fuel\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        //pthread_cond_signal(&condFuel); //signals to all the
        //but when you use _signal       //threads that are waiting
        //it goes right back to the     //(all that have called
        //fueling function, even if     //pthread_cond_wait(&condFuel, ...))
        //there is enough fuel for      //are going to be able to continue
        //another car

        //so instead of _signal, which just awakens one thread,
        //_broadcast awakens all of the waiting threads
        pthread_cond_broadcast(&condFuel);
        sleep(1);
    }
}

void* car(void* arg){
    pthread_mutex_lock(&mutexFuel);
    while (fuel < 40){
        printf("No fuel. Waiting... \n");
        pthread_cond_wait(&condFuel, &mutexFuel); //instead of sleep(1)
        //pthread_cond_wait should be in a while loop...
        /*Equivalent to:
        pthread_mutex_unlock(&mutexFuel);
        wait for signal on condFuel
        pthread_mutex_lock(&mutexFuel);*/
    }
    fuel -= 40;
    printf("Got the fuel. Now %d fuel left\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}

int main(int argc, char* argv[]){
    pthread_t th[5];
    pthread_mutex_init(&mutexFuel, NULL); //initializing mutex
    pthread_cond_init(&condFuel, NULL); //initializing condition variable
    for (int i = 0; i < 5; i++){
        if (i ==  4) {
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0){
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &car, NULL) != 0){
                perror("Failed to create thread");
            }   
        }
    }

    for (int i = 0; i < 5; i++){
        if (pthread_join(th[i], NULL) != 0){
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    pthread_cond_destroy(&condFuel);

    return 0;
}