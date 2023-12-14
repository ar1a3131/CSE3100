/* Demonstrating Basic IO operations in C 
 * Implicit streams [STDIN (0) / STDOUT (1)]; Implict APIs: int getchar(void) / int putchar(int) 
 *
 * Explicit APIs: int fgetc(FILE *stream) / int fputc(int c, FILE *stream) 
 * APIs to use: fopen / fclose / fread/ fwrite from the streams 
 * FILE* fopen(const char *restrict filename, const char *restrict mode);
 * int flcose(FILE *stream);
 * size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
 * size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
 *
 * int feof(FILE *stream); // return TRUE if we reached end of file
 * int fseek(FILE *stream, long offset, int whence)
 *
 * Formatted input/output: scanf and printf 
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int c = fgetc(stdin); //read a character from stdin
    while(!feof(stdin)) {
        fputc(c, stdout); //print the character to stdout
        c = fgetc(stdin);
    }

    /* Fileno is a function that returns the integer file descriptor associated with the stream */
    printf("Stdin: %d\n", fileno(stdin));
    printf("Stdout: %d\n", fileno(stdout));
    printf("Stderr: %d\n", fileno(stderr));
    
    return 0;
}
