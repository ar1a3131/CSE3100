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
    /* TODO: read a file and fill in the fields: size, contents and freq of the FileData struct.
        Open the file, compute its length (the fseek and ftell functions are helpful here), and read its contents.
        Then, count the character frequencies. I explained how the character frequencies can be stored in an array
        of length NUM SYMBOLS in the section on the createHuffmanTree function.
    */
 
}

void releaseFile(FileData* fd)
{
   /* THIS FUNCTION HAS BEEN COMPLETED FOR YOU. PLEASE DO NOT EDIT IT. */
   freeHuffmanTree(fd->root);
   free(fd->codeWords);
   free(fd->contents);
   free(fd->freq);
   free(fd);
}

void huffmanSetup(FileData* fd)
{
   /* THIS FUNCTION HAS BEEN COMPLETED FOR YOU. PLEASE DO NOT EDIT IT. */
   fd->root = createHuffmanTree(fd->freq);
   fd->codeWords = extractCodes(fd->root);
}

HuffData* huffmanCompress(FileData* fd)
{  
  /* THIS FUNCTION HAS BEEN COMPLETED FOR YOU. PLEASE DO NOT EDIT IT. */
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
