#include <stdio.h>
#include "queue.h"

void initQueue(Queue* q)
{
  q->one = (Stack*) malloc(sizeof(Stack));
  q->two = (Stack*) malloc(sizeof(Stack));
  initStack(q->one);
  initStack(q->two);
}

void enQueue(Queue* q, int number)
{
  pushStack(q->one, number);
}

int deQueue(Queue* q)
{
  /*
    TODO: Check if stack 2 is empty. If it is, flip all of the elements from stack 1 to stack 2. Then, pop and return the element from stack 2.
   */
  if(isEmpty(q->two)){
    while(!isEmpty(q->one)){
      int element = popStack(q->one);
      pushStack(q->two, element);    
    }
  }
  return popStack(q->two);

}
void freeQueue(Queue* q)
{
  freeStack(q->one);
  free(q->one);
  freeStack(q->two);
  free(q->two);
}

