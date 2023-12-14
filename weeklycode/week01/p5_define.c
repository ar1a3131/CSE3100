/* #define is a preprocessor directive that allows us to define macros
 * The value of macros is replaced at their occurences during pre-processing stage
 * See the preprocessed output using the -E flag - comment out #include for a cleaner output
 */

#include<stdio.h>
#define PRINTF 42

int main() {
	int x = PRINTF;
	printf("x = %d\n", x);
	return 0;
}
