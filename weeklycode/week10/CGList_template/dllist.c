#include "dllist.h"
#include <stdio.h>

void initList(DLList* list)
{
   list->head = list->tail = NULL;
}

void clearList(DLList* list)
{
   DLNode* cur = list->head;
   while (cur) {
      DLNode* next = cur->next;
      free(cur);
      cur = next;
   }
   list->head = list->tail = NULL;
}

void destroyList(DLList* list)
{
   clearList(list);
}

void insertFrontList(DLList* list,int v)
{
   DLNode* nn = malloc(sizeof(DLNode));
   nn->value = v;
   nn->next = nn->prv = NULL;
   if (list->head == NULL)
      list->head = list->tail = nn;
   else {
      nn->next = list->head;
      list->head->prv = nn;
      list->head = nn;
   }
}

void insertBackList(DLList* list,int v)
{
   DLNode* nn = malloc(sizeof(DLNode));
   nn->value = v;
   nn->next = nn->prv = NULL;
   if (list->head == NULL)
      list->head = list->tail = nn;
   else {
      list->tail->next = nn;
      nn->prv = list->tail;
      list->tail = nn;
   }
}

void insertSortedList(DLList* list,int v)
{
   DLNode* cur = list->head;
   while (cur && cur->value <= v)
      cur = cur->next;
   if (cur) {
      // We found the spot: before `cur`
      DLNode* prev = cur->prv;
      DLNode* nxt  = cur;
      DLNode* nn = malloc(sizeof(DLNode));
      nn->value = v;
      nn->prv = prev;
      nn->next = cur;
      if (prev) prev->next = nn;
      else list->head = nn;
      nxt->prv = nn;
   } else {
      DLNode* nn = malloc(sizeof(DLNode));
      nn->value = v;
      nn->next = NULL;
      nn->prv = list->tail;
      if (list->tail)
         list->tail->next = nn;
      else list->head = nn;
      list->tail = nn;
   }
}

void insertInListBehind(DLList* list,int key,int v)
{
   DLNode* cur = list->head;
   while (cur && cur->value != key)      
      cur = cur->next;
   if (cur) {
      DLNode* nn = malloc(sizeof(DLNode));
      nn->value = v;
      nn->next = cur->next;
      nn->prv = cur;
      if (cur->next)
         cur->next->prv = nn;
      else list->tail = nn;
      cur->next = nn;      
   } else 
      insertBackList(list,v);  
}

void removeFromList(DLList* list,int key)
{
   DLNode* cur = list->head;
   while (cur && cur->value != key)
      cur = cur->next;
   if (cur) {
      if (cur->prv)
         cur->prv->next = cur->next;
      else list->head = cur->next;
      if (cur->next)
         cur->next->prv = cur->prv;
      else list->tail = cur->prv;
      free(cur);
   }
}

void printListForward(DLList* list)
{
   DLNode* cur = list->head;
   while (cur) {
      printf("%d ",cur->value);
      cur = cur->next;
   }
   printf("\n");
}

void printListBackward(DLList* list)
{
   DLNode* cur = list->tail;
   while (cur) {
      printf("%d ",cur->value);
      cur = cur->prv;
   }
   printf("\n");
}
