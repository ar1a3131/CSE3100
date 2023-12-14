/* Using gdb */
/* Use the -g option during compiling and linking */

#include <stdio.h>
#include <stdlib.h>

double fact(int num) {
    if(num == 1)
        return 1;
    else 
        return num * fact(num - 1);
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage is: ./prog2 NUM\n");
        return -1;
    }
    int n = atoi(argv[1]);
    double res = fact(n);
    printf("The fact of %d is %lf\n", n, res);

    return 0;
}