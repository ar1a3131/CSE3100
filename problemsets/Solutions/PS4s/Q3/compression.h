#ifndef __COMPRESSION_H
#define __COMPRESSION_H

#include "huffman.h"
#include "file.h"

typedef struct BitStream {
   unsigned long numBits;
   char*         data;
} BitStream;

unsigned long countBits(char* text, unsigned long sz, Code* codes);
BitStream compress(char* text, unsigned long sz, Code* codes);
char* decompress(char* bits, unsigned long numBits, unsigned long usz, Node* root);

#endif
