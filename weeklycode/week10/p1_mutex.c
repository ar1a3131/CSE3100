/* Using MUTEXes to increment a global variable x to cnt concurrently in two threads
    *
    * Mutex is a lock. Stands for mutual exclusion.
    * We need one mutex per "thing" we wish to protect.
    * If a thread can acquire the lock, it can enter the critical section. Once it's complete it can unlock.
    * If a thread fails to acquire the lock, it will go to sleep and wake up once the lock is available.

    * Mutex APIs:
        * Initialize a mutex - int pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t * attr);
            * init takes two arguments: the mutex and its attributes. Attributes define the kind of mutexes we can create.
                * NULL - create the default mutex
                * PTHREAD_MUTEX_NORMAL - does not detect a deadlock. This is the default.
                * PTHREAD_MUTEX_ERRORCHECK - provides error checking for scenarios like deadlock.
                * PTHREAD_MUTEX_RECURSIVE - useful for recursive function - to relock a mutex without first unlocking it. Unlock as many times as you lock.
                * PTHRED_MUTEX_DEFAULT - use the default of the operating system.
        * Lock a mutex - int pthread_mutex_lock(pthread_mutex_t * mutex);
        * Unlock a mutex - int pthread_mutex_unlock(pthread_mutex_t * mutex);
        * Destroy a mutex - int pthread_mutex_destroy(pthread_mutex_t * mutex);

    * Note: By default, mutexes are blocking!!!!
    * Try locking a mutex - int pthread_mutex_trylock(pthread_mutex_t *mutex);
        * if the mutex is not locked, it will succeed and lock
        * if the mutex is already locked, it will not block and return an error code.
        * if you use it, do not forget to check the return value of trylock!!!
    * Rule of thumb: Critical sections must be short to ensure low contention and, therefore, lesser context switching!
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int x = 0;
pthread_mutex_t m;

void increase(int cnt) {
	int i;
	for(i=0;i<cnt;i++) {
		pthread_mutex_lock(&m);             //lock the mutex m
		x = x + 1;                          //critical section
		pthread_mutex_unlock(&m);           //unlock the mutex m
	}
}

int main(int argc, char* argv[]) {
	pthread_t tid1, tid2; //declare two threads
	int status;
	long cnt = atol(argv[1]); //obtain the value of cnt as a command line argument
	pthread_mutex_init(&m, NULL); //initialize the mutex

	status = pthread_create(&tid1, NULL, (void * (*) (void *))increase, (void*)cnt); //create thread 1
	status = pthread_create(&tid2, NULL, (void * (*) (void *))increase, (void*)cnt); //create thread 2

	void *v1, *v2;
	pthread_join(tid1, &v1); //waiting for tid1 to end
	pthread_join(tid2, &v2); //waiting for tid2 to end
	
    pthread_mutex_destroy(&m);
	
    printf("Counter is %d\n", x); //x should equal 2*cnt
	
    return 0;
}

/* Install htop on your VM using: sudo yum install htop 
 * Run htop on cli to see the CPU usage
 */

/* 2 core machine
 * core 1: thread 1 - grabs the mutex m
 * core 2: thread 2 - sleep
    * thread 3 
    * once m is released, thread 3 -> thread 2
    */

/* spinlocks
* core 1: thread 1 - grabs mutex m
* core 2 : thread 2 - busy waiting 
*/