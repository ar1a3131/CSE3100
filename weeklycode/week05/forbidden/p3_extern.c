/* Defining variables as extern */

#include <stdio.h>

int foobar(int);

int main() {
    extern int y;
    int w1 = foobar(10);
    y = 1000;
    int w2 = foobar(10);
    printf("w1 = %d and w2 = %d\n", w1, w2);
}
