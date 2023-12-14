#include <stdio.h>

int main() {
	int x;
	scanf("Enter number to left shift, x: \n", &x);
	int result = x << 1;
	printf("%d %d", x, result);
	printf("%d << 1 = %d\n", x, result);
	return 0;
}
