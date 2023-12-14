/* Array declarations inside functions - call by value */

#include <stdio.h>

int fib(int n, int k);

int main() {
	int z = fib(10, 5);     /* call by value */
	printf("The value returned is: %d\n", z);
	return 0;
}

int fib(int n, int k) { /* call by value */
	int x[n];           /* dynamic size - depends on the input to the function */
	for(int i=0; i< n; i++)
		x[i] = 0;
	x[0] = 0;
	x[1] = 1;
	for(int i=2; i<n; i++)
		x[i] = x[i-1] + x[i-2];
	if(0<=k && k < n)
		return x[k];
	else return -1;	
}
