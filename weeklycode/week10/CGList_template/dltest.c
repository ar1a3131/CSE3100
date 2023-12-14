#include "dllist.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct Job {
	DLList* theList;            // shared list
	int nbToDo;                 // no. of values to be inserted by this thread
} Job;

void* minion(void* theData) {
	Job* job = theData;
	int i;
	for(i=0; i<job->nbToDo; i++) {
		insertSortedList(job->theList, i);      // insert i in the list
	}
	return NULL;
}

int main(int argc,char* argv[])
{
    int nbT = atoi(argv[1]); //number of threads to create
    DLList* lst = malloc(sizeof(DLList));
    initList(lst);
    Job job = {lst, 100}; //arguments for the minion
    
    pthread_t tid[nbT]; //declare nbT threads
    for(int i=0; i<nbT;i++) { //create nbT threads
        pthread_create(tid+i, NULL, minion, &job);      // all workers have the same task to add 100 values
    }

    for(int i=0; i<nbT; i++)
        pthread_join(tid[i], NULL);

    printListForward(lst);
        
    destroyList(lst);
    free(lst);
    
    return 0;
}

