/* Back to scanf - use of & to read values into variables */

#include <stdio.h>

int main() {
	char name[128];
	int pears = 0;
	int apples = 0;
	scanf("%s %d %d", name, &pears, &apples);
	printf("%s ate %d apples and %d pears\n", name, apples, pears);

	return 0;
}
