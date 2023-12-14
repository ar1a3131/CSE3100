#include "dllist.h"
#include <stdio.h>
#include <assert.h>

// THE PARAGRAPH BELOW IS TO EXPLAIN THE LOGIC. IF YOU DO NOT NEED THE LOGIC, YOU DO NOT NEED TO READ IT.

// This program implements a thread-safe doubly linked list (DLL) using the pthread library 
// for mutex synchronization. The DLL is encapsulated within a structure called 'DLList', which includes 
// pointers to the head and tail nodes of the list and a mutex ('mtx') for ensuring exclusive 
// access during list operations. The program defines functions for initializing, clearing, and 
// destroying the doubly linked list, as well as for inserting elements in sorted order and printing 
// the list in a forward manner. The insertion function ('insertSortedList') is designed to maintain 
// the sorted order of the list while handling mutex locks to ensure thread safety. Nodes are 
// dynamically allocated using malloc, and each node contains a value, pointers to the next and 
// previous nodes, and an associated mutex for synchronization. The print function (printListForward) 
// also ensures that each node is individually locked during traversal to prevent data hazards.
// The overall logic of the program revolves around careful mutex management to guarantee the integrity 
// and consistency of the doubly linked list in a multi-threaded environment.

// Initialize the doubly linked list
void initList(DLList* list) {
   list->head = list->tail = NULL;
   pthread_mutex_init(&list->mtx, NULL);
}

// Clear the doubly linked list, freeing memory for each node and destroying associated mutexes
void clearList(DLList* list) {
   pthread_mutex_lock(&list->mtx);
   DLNode* cur = list->head;

   // Traverse the list, destroying each node and its mutex
   while (cur) {
      DLNode* next = cur->next;
      pthread_mutex_destroy(&cur->mtx);
      free(cur);
      cur = next;
   }
   list->head = list->tail = NULL;
   pthread_mutex_unlock(&list->mtx);
}

// Destroy the doubly linked list, clearing it and freeing any remaining memory
void destroyList(DLList* list) {
   clearList(list);
   pthread_mutex_destroy(&list->mtx);
}

// Insert an integer into the sorted doubly linked list in a thread-safe manner
void insertSortedList(DLList* list, int v) {
   pthread_mutex_lock(&list->mtx);        // Lock the list to ensure exclusive access during the insertion
   DLNode* cur = list->head, *prv = NULL; // Initialize pointers for current and previous nodes

   // If the list is empty, create a new node and set it as both head and tail
   if (cur == NULL) {
      DLNode* nn = malloc(sizeof(DLNode));   // Allocate memory for a new node                     (SAFE)
      pthread_mutex_init(&nn->mtx, NULL);    // Initialize the mutex of the new node               (SAFE)
      nn->value = v;                         // Set the value of the new node                |SAFE, we're the only |
      nn->next = nn->prv = NULL;             // Set pointers to NULL as it is the only node  |one with access to nn|
      list->head = list->tail = nn;          // Update head and tail to point to the new node
      pthread_mutex_unlock(&list->mtx);      // Unlock the list mutex as we are done modifying it
   } else {
      pthread_mutex_t* lastLock = &list->mtx;

      // Find the correct position to insert the new node while maintaining the sorted order
      while (cur) {      
         pthread_mutex_lock(&cur->mtx);
         // Invariant: CUR is locked, and the mutex leading here is locked.
         if (cur->value > v) break;
         pthread_mutex_unlock(lastLock);        // Release the previous lock
         lastLock = &cur->mtx;                  // Update the lock for the current node
         prv = cur;
         cur = cur->next;                       // Move to the next node
      }       

      // If the correct position is found, insert the new node
      if (cur) {
         // We found the spot: before `cur`
         // cur is locked. lastLock (on the thing leading here) is locked. It could be a node or the list.

         DLNode* prev = cur->prv;               // Because cur is locked, we can access its prv
         DLNode* nxt  = cur;                    // Copy address of a locked node             (SAFE)
         DLNode* nn = malloc(sizeof(DLNode));   // Allocate memory for a new node            (SAFE)
         pthread_mutex_init(&nn->mtx, NULL);    // Initialize the mutex of the new node
         nn->value = v;                         // Set the value of the new node             (SAFE)
         nn->prv = prev;                                                          // (because nobody knows nn)
         nn->next = cur;
         if (prev) prev->next = nn;             // Remember prv is locked, so it's safe to access prev->next
         else list->head = nn;                  // The above applies to the head of the list as well

         nxt->prv = nn;                         // Update the previous pointer of the current node to the new node

         pthread_mutex_unlock(&cur->mtx);       // Unlock the current node
         pthread_mutex_unlock(lastLock);        // Release the previous lock
      } else {
         // If CUR == NULL, only the last node (prv) is locked (lastLock)
         DLNode* nn = malloc(sizeof(DLNode));   // Allocate memory for a new node            (SAFE)
         pthread_mutex_init(&nn->mtx, NULL);    // Initialize the mutex of the new node      (SAFE)
         nn->value = v;                         // Set the value of the new node          (SAFE, we're the only ones with nn)
         nn->next = NULL;                       // Set next to NULL as it is the last node
         nn->prv  = prv;                        // Point back to the former last node
         prv->next = nn;                        // Make the last node point to the new node
         pthread_mutex_unlock(lastLock);        // Release the lock for the last node, we're done modifying prv
         pthread_mutex_lock(&list->mtx);
         list->tail = nn;
         pthread_mutex_unlock(&list->mtx);
      }
   }
}

// Print the values in the doubly linked list in a forward manner
void printListForward(DLList* list) {
   pthread_mutex_lock(&list->mtx);
   DLNode* cur = list->head;
   pthread_mutex_t* last = &list->mtx;

   // Traverse the list, printing each value
   while (cur) {
      pthread_mutex_lock(&cur->mtx);
      printf("%d ", cur->value);
      pthread_mutex_unlock(last);
      last = &cur->mtx;
      cur = cur->next;
   }
   printf("\n");
   pthread_mutex_unlock(last);
}
