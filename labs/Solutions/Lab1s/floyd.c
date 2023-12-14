#include <stdio.h>

int main(void)
{
  int n = 0;
  printf("Enter number of rows: ");
  scanf("%d", &n);
  int k = 1;
  for(int i = 1; i <= n; i++){
    for(int j = 0; j < i; j++){
      printf("%d ", k);
      k++;
    }
    printf("\n");
  }
  return 0;
}
