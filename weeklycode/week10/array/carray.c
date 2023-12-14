#include "carray.h"
#include <stdlib.h>
#include <pthread.h>

/* Allocate space for the array and each cell. Initialize the value in each cell to 0 and initialize each mutex */
CArray* makeAnArray(int sz)
{
  CArray* rv = (CArray*)malloc(sizeof(CArray));
  rv->_sz = sz;
  rv->_data = (ArrayCell*)malloc(sizeof(ArrayCell)*sz);
  for(int i=0;i < sz;i++) {
    rv->_data[i]._val = 0;
    pthread_mutex_init(&rv->_data[i]._lock, NULL);
  }
  return rv;
}

/* Destroy each mutex and free the allocated memory */
void freeArray(CArray* a)
{
  for(int i =0;i < a->_sz;i++)
    pthread_mutex_destroy(&a->_data[i]._lock);
  free(a->_data);
  free(a);
}

/* Get value at the index idx */
int getValueAt(CArray* a,int idx)
{
  pthread_mutex_lock(&a->_data[idx]._lock);
  int rv = a->_data[idx]._val;
  pthread_mutex_unlock(&a->_data[idx]._lock);
  return rv;
}

/* Set the value val at index idx */
void setValueAt(CArray* a,int idx,int val)
{
  pthread_mutex_lock(&a->_data[idx]._lock);
  a->_data[idx]._val = val;
  pthread_mutex_unlock(&a->_data[idx]._lock);
}
