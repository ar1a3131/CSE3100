#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char* argv[]){
  unsigned long* freq = calloc(NUM_SYMBOLS, sizeof(unsigned long));
  for(int i = 0; i < strlen(argv[1]); i++){
    freq[argv[1][i]]++;
  }
  Node* root = createHuffmanTree(freq);
  Code* codes = extractCodes(root);
  unsigned long score = 0;
  for(int i = 0; i < NUM_SYMBOLS; i++){
    if(freq[i] > 0){
      char* s = codeToString(codes[i]);
      score += freq[i]*strlen(s);
      printf("%c: %lu, %s\n", i, freq[i], s);
      free(s);
    }
  }
  printf("Number of bits used: %lu\n", score);
  freeHuffmanTree(root);
  free(freq);
  free(codes);
  return 0;

}
