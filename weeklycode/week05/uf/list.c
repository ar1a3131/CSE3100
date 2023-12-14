#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


void initList(IntList* list)
{
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;  
}
void freeList(IntList* list)
{
  ILNode* cur = list->head;
  while(cur){
    ILNode* n = cur->next;
    free(cur);
    cur = n;
  }
}

void pushBackList(IntList* list, int number)
{
  ILNode* node = malloc(sizeof(ILNode));
  node->number = number;
  node->next = NULL;
  if(list->head){
    list->tail->next = node;
    list->tail = node;
  }else{
    list->head = node;
    list->tail = node;
  }
  list->len++;
}
int popBackList(IntList* list)
{
  assert(list->head && list->len > 0);
  int number = list->tail->number;
  ILNode* cur = list->head;
  ILNode* prev = NULL;
  while(cur && cur->next){
    prev = cur;
    cur = cur->next;
  }
  free(cur);
  list->tail = prev;
  if(list->tail){
    list->tail->next = NULL;
  }else{
    list->head = NULL;
  }
  list->len--;
  return number;
}
void pushFrontList(IntList* list, int number)
{
  ILNode* node = malloc(sizeof(ILNode));
  node->number = number;
  node->next = list->head;
  list->head = node;
  if(list->tail == NULL){
    list->tail = list->head;
  }
  list->len++;
}
int popFrontList(IntList* list)
{
  assert(list->head && list->len > 0);
  int number = list->head->number;
  ILNode* nodeToFree = list->head;
  list->head = list->head->next;
  if(list->head == NULL){
    list->tail = NULL;
  }
  free(nodeToFree);
  list->len--;
  return number;
}

void sortedInsert(IntList* sortedList, ILNode* nodeToInsert){
  if(sortedList->head == NULL){
    nodeToInsert->next = NULL;
    sortedList->head = sortedList->tail = nodeToInsert;
  }else{
    ILNode* cur = sortedList->head;
    ILNode* prev = NULL;
    while(cur && cur->number < nodeToInsert->number){
      prev = cur;
      cur = cur->next;
    }
    nodeToInsert->next = cur;
    if(prev){
      prev->next = nodeToInsert;
    }else {
      sortedList->head = nodeToInsert;
    }
    if(cur == NULL){
      sortedList->tail = nodeToInsert;
    }
  }
  sortedList->len++;
}

void sortList(IntList* list)
{
   IntList newList = {NULL,NULL};
  ILNode* next = NULL;
  ILNode* cur = list->head;
  while(cur){
    next = cur->next;
    sortedInsert(&newList, cur);
    cur = next;
  }
  *list = newList;
}



int lengthList(IntList* list)
{
  return list->len;
}
void printList(IntList* list)
{
   printf("(");
   ILNode* node = list->head;
   while (node) {
      printf("%d ", node->number);
      if(node->next == NULL){
	assert(list->tail == node);
      }
      node = node->next;
   }
   printf(")\n");
}
