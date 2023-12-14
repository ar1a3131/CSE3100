/* Implementing prefix sum using multiple threads and read-write locks */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Data {
        long long t[1024];
        pthread_rwlock_t rwl;   //creating a read-write lock
} Data;

void prefix_sum(long n, Data* data) {
        if(n==0) {
                pthread_rwlock_wrlock(&data->rwl);
                data->t[0] = 0; //data write
                pthread_rwlock_unlock(&data->rwl);
        } else {
                pthread_rwlock_rdlock(&data->rwl);
                int cond = data->t[n-1] == 0; //data read
            pthread_rwlock_unlock(&data->rwl);

        if(cond)
                        prefix_sum(n-1, data);

                pthread_rwlock_rdlock(&data->rwl);
                long long temp = data->t[n-1]; //data read
                pthread_rwlock_unlock(&data->rwl);

                pthread_rwlock_wrlock(&data->rwl);
                data->t[n] = n + data->t[n-1]; //data write
                pthread_rwlock_unlock(&data->rwl);
        }
}

void doWork(Data* data) {
        long sv = ((long) ((long)pthread_self() >> 16)) % 100;
        prefix_sum(sv, data);

        pthread_rwlock_rdlock(&data->rwl); //implement a reader lock to read the data
        printf("Called by %ld with %ld got: %lld\n", pthread_self(), sv, data->t[sv]);
        pthread_rwlock_unlock(&data->rwl);
}

int main(int argc, char* argv[]) {
        int nb = atoi(argv[1]); //get the number of threads from the user
        Data data;
        for(int i=0; i<sizeof(data.t)/sizeof(long long);i++)
                data.t[i] = 0;

        pthread_rwlock_init(&data.rwl, NULL); //initialize the read-write lock

        pthread_t minion[nb]; //declare and create nb threads; each thread calls the function doWork and takes as input the ADT data
        for(int i=0; i<nb; i++) {
                pthread_create(minion+i, NULL, (void*(*)(void*))doWork, &data);
        }

        for(int i=0; i<nb; i++)
                pthread_join(minion[i], NULL); //join all threads

        pthread_rwlock_destroy(&data.rwl); //destroy the read-write lock

        return 0;
}