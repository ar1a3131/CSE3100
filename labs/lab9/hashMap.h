#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <pthread.h>
#include "dllist.h"

/* a bucket consists of a doubly linked list and a mutex to protect it */
typedef struct Bucket {
  DLList* list;
  pthread_mutex_t lock;
} Bucket;

/* numBuckets - number of buckets, numItems - number of items, buckets - array of buckets, tableLock - mutex for the hash map */
typedef struct HashMap {
  unsigned int numBuckets;
  unsigned int numItems;
  Bucket* buckets;
  pthread_mutex_t tableLock;
} HashMap;

void initHashMap(HashMap* hm, unsigned int nb);
void destroyHashMap(HashMap* hm);
void insertHashMap(HashMap* hm, char* key, char* value);
char* retrieveValue(HashMap* hm, char* key);
void deleteEntry(HashMap* hm, char* key);

#endif
