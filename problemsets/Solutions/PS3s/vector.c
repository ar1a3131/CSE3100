#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void initVector(IntVector* vec, int cap)
{
  int* numbers = malloc(sizeof(int)*cap);
  vec->numbers = numbers;
  vec->size = 0;
  vec->capacity = cap;
}

void freeVector(IntVector* vec)
{
  free(vec->numbers);
}

void expandVector(IntVector* vec)
{
  vec->capacity *= 2;
  vec->numbers = realloc(vec->numbers, sizeof(int)*vec->capacity);
}

void pushBackVector(IntVector* vec, int number)
{
  if(vec->size == vec->capacity){
    expandVector(vec);
  }
  vec->numbers[vec->size] = number;
  vec->size++;
}

int popBackVector(IntVector* vec)
{
  assert(vec->size > 0);
  vec->size--;
  return vec->numbers[vec->size];
}

void pushFrontVector(IntVector* vec, int number)
{
  if(vec->size == vec->capacity){
    expandVector(vec);
  }
  
  for(size_t i = vec->size; i > 0; i--){
    vec->numbers[i] = vec->numbers[i - 1];
  }
  vec->numbers[0] = number;
  vec->size++;
}

int popFrontVector(IntVector* vec)
{
  assert(vec->size > 0);
  int number = vec->numbers[0];
  for(size_t i = 0; i < vec->size - 1; i++){
    vec->numbers[i] = vec->numbers[i + 1];
  }
  vec->size--;
  return number;
}

void swap(IntVector* vec, size_t indexOne, size_t indexTwo)
{
  assert(indexOne < vec->size);
  assert(indexTwo < vec->size);
  int temp = vec->numbers[indexOne];
  vec->numbers[indexOne] = vec->numbers[indexTwo];
  vec->numbers[indexTwo] = temp;
}

void sortVector(IntVector* vec)
{
  for(size_t i = 1; i < vec->size; i++){
    size_t j = i;
    while(j > 0 && vec->numbers[j - 1] > vec->numbers[j]){
      swap(vec, j, j - 1);
      j--;
    }
  }
}

void printVector(IntVector* vec)
{
  printf("(");
  for(size_t i = 0; i < vec->size; i++){
    printf("%d ", vec->numbers[i]);
  }
  printf(")\n");
}

int lengthVector(IntVector* vec)
{
  return vec->size;
}
