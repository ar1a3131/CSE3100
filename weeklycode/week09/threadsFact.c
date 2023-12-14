/* Using threads to compute factorials */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long fact(long n)
{
   if (n==0)
      return 1;
   else return n * fact(n-1);
}

long minionWorker(long value)
{
   long res = fact(value);
   printf("I computed: fact(%ld)=%ld\n",value,res);
   return res;
}

int main(int argc,char* argv[])
{
   long a1 = atol(argv[1]);
   long a2 = atol(argv[2]);
   long v1, v2;

   /* TODO: Use threads to compute factorial of a1 and a2
    * Create 2 threads. 
    * Each thread should call the minionWorker function.
    * Thread 1 should compute the factorial of a1.
    * Thread 2 should compute the factorial of a2.
    * Store the outputs from both threads in v1 and v2.
    */
   pthread_t thread1, thread2;
   int status;
   status = pthread_create(&thread1, NULL, (void * (*) (void *))minionWorker, (void *)a1);

   status = pthread_create(&thread2, NULL, (void * (*) (void *))minionWorker, (void *)a2);



   pthread_join(thread1, (void **)&v1);
   pthread_join(thread2, (void **)&v2);

   printf("Factorial of %ld = %ld and Factorial of %ld = %ld\n",a1, v1, a2, v2);
   
   return 0;
}
