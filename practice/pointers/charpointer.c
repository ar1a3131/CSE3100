#include <stdio.h>
#include <stdlib.h>


char text[4] = "text";
char* t = text;

int main(){
	printf("t is the pointer and text is the array\n");	
	for (int i = 0; i < 4; i++){
		printf("dereferencing (t+i): %c  (t+i): %d    text[i]: %c   (text+i): %d   dereferencing (text+i): %c  t[i]: %c\n", *(t+i), (t+i), text[i], (text + i), *(text + i), t[i]);
	}
	return 0;
}
