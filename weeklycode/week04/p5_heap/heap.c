#include "heap.h"
#include<stdio.h>
#include<stdlib.h>

Heap* makeHeap(int msz) {
	Heap* h = (Heap*)malloc(sizeof(Heap));
	h->t = (int*)malloc(sizeof(int)*msz);
	h->sz = 0;
	return h;
}

void freeHeap(Heap* h) {
	free(h->t);
	free(h);
}

void insertHeap(Heap* h, int v) {
	// [v0, v1, v2,....,vk,..,0,0,0]
	// sz = k+1
	// [-, v0, v1, v2,...]
	// left = parent * 2
	// right = parent * 2 + 1
	// minHeap -> smallest element is at the top. p=1
	int at = h -> sz+1;
	h->t[at] = v;
	h->sz += 1;
	int p = at;
	while(at>1) {
		p = at / 2;
		if(h->t[p] < v)
			return;
		h->t[at] = h->t[p];
		h->t[p] = v;
		at = p;
	}
}

void printHeapAux(Heap* h, int from) {
	if(from < h->sz+1) {
		printf("(");
		printf("%d ",h->t[from]);
		printHeapAux(h,from*2);
		printf(" ");
		printHeapAux(h, from*2 + 1);
		printf(")");
	} else printf(".");
	
}

void printHeap(Heap* h) {
	printHeapAux(h, 1);
	printf("\n");
}

int emptyHeap(Heap* h) {
	return h->sz == 0;
}

void heapifyHeap(Heap* h, int from) {
	while(1) {
		int l = from *2, r = from * 2 +1;
		int s = from;
		if(l < h->sz && h->t[l] < h->t[s])
			s = l;
		if(r < h->sz && h->t[r] < h->t[s])
			s = r;
		if(s!=from) {
			int w = h->t[s];
			h->t[s] = h->t[from];
			h->t[from] = w;
			from= s;
		} else break;
	}
}

int extractMin(Heap* h) {
	int v = h->t[1];
	h->t[1] = h->t[h->sz];
	h->sz -= 1;
	heapifyHeap(h, 1);
	return v;
}

