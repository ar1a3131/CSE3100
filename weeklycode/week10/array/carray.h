#ifndef __CARRAY_H
#define __CARRAY_H
#include <pthread.h>

/* Each cell consists of an int value and a mutex to coordinate access to the lock */
typedef struct ArrayCell {
  int              _val;
  pthread_mutex_t _lock;
} ArrayCell;

/* An array consists of the total size/capacity of the array and an array of cells */
typedef struct CArray {
  int _sz;
  ArrayCell* _data;
} CArray;

CArray* makeAnArray(int sz);
void freeArray(CArray* a);
int getValueAt(CArray* a,int idx);
void setValueAt(CArray* a,int idx,int val);

#endif
