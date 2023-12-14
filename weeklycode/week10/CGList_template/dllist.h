#ifndef __DLLIST_H
#define __DLLIST_H

#include <stdlib.h>

typedef struct DLNode {
   struct DLNode* next;
   struct DLNode* prv;
   int value;
} DLNode;

typedef struct DLList {
   DLNode* head;
   DLNode* tail;
} DLList;

void initList(DLList* list);
void clearList(DLList* list);
void destroyList(DLList* list);
void insertFrontList(DLList* list,int v);
void insertBackList(DLList* list,int v);
void insertSortedList(DLList* list,int v);
void insertInListBehind(DLList* list,int key,int v);
void removeFromList(DLList* list,int key);
void printListForward(DLList* list);
void printListBackward(DLList* list);

#endif
