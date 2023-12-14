#ifndef __HEAPHEADER_H
#define __HEAPHEADER_H

typedef struct Heap {
	int* t;
	int sz;
}Heap;

Heap* makeHeap(int msz);
void freeHeap(Heap* h);
void insertHeap(Heap* h, int v);
void printHeap(Heap* h);
int emptyHeap(Heap* h);
int extractMin(Heap* h);

#endif
