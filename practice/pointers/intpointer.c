#include <stdio.h>

int main(){

	int a[5] = {4, 9, 10, 3, 16};
	int* pint;

	printf("numbers in a are: 4, 9, 10, 3, 16\n");

	for (int i = 0; i<5; i++){
		printf("a[i]: %d   (a+i): %d a: %d  deref (a+i): %d  deref pint: %d   deref (pint+i): %d   pint[i]: %d\n", a[i], (a+i), a, *(a+i), *pint, *(pint+i), pint[i]);
	}
return 0;
}
