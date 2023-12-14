/* Passing array as arguments to function - call by reference */

#include <stdio.h>

int fib(int k, int x[10]);

int main() {
	int t[10];
	int a0 = fib(5, t); /* passing a reference of t declared in the main stack frame. A new copy of t is not created in fib */
	int a1 = t[8];
	printf("Got 2 values: %d and %d \n", a0, a1);
	return 0;
}

int fib(int k, int x[10]) { /* passing an array of fixed size */
	for(int i=0; i< 10; i++)
		x[i] = 0;
	x[0] = 0;
	x[1] = 1;
	for(int i=2; i<10; i++)
		x[i] = x[i-1] + x[i-2];
	if(0<=k && k < 10)
		return x[k];
	else return -1;	
}
