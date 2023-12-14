#include <stdio.h>

void decimalToBinary(int decnum){
	if (decnum == 0){
	       	printf("Binary: 0\n"); return;}

	int binary[32];
	int index = 0;

	while (decnum > 0){
		binary[index] = decnum%2;
		decnum = decnum/2;
		index++;
	}

	printf("Binary: ");
	for (int i = index - 1; i>=0; i--){
		printf("%d", binary[i]);
	}
}


int main(){
printf("output of operation, 46 & 25: %d\n", 46&25);

decimalToBinary(46);
printf("\n");
decimalToBinary(25);
printf("\n");
printf("--------------------------------------\n");
int andresult = 46&25;
decimalToBinary(andresult);
printf("\n");
return 0;
}
