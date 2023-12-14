/* Implementing prefix sum using a single thread */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Data {
        long long t[1024];    // to store the values we compute for prefix sum
} Data;

void prefix_sum(long n, Data* data) {
        if(n==0) {
                data->t[0] = 0;                       //this is a data write operation
        } else {
                if(data->t[n-1]==0)                     //this is a read operation
                        prefix_sum(n-1, data);
                long long temp = data->t[n-1];          //this is a read operation
                data->t[n] = n + temp;              //this is a write operation
        }
}

void doWork(Data* data) {
        long sv = ((long) ((long)pthread_self() >> 16)) % 100;
        prefix_sum(sv, data);
        printf("Called by %ld with %ld got: %lld\n", pthread_self(), sv, data->t[sv]);
}

int main(int argc, char* argv[]) {
        Data data;
        for(int i=0; i<sizeof(data.t)/sizeof(long long);i++)
                data.t[i] = 0;
        for(int i=0; i<10; i++)
                doWork(&data);
        return 0;
}