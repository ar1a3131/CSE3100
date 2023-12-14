#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

//command that compiles: gcc -g -pthread intro.c -o intro


/*THREADS*/


void* routine() {
  printf("Test from thread %d\n", getpid()); //when you get
                            //the process IDs of both threads, they are
                            //the same... a process can have multiple threads
                            
  //the bottom is added to check if the threads are executing in parallel
  sleep(3);
  printf("Ending thread\n");
}

int main(int argc, char* argv[]) 
  pthread_t t1, t2; //the type is pthread_t... defines the place where
                //the API can store information abt the thread...
                //t1 = thread1
                //added t2 = thread2
  pthread_create(&t1, NULL, &routine, NULL); //initializes thread...
                                             //pthread_create needs a
                                             //reference to the thread (&t1),
                                             //reference to function, and
                                             //the attribute to said func
        //pthread_create returns an int, so you can do 
        //if (pthread_create(...) != 0){
        //print an error statement}

  pthread_create(&t2, NULL, &routine, NULL); //added t2

  pthread_join(t1, NULL); //we have to wait for the thread to finish its execution...
                   //only 2 parameters... 1) thread that hold the info, 2)
                   // pointer that gets the result from that thread... we are passing NULL
                   // this time bc we dont care about its result
  pthread_join(t2, NULL); //wait until finished executing
  return 0; 
}

/*
PROCESSES: can contain multiple threads in a single process,
            the file descriptors are duplicated
THREADS: cannot have multiple processes for a single thread,
          the same file descriptors are shared (they share memory)*/





