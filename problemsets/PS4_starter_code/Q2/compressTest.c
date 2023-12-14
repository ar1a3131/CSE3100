#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
  Code* codes = calloc(NUM_SYMBOLS, sizeof(Code));
  codes['b'].nbits = 1;
  codes['b'].bits = 1;
  
  codes['c'].nbits = 2;
  codes['c'].bits = 0;
  
  codes['r'].nbits = 3;
  codes['r'].bits = 3;

  codes['s'].nbits = 3;
  codes['s'].bits = 2;

  char* s = "bbbbbbbbbbbbbbbbbbbccrrss";
  BitStream bs = compress(s, strlen(s), codes);
  printf("Bit stream: ");
  for(unsigned long i = 0; i < bs.numBits; i++){
    if(bs.data[i]){
      printf("1");
    }else{
      printf("0");
    }
  }
  printf("\n");
  return 0;
}
