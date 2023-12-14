#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sarray.h"

/* input holds a shared array and the number of values to insert nb */
typedef struct Input {
	SArray* a;
	int nb;
} Input;

void* insert(void* input) {
	Input* in = input;
	for(int i=0; i<in->nb; i++)
		setValueAt(in->a, i, i);
	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t tid1, tid2;
	int status, i, rv;
	int sz = atoi(argv[1]);                 /* size/capacity of the array */
	Input input = {makeAnArray(sz), sz};    /* number of elements to input in array is sz */

    /* each thread inserts sz number of elements in the array. access to the array must be coordinated */
	status = pthread_create(&tid1, NULL, (void*(*)(void*))insert, (void*)&input);   
	status = pthread_create(&tid2, NULL, (void*(*)(void*))insert, (void*)&input);   

	void *v1, *v2;
	pthread_join(tid1, &v1);
	pthread_join(tid2, &v2);

	for(i=0; i<sz; i++) {
		rv = getValueAt(input.a, i);
		printf("%d ", rv);
	}
	printf("\n");
	
	freeArray(input.a);
	return 0;
}
