#include <stdio.h>

//before it can be read or written, a file has to be opened
//... once you open the file with fopen, fopen returns a pointer
//to be used in the subsequent reads or writes

//the file pointer points to a structure that contains information about the file

FILE *fp = *fopen("example.txt", "r");
char filetext[500];

//fgetc(): this function is used to read a single character from the file
//

