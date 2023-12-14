#include "ufind.h"
#include "list.h"
#include <stdlib.h>

void initUFind(UFind* uf,int sz)
{
   uf->sz = sz;
   uf->data = malloc(sizeof(UFNode)*sz);
   for(int i=0;i < sz;i++) {
      uf->data[i].value = i;
      uf->data[i].parent = uf->data + i;
   }
}

void releaseUFind(UFind* uf)
{
   free(uf->data);
}

int findRep(UFind* uf,int r)
{
   UFNode* s = uf->data + r;
   while (s != s->parent)
      s = s->parent;
   return s->value;
}

int mergeUFind(UFind* uf,int a,int b)
{
   int repA = findRep(uf,a);
   int repB = findRep(uf,b);
   if (repA != repB) {
      uf->data[repA].parent = uf->data + repB;
      return TRUE;
   } else
      return FALSE;
}

int isSameSet(UFind* uf,int a,int b)
{
   int repA = findRep(uf,a);
   int repB = findRep(uf,b);
   return repA == repB;
}

void printUFind(UFind* uf)
{
   IntList as[uf->sz];
   for(int i=0;i<uf->sz;i++)
      initList(as+i);
   for(int i=0;i < uf->sz;i++) {
      int r = findRep(uf,i);
      pushFrontList(as+r,i);
   }
   for(int i=0;i < uf->sz;i++) 
      if (lengthList(as+i) > 0)
         printList(as+i);  
   for(int i=0;i < uf->sz;i++) 
      freeList(as+i);
}
