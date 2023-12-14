#include <stdio.h>
#include <stdlib.h>

int caesar(int c, int shift) {
	int sc = ((c - 'A') + shift) % 26; //produced 0-based offset of each character
	return sc + 'A';
}

int main(int argc, char* argv[]) {
	/* Caesar cipher
	 * take as input a sequence of characters from stdin
	 * ABCD
	 * take as command line input an integer 
	 * 3
	 * Cipher: shift the characters by the integer: ABCD(3) --> DEFG
	 * wrap around towards the end: Z --> C
	 * return as output a sequence of characters shifted by the integer on stdout
	 */
	if(argc < 2) {
		printf("usage is: prog2_cipher <integer>\n");
		return -1;
	}
	
	int shift = atoi(argv[1]);
	int c = getchar();
	
	while(c!=EOF) {
		int t = caesar(c, shift);
		putchar(t);
		c = getchar();
	}
	printf("\n");
	return 0;
}
