/* Arrays are a type constructor. Represent linear, contiguous collection of things. */

#include <stdio.h>

int main() {
	int x[5]; /* array of 5 integers - allocated automatically on the stack */
	x[0] = 1;
	x[1] = 2;
	x[2] = 3;
	x[3] = 4;
	x[4] = 5;

 	for(int i=0; i<5; i++) 
		printf("%d\t", x[i]);
	printf("\n");

    /* Q1. What happens if we access an index after 4? */
    /*
    x[5] = 10;
    x[-1] = -2;
    for(int i=0; i<5; i++) 
		printf("%d\t", x[i]);
	printf("\n");
    */

	/* Q2. Can we copy an array into another? */
    /*
    int y[5];
    y=x;
    */

    /* Character arrays or strings */

	char s1[6] = {'H', 'e', 'l', 'l', 'o', '\0'}; /* ASCII code for \0 is 0 */
	printf("Array is: %s\n", s1);

	char s2[6] = "Kriti";
	printf("Array is: %s\n", s2);
	
	char s3[] = "Bhargava";
	printf("Array is: %s\n", s3);

	return 0;
}
