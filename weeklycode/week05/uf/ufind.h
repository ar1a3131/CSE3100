#ifndef __UFIND_H
#define __UFIND_H

#define TRUE 1
#define FALSE 0

typedef struct UFNode {
   int value;
   struct UFNode* parent;
} UFNode;

typedef struct UFind {
   int sz;
   UFNode* data;
} UFind;

void initUFind(UFind* uf,int sz);
void releaseUFind(UFind* uf);
int mergeUFind(UFind* uf,int a,int b);
int isSameSet(UFind* uf,int a,int b);
void printUFind(UFind* uf);

#endif
