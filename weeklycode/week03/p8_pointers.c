/* Pointers in C - value that denotes the address of a memory cell */
/* What can you do with pointers - get addr of something, deference an addr to get a value, compute the address of something */
/* & operator - address of operator; gets the address of a variable */

#include <stdio.h>

int main() {
	int x = 10;
	printf("The value of x is: %d\n", x);
	
	int* px = &x; /* px is a pointer to an int i.e. it contains an address of an int variable */
	*px = 20; /* follow the pointer px and update the value it points to i.e. updates the value of x from 10 to 20 */

	printf("Address of x is: %p\n", px);
	printf("The value of x is: %d\n", x);

	/* Declaring pointers in C */
	int* p1;
	int * p2;
	int *p3;

	return 0;
}
