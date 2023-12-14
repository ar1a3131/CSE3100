#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	/* argc and argv are command line arguments passed to the main function
	 * argc: argument counter stores the number of arguments passed
	 * argv: array of strings where each string is an argument 
	 */
	
	printf("%d\n", argc);      
	printf("String in argv[0] = %s\n", argv[0]);  //prints the first argument. it is always the name of the executable
	printf("String in argv[1] = %s\n", argv[1]);  //prints the second argument
	
    //the function atoi from stdlib converts an integer string to an integer
	int x;
    x = atoi(argv[1]);
	printf("x is an int = %d\n", x);

	return 0; 
}
