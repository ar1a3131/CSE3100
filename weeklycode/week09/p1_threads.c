/************************** What is a thread? *******************************/
/* Thread is a sequence of instructions that can be managed independently.
 * It is an abstraction of the OS consisting of a stack and execution state
   * location of the current instruction
   * address space
   * data registers
*/
/*****************************************************************************/

/************************** Life of a thread *********************************/

/* Thread creation - using pthread_create
     * pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
     * 1. pointer to a variable that stores the name (id) of the thread
     * 2. pointer to a structure that holds a collection of options. NULL implies use the default options. 
     * 3. entry point (pointer to the function) for the thread
         * the function takes as argument a void * and returns a void *
     * 4. argument that the start function would take 
     */ 

/* Thread joining 
  * A thread can be detached - thread just dies, parent doesn't care. Resources are claimed automatically.
  * A thread can be joinable (default) - produces a return val (of type void*).
    Another thread can join to pick up the value and reclaim resources.
        * int pthread_join(pthread_t thread, void **value_ptr)
        * arguments: id of the thread to join with and location of a 64-bit wide value where to deposit the result.
        * the call to join is blocking. The calling thread will wait for the thread to exit before proceeding. 
*/

/* Thread termination 
 * A thread can exit normally if it completes the function it is performing.
 * A thread can exit early by calling void pthread_exit(void *retval).
 * A thread can be killed using int pthread_kill(pthread_t thread, int sig)
    * to include signal.h header file
    * takes two inputs - id of the victim and cause for termination
 */
/*************************************************************************************************************************************************************/

/* Example 1:Incrementing a global variable x to cnt concurrently in two threads */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int x = 0; //global variable

void increase(int cnt) {
	int i;
	for(i=0;i<cnt;i++)
		x = x + 1; //incrementing global variable
}

int main(int argc, char* argv[]) {
	pthread_t tid1, tid2, tid3;       //declare two threads... i added third
	int status;
	long cnt = atol(argv[1]); //obtain the value of cnt as a command line argument

	status = pthread_create(&tid1, NULL, (void * (*) (void *))increase, (void*)cnt); //create thread 1
  printf("(after 1st create) Counter is %d\n", x);
	status = pthread_create(&tid2, NULL, (void * (*) (void *))increase, (void*)cnt); //create thread 2
  printf("(after 2nd create) Counter is %d\n", x);
  /*added a third*/
  status = pthread_create(&tid3, NULL, (void * (*) (void *))increase, (void*)cnt); //create thread 2
  printf("(after 3rd create) Counter is %d\n", x);

	void *v1, *v2, /*added third*/ *v3; // the v's are used to pick up
                                      // the reuslts of the threads
                                      //when they terminate 
                                      
/* when one thread wants to wait for another thread to complete,
it invokes pthread_join

int pthread_join(pthread_t thread, void **status);

pthread_t is the data type used to uniquely identify a thread.
It is returned by pthread_create() and used by the application
in function calls that require a thread identifier.

status contains a pointer to the status argument passed by the 
ending thread as part of pthread_exit(). If the ending thread 
terminated with a return, status contains a pointer to the return 
value. If the thread was canceled, status can be set to -1.

*/

  //(Allows the calling thread to wait for the ending of the target thread.)
	pthread_join(tid1, &v1); //waiting for tid1 to end 
  printf("(after first join) Counter is %d\n", x); //**I added this**

	pthread_join(tid2, &v2); //waiting for tid2 to end
	printf("(after second join) Counter is %d\n", x); //x should equal 2*cnt

  //added third
  pthread_join(tid3, &v3);
  printf("(after third join) Counter is %d\n", x);

	return 0;
}


