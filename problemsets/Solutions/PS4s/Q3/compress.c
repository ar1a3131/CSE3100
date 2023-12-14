#include "huffman.h"
#include "file.h"
#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct FileData {
   long int size;       // file size in bytes
   char* contents;      // actual bytes
   unsigned long *freq; // frequency table.
   Node* root;          // root of huffman tree
   Code* codeWords;     // collection of code words.
} FileData;

typedef struct HuffData {
   FileData* source;
   ByteArray data;
} HuffData;
   
FileData* readFile(char* fName)
{
  FILE* fp = fopen(fName, "r");
  if(fp == NULL){
    printf("input does not exist\n");
    exit(1);
  }
  FileData* fd = malloc(sizeof(FileData));
  fseek(fp, 0L, SEEK_END);
  fd->size = ftell(fp);
  rewind(fp);
  fd->contents = malloc(fd->size);
  fd->freq = calloc(NUM_SYMBOLS,sizeof(unsigned long)); // everyone set to zero
  int c = getc(fp);
  long int i = 0;
  while(c != EOF){
    assert(c < NUM_SYMBOLS);
    fd->contents[i] = c;
    fd->freq[c]++;
    c = getc(fp);
    i++;
  }
  fclose(fp);
  return fd;
}

void releaseFile(FileData* fd)
{
   freeHuffmanTree(fd->root);
   free(fd->codeWords);
   free(fd->contents);
   free(fd->freq);
   free(fd);
}

void huffmanSetup(FileData* fd)
{
   fd->root = createHuffmanTree(fd->freq);
   fd->codeWords = extractCodes(fd->root);
}

HuffData* huffmanCompress(FileData* fd)
{  
  BitStream bs = compress(fd->contents, fd->size, fd->codeWords);
  HuffData* hd = malloc(sizeof(HuffData));
  hd->source = fd;
  hd->data = packBits(bs.data, bs.numBits);
  free(bs.data);
  return hd;
}

void dumpHuffData(HuffData* hd,char* fName)
{
   FILE* fp = fopen(fName, "wb");
   CompressionHeader ch = {hd->source->freq, hd->source->size, hd->data.numBytes, hd->data.padding};
   printVerbose(hd->source->codeWords, ch);
   writeCompressionHeader(fp,ch);
   fwrite(hd->data.bytes, sizeof(char), hd->data.numBytes, fp);
   fclose(fp);
}

void releaseHuffData(HuffData* hd)
{
   free(hd->data.bytes);
   free(hd);
}

int main(int argc, char* argv[]){
  if(argc != 3){
    printf("Usage: ./compress input output\n");
    return 1;
  }
  FileData* theFile = readFile(argv[1]);
  huffmanSetup(theFile);
  HuffData* theData = huffmanCompress(theFile);
  dumpHuffData(theData,argv[2]);
  releaseHuffData(theData); 
  releaseFile(theFile);
  return 0;
}
