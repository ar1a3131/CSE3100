#ifndef __FILE_H
#define __FILE_H

#include "huffman.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct CompressionHeader{
   unsigned long* freq;
   unsigned long usz; // usz is uncompressed size in bytes 
   unsigned long csz; //csz is compressed size in bytes
   //padding is number of padding bits.
   char padding;  
} CompressionHeader;

typedef struct ByteArray{
  char* bytes;
  unsigned long numBytes;
  char padding;
} ByteArray;


CompressionHeader readCompressionHeader(FILE* fp);
void writeCompressionHeader(FILE* fp, CompressionHeader h);

void printVerbose(Code* codes, CompressionHeader ch);

unsigned long unpackBytes(ByteArray ba, char** bits);
ByteArray packBits(char* bits, unsigned long numBits);
#endif
