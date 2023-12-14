#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "buffer.h"

void solve(Task* t) {
   long long ttl = 0;
   for(int i=0;i <= t->query;i++)
      ttl += i;
   t->answer = ttl;
   t->worker = getpid();
}

int main() {
   SBuffer* requests = getBuffer("/requests",100000);
   SBuffer* answers  = getBuffer("/answers",200);
   sem_t* slots   = sem_open("/semaSlots",O_EXCL);
   sem_t* tasks   = sem_open("/semaTasks",O_EXCL);
   sem_t* asw     = sem_open("/semaASW",O_EXCL);
   sem_t* lock    = sem_open("/semaPullLock",O_EXCL);

   /* TODO: 
    * Ensure there is atleast one task in the requests buffer. Hint: use the function sem_trywait(tasks). It returns 0 if there is a task.
    * If a task exists, retreive it, solve it and add the answer to the answers buffer.
    * Make sure you are using all the semaphores to coordinate access.
   */

   while(1) {
      int rv = sem_trywait(tasks);      // returns 0 if there is a task to complete
      if (rv == 0) {
         sem_wait(lock);                    // lock the requests buffer
         Task t = bufferDeQueue(requests);  // dequeue a task
         sem_post(lock);                    // release the lock
         solve(&t);                         // solve the task
         sem_wait(slots);                   // look for slots in the answers buffer
         sem_wait(lock);                    // lock the buffer
         bufferEnQueue(answers,t);          // enqueue your answer
         sem_post(lock);                    // release the lock
         sem_post(asw);                     // raise the sem for answers 
      } else break;
   }
   sem_close(slots);
   sem_close(tasks);
   sem_close(asw);
   sem_close(lock);
   return 0;
}
