#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "guess-mt.h"

typedef struct thread_arg_tag {
    gmn_t* sb;
    pthread_mutex_t mutex;          /* Mutex to protect the struct */
    pthread_cond_t cond_guess;      /* signal that a guess is ready */
    pthread_cond_t cond_result;     /* signal that the result is ready */
} thread_arg_t;

void *thread_c(void* consumer_thread_data);
void *thread_p(void* producer_thread_data);

int main(int argc,char* argv[])
{
    thread_arg_t arg;
    arg.sb = gmn_init(atoi(argv[1])); /* initialize the struct to assign a value, initial guess, result and status and message */
    pthread_mutex_init(&arg.mutex, NULL);
    pthread_cond_init(&arg.cond_guess, NULL);
    pthread_cond_init(&arg.cond_result, NULL);

    /* Create 2 threads - one for parent and another for child */
    pthread_t tidc, tidp;
    pthread_create(&tidc, NULL, thread_c, &arg);
    pthread_create(&tidp, NULL, thread_p, &arg);
   
    void *v1, *v2;
    pthread_join(tidc, &v1);
    pthread_join(tidp, &v2);

    pthread_mutex_destroy(&arg.mutex);
    pthread_cond_destroy(&arg.cond_guess);
    pthread_cond_destroy(&arg.cond_result);
    
    return 0;
}

void* thread_p(void *producer_thread_data) {
   thread_arg_t *arg = producer_thread_data;
   int min = 1, max = MAX_VALUE; 
   while(1) {
        pthread_mutex_lock(&arg->mutex);
        while (arg->sb->status == 1)
            pthread_cond_wait(&arg->cond_result, &arg->mutex);
        if(arg->sb->result==0)
            break;
        if(arg->sb->result == -1)
            max = arg->sb->guess - 1;
        if(arg->sb->result == 1)
            min = arg->sb->guess + 1;
        arg->sb->guess = (min + max) / 2;
        snprintf(arg->sb->message, MSG_BUF_SIZE, "My guess is %d.\n", arg->sb->guess); 
        printf("%s\n",arg->sb->message);
        arg->sb->status = 1;
        pthread_cond_signal(&arg->cond_guess);
        pthread_mutex_unlock(&arg->mutex);
   }
   return NULL;
}

void* thread_c(void * consumer_thread_data)
{
    thread_arg_t *arg = consumer_thread_data;
    int result;

    /* TODO
     *  repeat the following until guess is correct 
     *      wait for a guess from thread_p 
     *      call gmn_check() 
     *      send the result and, if the guess is correct, the final message to thread_p
     */
    do {
        pthread_mutex_lock(&arg->mutex);
        while(arg->sb->status == 0)
            pthread_cond_wait(&arg->cond_guess, &arg->mutex);
        arg->sb->result = gmn_check(arg->sb); /* check if the guess is correct */
        result = arg->sb->result;
        printf("%s\n",arg->sb->message);
        arg->sb->status = 0;
        pthread_cond_signal(&arg->cond_result);
        pthread_mutex_unlock(&arg->mutex);
    } while(result!=0);
    return NULL;
}
