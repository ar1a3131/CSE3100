#include <stdio.h>
#include <stdlib.h>

#define N 10000
#define RANDPOINT(a, b) (a + (b - a)*(rand() % RAND_MAX)/RAND_MAX)
#define RANDX(k) (RANDPOINT(1.0, k))
#define RANDY (RANDPOINT(0.0, 1.0))
#define SAMPLE(k) (RANDY <= 1.0/RANDX(k))

int main(){
  srand(50);
  printf("Enter number to take ln of: ");
  double k;
  scanf("%lf", &k);
  unsigned int m = 0;
  for(unsigned int i = 0; i < N; i++){
    if(SAMPLE(k)){
      m++;
    }
  }
  printf("Approximate logarithm: %lf\n", (k - 1.0)*m/N);

  return 0;
}


