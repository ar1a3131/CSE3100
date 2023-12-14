#ifndef __HEAP_H
#define __HEAP_H

#include "huffman.h"

typedef struct Heap {
   Node** t;             // These are nodes of the huffman tree
   int sz;
} Heap;


Heap* makeHeap(int msz);
void freeHeap(Heap* h);
void insertHeap(Heap* h, Node* v);
Node* extractMin(Heap* h);


#endif
