/* Using typedef to create aliases in C */
/* Discussion on memory alignment */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person { /*structure declaration */
	int age;
	char gender;
	char name[32];
} TPerson; /* TPerson is an alias for struct Person */

int main() {
	TPerson p1; /* structure definition */
	p1.age = 50;
	p1.gender = 'F';
	strcpy(p1.name, "ABC");

	TPerson p2 = {44, 'M', "DEF"};

	TPerson family[4] = {
		{1, 'F', "ABC"}, {22, 'M', "DEF"},
		{30, 'F', "GHI"}, {40, 'M', "JKL"}
	};

	int kidAge = family[1].age;
    printf("%d\n", kidAge);
    
    /* Q1. How many bytes of memory are we using here? Review memory alignment in C */
    

	return 0;
}
