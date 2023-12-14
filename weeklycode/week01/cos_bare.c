#include <stdio.h>
#include <stdlib.h>
#include <math.h>   //to link the math library using the flag -lm at compile time

double fact(int n) {
    if(n<=1)
        return 1;
    else
        return n * fact(n-1);
}

int main(int argc, char* argv[]) {
	/* 
    Compute cos_n(x)
	x : the point at which to evaluate the cosine
	n: the degree of the polynomial approximation
	
    The technique:
	We know that cos_n(x) = sum_{k=0}^n (-1)^k * x^{2*k} / (2k)!
	n is the number of terms in the polynomial. What type?
	x is the point on the x axis. What type?
	The result has a type too. What is it?
	
    Write the code here:
	Assume that  (1) n is the first argument on the command line
	 		     (2) x is the second argument on the command line
    Math library has a function pow(x, y) that computes x^y
    The function to compute factorial is given to you.

    */


	return 0;
}
