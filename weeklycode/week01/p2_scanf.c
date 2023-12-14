#include <stdio.h>

int main() {
	int x = 34;
	double y = 3.14;
	
	printf("x = %d \n", x);
	printf("y = %lf \n", y);
	
	printf("Enter a new value for x: \n");
	scanf("%d", &x);
	printf("Enter a new value for y: \n");
	scanf("%lf", &y);

	printf("y = %d \n", x);
	printf("y = %lf \n", y);
	
	return 0;
}
