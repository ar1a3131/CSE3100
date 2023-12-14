/* creating a 2-D array - interpreted as an array of arrays */

#include <stdio.h>

void printMat(int n, int x[n][n]) {
	printf("%d\n", n);
	for(int i=0; i<n;i++) {
		for(int j=0;j<n;j++) {
			printf("%d ", x[i][j]);
		}
		printf("\n");
	}
	return;
}

int main() {
	int matrix[3][3] = {{1,0,0}, {0,1,0}, {0,0,1}};
	printMat(3, matrix);
	return 0;
}
