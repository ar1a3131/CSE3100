#include <stdio.h>


double approxLog(double a, int n)
{
  double delta = (a - 1)/n;
  double sum = (1.0/a + 1.0)/2.0;
  for(int i = 1; i < n; i++){
    sum += 1.0/(1 + delta*i);
  }

  return delta*sum;
  
}

int main(int argc, char* argv[])
{
  double a;
  int n;
  printf("Enter a: ");
  scanf("%lf", &a);
  printf("Enter n: ");
  scanf("%d", &n);
  double logarithm = approxLog(a, n);
  printf("Natural logarithm: %lf\n", logarithm);
  return 0;
}
