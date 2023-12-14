#include <stdio.h>

double fact(int n) {
    if(n < 1)
        return 1;
    else 
        return n * fact(n-1);
}

int main(int argc, char* argv[]){
  printf("n = ");
  int n;
  scanf("%d", &n);
  unsigned int denom = 1;
  double e = 1.0;

  /* Method 1 */
  /*for(int i = 1; i <= n; i++){
    denom *= i;
    e += 1.0/denom;
  }*/

  /* Method 2 */
  for(int i = 1; i <= n; i++) {
      e = e + 1 / fact(i);
  }

  printf("e =  %lf\n", e);
}