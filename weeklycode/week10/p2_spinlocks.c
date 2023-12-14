/* Incrementing a global variable x to cnt concurrently in two threads using spinlocks 
 
 * Preferred over Mutexes if the contention is low and critical sections are short
 
 * Follow the same principle of lock and unlock
    * function spinlock_lock(spinlock_t* mx) {
        while (test_and_set(mx) == 1);     // keep spinning till the value is 1
      }

    * function spinlock_unlock(spinlock_t* mx) {
        *mx = 0;
      }
  
 * Spinlock APIs
    * int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
    * int pthread_spin_destroy(pthread_spinlock_t *lock);
    * int pthread_spin_lock(pthread_spinlock_t *lock);
    * int pthread_spin_trylock(pthread_spinlock_t *lock);
    * int pthread_spin_unlock(pthread_spinlock_t *lock);
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int x = 0;
pthread_spinlock_t spin;

void increase(int cnt) {
	int i;
	for(i=0;i<cnt;i++) {
		pthread_spin_lock(&spin);           //lock the spinlock spin
		x = x + 1;                          //critical section
		pthread_spin_unlock(&spin);         //unlock the spinlock spin
	}
}

int main(int argc, char* argv[]) {
	pthread_t tid1, tid2; //declare two threads
	int status;
	long cnt = atol(argv[1]); //obtain the value of cnt as a command line argument
	pthread_spin_init(&spin, 0); //initialize the spinlock

	status = pthread_create(&tid1, NULL, (void * (*) (void *))increase, (void*)cnt); //create thread 1
	status = pthread_create(&tid2, NULL, (void * (*) (void *))increase, (void*)cnt); //create thread 2
	
	void *v1, *v2;
	pthread_join(tid1, &v1); //waiting for tid1 to end
	pthread_join(tid2, &v2); //waiting for tid2 to end
	pthread_spin_destroy(&spin);
	printf("Counter is %d\n", x); //x should equal 2*cnt
	return 0;
}
