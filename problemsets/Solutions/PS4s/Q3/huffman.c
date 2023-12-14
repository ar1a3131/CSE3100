#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"
#include "heap.h"

void extractCodesAux(Node* node, Code* codes,unsigned long long path, char nbits)
{
  if(node->c == 0){
    extractCodesAux(node->left, codes, path << 1, nbits + 1);
    extractCodesAux(node->right, codes, path << 1 |  1, nbits + 1);
  } else {
    assert(codes[node->c].nbits == 0);
    codes[node->c].bits = path;
    codes[node->c].nbits = nbits;
  }
}

Code* extractCodes(Node* root)
{
  Code* codes = calloc(NUM_SYMBOLS, sizeof(Code));
  extractCodesAux(root, codes, 0, 0);
  return codes;
}

char* codeToString(Code code)
{
  char nbits = code.nbits;
  printf("#bits: %d\n",nbits);
  unsigned long long bits = code.bits;
  char* s = calloc(nbits + 1, sizeof(char));
  for(int i = 0; i < nbits; i++){
    s[nbits - i - 1] = bits & 1 ? '1' : '0';
    bits = bits >> 1;
  }
  return s;
}


void freeHuffmanTree(Node* root)
{
  if(root->c == 0){
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
  }
  free(root);
}

Node* createHuffmanTree(unsigned long* freq)
{
  Heap* h = makeHeap(2*NUM_SYMBOLS - 1);
  for(int i = 0; i < NUM_SYMBOLS; i++){
    if(freq[i] > 0){
      Node* w = malloc(sizeof(Node));
      w->freq = freq[i];
      w->c = (char) i;
      w->left = NULL;
      w->right = NULL;
      insertHeap(h, w);
    }
  }
  while(h->sz > 1){
    Node* minOne = extractMin(h);
    Node* minTwo = extractMin(h);
    Node* internal = malloc(sizeof(Node));
    internal->freq = minOne->freq + minTwo->freq;
    internal->c = 0;
    internal->left = minOne;
    internal->right = minTwo;
    insertHeap(h, internal);
  }
  Node* root = extractMin(h);
  freeHeap(h);
  return root;
}
