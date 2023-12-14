#ifndef __HUFFMAN_H
#define __HUFFMAN_H
#define NUM_SYMBOLS 128

typedef struct Node {
  unsigned long freq;
  char c;
  struct Node* left;
  struct Node* right;
} Node;


typedef struct Code {
   char nbits;
   unsigned long long bits; // longest code-word cannot exceed 64 bits.
} Code;


Node* createHuffmanTree(unsigned long* freq);
Code* extractCodes(Node* root);

void freeHuffmanTree(Node* root);
char* codeToString(Code code);

#endif
