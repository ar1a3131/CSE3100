/* Implementing Floyd's triangle */

#include <stdio.h>

int main(void)
{
    int n = 0;
    printf("Enter number of rows: ");
    int count = 1;
    /* TODO: Read in the number of rows from the user, and place it in n */
    scanf("%d", &n);
    for(int i = 1; i <= n; i++){
        /*

        TODO: Print row i of the Floyd triangle.

        */
	for (int j = 1; j <= i; j++){
        	printf("%d ", count);
		count++;
	}
        //This printf prints a newline.
        printf("\n");
 
}
return 0;
}
