#include "file.h"

CompressionHeader readCompressionHeader(FILE* fp)
{
  CompressionHeader h;
  h.freq = calloc(NUM_SYMBOLS, sizeof(unsigned long));
  fread(h.freq, sizeof(unsigned long), NUM_SYMBOLS, fp);
  fread(&(h.usz), sizeof(unsigned long), 1, fp);
  fread(&(h.csz), sizeof(unsigned long), 1, fp);
  fread(&(h.padding), sizeof(char), 1, fp);
  return h;
}

void writeCompressionHeader(FILE* fp, CompressionHeader h)
{
  fwrite(h.freq, sizeof(unsigned long), NUM_SYMBOLS, fp);
  fwrite(&(h.usz), sizeof(unsigned long), 1, fp);
  fwrite(&(h.csz), sizeof(unsigned long), 1, fp);
  fwrite(&(h.padding), sizeof(char), 1, fp);
}

unsigned long unpackBytes(ByteArray ba, char** bits)
{
  unsigned long numBits = ba.numBytes*8 - ba.padding;
  *bits = calloc(numBits, sizeof(char));
  for(unsigned long i = 0; i < numBits; i++){
    char byte = ba.bytes[i/8];
    char offset = i % 8;
    (*bits)[i] = (byte >> (7 - offset)) & 1;
  }
  return numBits;
}

void printVerbose(Code* codes, CompressionHeader ch){
  printf("Uncompressed size: %lu\n", ch.usz);
  printf("Compressed size: %lu\n", ch.csz);
  printf("Number of pad bits: %d\n", (int)ch.padding);
  printf("Symbols: \n");
  for(int i = 0; i < NUM_SYMBOLS; i++){
    if(ch.freq[i] > 0){
      char* s = codeToString(codes[i]);
      printf("%c: %lu, %s\n", i, ch.freq[i], s);
      free(s);
    }
  }
}

ByteArray packBits(char* bits, unsigned long numBits)
{
  unsigned long numBytes = numBits/8 + (numBits % 8 == 0 ? 0 : 1);
  char padding = numBytes*8 - numBits;
  char* bytes = calloc(numBytes, sizeof(char));
  for(unsigned int i = 0; i < numBits; i++){
    if(bits[i]){
      bytes[i/8] |= 1 << (7 - i % 8);
    }
  }
  ByteArray ba = {bytes, numBytes, padding};
  return ba;
}
