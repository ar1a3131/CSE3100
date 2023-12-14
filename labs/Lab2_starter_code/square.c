#include <stdio.h>

double relError(double x, double s){
  /*
    TODO: Compute relative error
  */
  double relerror = x*x-s;
  if (relerror < 0){
	relerror *= -1;
 } 
  return relerror/s;
}

int main(int argc, char* argv[]){
  double s;
  printf("Enter S to take square root of: ");
  scanf("%lf", &s);
  double guess = 1;
  /* TODO: Increment guess until guess^2 >= s */
  while (guess*guess <= s){
	guess++;
  }
  /* TODO: Iterate until relative error is less than 0.001 */

  while (relError(guess, s) >= 0.001){
	guess = (guess + s/guess)/2.0;
  }
  printf("Square root: %f\n", guess);
  
  return 0;
}
