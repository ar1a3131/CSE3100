#include "dllist.h"
#include <stdio.h>
#include <assert.h>

void initList(DLList* list)
{
   list->head = list->tail = NULL;
   pthread_mutex_init(&list->mtx,NULL);
}

void clearList(DLList* list)
{
   pthread_mutex_lock(&list->mtx);
   DLNode* cur = list->head;
   while (cur) {
      DLNode* next = cur->next;
      pthread_mutex_destroy(&cur->mtx);
      free(cur);
      cur = next;
   }
   list->head = list->tail = NULL;
   pthread_mutex_unlock(&list->mtx);
}

void destroyList(DLList* list)
{
   clearList(list);
   pthread_mutex_destroy(&list->mtx);
}

void insertSortedList(DLList* list,int v)
{
  /*
    TODO: Iterate through the list, find the correct place to insert the integer v. 
    Each node has a mutex; use those mutexes to coordinate the threads. 
    Note: Your code will be checked manually to ensure that fine-grained locking is used. 
    If you use coarse-grained locking to pass the tests, no points will be awarded.
   */

   // acquire the lock for the entire list before proceeding
   pthread_mutex_lock(&list->mtx);

   // allocate memory for the new node
   DLNode* newNode = (DLNode*)malloc(sizeof(DLNode));

   // handle memory allocation failure, possibly by returning from the function
   if (!newNode) {
      pthread_mutex_unlock(&list->mtx);
      return;
   }
   // set the value of the new node
   newNode->value = v;
   newNode->next = newNode->prv = NULL;

   // initialize the mutex for the new node
   pthread_mutex_init(&newNode->mtx, NULL);

   // check if list is empty
   if (list->head == NULL) {
      list->head = list->tail = newNode;
      pthread_mutex_unlock(&list->mtx); // done here in this case, unlock the list
      return;
   }

   // if there is something in the list
   DLNode* current = list->head;
   DLNode* previous = NULL;

   // find the correct position in the sorted list
   while (current != NULL && current->value < v) {
      previous = current;
      current = current->next;
   }

   // correct position is found, start locking nodes to perform the insertion
   if (previous) { pthread_mutex_lock(&previous->mtx); }
   if (current) { pthread_mutex_lock(&current->mtx); }

   // insert the node
   newNode->next = current;
   newNode->prv = previous;

   if (previous) {
      previous->next = newNode;
      pthread_mutex_unlock(&previous->mtx); // unlock only if it is not NULL
   } else {
      list->head = newNode; // newNode becomes the new head if prev is NULL
   }

   if (current) {
      current->prv = newNode; // unlock only if it is not NULL
      pthread_mutex_unlock(&current->mtx);
   } else {
      list->tail = newNode; // if current is NULL, newTail is newNode
   }

   pthread_mutex_unlock(&list->mtx);
}

void printListForward(DLList* list)
{
   pthread_mutex_lock(&list->mtx);
   DLNode* cur = list->head;
   pthread_mutex_t* last = &list->mtx;
   while (cur) {
      pthread_mutex_lock(&cur->mtx);
      printf("%d ",cur->value);
      pthread_mutex_unlock(last);
      last = &cur->mtx;
      cur = cur->next;
   }
   printf("\n");
   pthread_mutex_unlock(last);
}