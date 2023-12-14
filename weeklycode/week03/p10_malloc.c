/* Dynamic memory allocation using malloc/calloc and pointer arithmetic */

#include <stdio.h>
#include <stdlib.h>

int main() {
	int* pox;
	int size = 4;

	pox = (int*)malloc(sizeof(int)*size); /* points to the start of memory block allocated - array of integers */
	
    *pox = 0; /* points to the first value stored and initializes it to 0 */

	for(int i=0; i < size; i++) {
		pox[i] = i + 10;
		printf("The value at index i = %d is %d \n", i, pox[i]);
	}
	
	/* print values */
	for(int i=0; i<size; i++) {
		printf("The value at index i = %d is %d. The address is %p\n", i, *(pox+i), pox+i); /* accessing values through offset of i */
	}

	free(pox); /* ALWAYS FREE ANY MEMORY YOU MALLOC AND CALLOC */

	return 0;
}
