/* Passing array as arguments to function - call by reference */
/* Passing an array of variable length */

#include <stdio.h>

int fib(int n, int k, int x[n]); /* takes as argument an array of size n */

int main() {
	int nb = 10;
	int t[nb];
	int a0 = fib(10, 5, t);
	int a1 = t[8];
	printf("Got two values at indexes 5 and 8: %d %d\n", a0, a1);
	return 0;
}

int fib(int n, int k, int x[n]) { /* does not create a new array x. passes ref of t to x */
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
