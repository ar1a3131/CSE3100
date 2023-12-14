#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex; // == int lock = 0;

/*a mutex is a lock around a section of code*/

void* routine() {
    for (int i = 0; i < 1000000; i++){
        /*if (lock == 1){
            wait until lock is 0
        }
        lock = 1;
        mails++;
        lock = 0;


        instead of this ^^

        do this:
        */
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);

    }
}

int main(int argc, char* argv[]){
    pthread_t p1, p2;
    pthread_mutex_init(&mutex, NULL); //initializing the mutex defined at
                                      //the top of the page...
                                      //every init needs a 
                                      //DESTROY in the end

    pthread_create(&p1, NULL, &routine, NULL);
    pthread_create(&p2, NULL, &routine, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    pthread_mutex_destroy(&mutex); //DESTROY

    printf("Number of mails: %d\n", mails);
    return 0;
}


