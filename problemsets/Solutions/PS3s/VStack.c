/* NOTE: To test this program, you must create a copy of vector.c and vector.h that you've implemented in 1.2 */

#include "VStack.h"

void initStack(Stack * s)
{
  initVector(&(s->vector), 2);
}

void pushStack(Stack* stack, int number)
{
  pushBackVector(&(stack->vector), number);
}

int popStack(Stack* stack)
{
  return popBackVector(&(stack->vector));
}

void freeStack(Stack* stack)
{
  freeVector(&(stack->vector));
}

void printStack(Stack* stack)
{
  printVector(&(stack->vector));
}

int isEmpty(Stack* stack)
{
  return lengthVector(&(stack->vector)) > 0;
}
