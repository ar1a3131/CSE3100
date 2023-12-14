#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Heap* makeHeap(int msz) {
  Heap* h = (Heap*)malloc(sizeof(Heap));
  h->t = (Node**)malloc(sizeof(Node*)*msz);
  h->sz = 0;
  return h;
}

void freeHeap(Heap* h)
{
   free(h->t);
   free(h);
}

void insertHeap(Heap* h, Node* v)
{
   //[v0,v1,v2,...., vk, 0 , 0, 0 ]
   //sz = k+1
   // [ - ,v0,v1,v2,.... ]
   // left = p * 2
   // right = p * 2 + 1
   // minHeap -> smallest element is at the top. p = 1
   int at = h->sz + 1;
   h->t[at] = v;
   h->sz += 1;
   int p = at;
   while (at > 1) {
      p = at /2;
      if (h->t[p]->freq < v->freq)
         return;
      h->t[at] = h->t[p];
      h->t[p]  = v;
      at = p;
   }
}


void heapifyHeap(Heap* h,int from)
{
  while (1) {
    int l = from * 2, r = from * 2 + 1;
    int s = from;
    if (l <= h->sz &&  h->t[l]->freq < h->t[s]->freq) s = l;
    if (r <= h->sz &&  h->t[r]->freq < h->t[s]->freq) s = r;
    if (s != from) {
      Node* w = h->t[s];
      h->t[s] = h->t[from];
      h->t[from] =w;
      from = s;
    } else break;
  }
}

Node* extractMin(Heap* h)
{
   Node* v = h->t[1];
   h->t[1] = h->t[h->sz];
   h->sz -= 1;
   heapifyHeap(h,1);
   return v;
}
