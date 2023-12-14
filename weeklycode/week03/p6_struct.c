/* Structures in C - bundle variables of different data types */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person { /*structure definition */
	int age;
	char gender;
	char name[32];
};  /* note the semicolon to end the definition */

int main() {
	struct Person p1; /* structure declaration */
	p1.age = 50;
	p1.gender = 'F';
	strcpy(p1.name, "ABC"); /* because array assignment is not supported in C */

	struct Person p2 = {44, 'M', "DEF"};

	struct Person family[4] = {
		{1, 'F', "ABC"}, {22, 'M', "DEF"},
		{30, 'F', "GHI"}, {40, 'M', "JKL"}
	};

	int kidAge = family[1].age; /* age of the second family member */
    printf("%d\n", kidAge);

	return 0;
}
