/* Steps involved in compiling a C program 
 * Preprocess: cc -E -c hello.c -o hello-bis.c
 * Compile: cc -S hello-bis.c -o hello-bis.s
 * Assemble: as -c hello-bis.s -o hello-bis.o
 *
 * Link: cc hello-bis.o -o hello-bis
*/

#include <stdio.h>

int main() {
    printf("Hello world!\n");
    return 0;
}