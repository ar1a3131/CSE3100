#define CH 0
#define UINT 1
#define INT 2
#define _GNU_SOURCE

#include <stdio.h> 
#include <stdlib.h>

// Union called Values can hold char, unsigned int, or int //
typedef union Values {
  char ch;
  unsigned int uint;
  int integer;
} Values;

// Tagged Union has an integer tag to represent what is being stored within the Values union //
typedef struct Variant {
  int tag;
  Values val;
} Variant; 

typedef int(*VarFun)(Variant*, Variant*);

// Used to set the tag and the correct field in the union //
int setUnion(Variant * tagged_union,int dType, void* value) {
  tagged_union->tag = dType;
  switch(dType) {
	case(CH):
	tagged_union->val.ch = *(char*)value;
	break;
	case(UINT):
	tagged_union->val.uint = *(unsigned int*)value;
	case(INT):
	tagged_union->val.integer = *(int*)value;
	break;
  }
  return 0;
}

// Used to retrieve the value stored in the union //
void * getUnion(Variant * tagged_union) {
  switch(tagged_union->tag) {
	case(CH):
	return (void*)&tagged_union->val.ch;
	case(UINT):
	return (void*)&tagged_union->val.uint;
	case(INT):
	return (void*)&tagged_union->val.integer;
  }
  exit(-1);
}
  
// Used to print the value stored in the union //
void printUnion(Variant * tagged_union) {
  switch(tagged_union->tag) {
	case(CH):
	printf("%c", *(char*)getUnion(tagged_union));
	break;
	case(UINT):
	printf("%u", *(unsigned int*)getUnion(tagged_union));
	case(INT):
	printf("%d", *(int*)getUnion(tagged_union));
	break;
  }
}

// Used to compare the value within two tagged unions that have the same tag (ie store the same datatype) //
int compUnion(const void * tagged_union1, const void * tagged_union2) {
  Variant * tU1 = (Variant*)tagged_union1;
  Variant * tU2 = (Variant*)tagged_union2;
  switch(tU1->tag) {
  case(CH):
    return (tU1->val.ch - tU2->val.ch);
  case (UINT):
    return (tU1->val.uint - tU2->val.uint);
  case(INT):
    return abs(tU1->val.integer) - abs(tU2->val.integer);
  }
  exit(-1);
}

// To sort an array of n tagged unions using qsort //
void sortUnion(Variant * tUnionArray, int n) {
  printf("Printing Unsorted Array:\n[");
  for(int i = 0; i<n-1; i++) {
    printUnion(&tUnionArray[i]);
	printf(", ");
  }
  printUnion(&tUnionArray[n-1]);
  printf("]\n");
  printf("Sorting...\n");
  qsort(tUnionArray, n, sizeof(tUnionArray[n-1]), compUnion);
  printf("Printing Sorted Array:\n[");
  for(int i = 0; i<n-1; i++) {
    printUnion(&tUnionArray[i]);
	printf(", ");
  }
  printUnion(&tUnionArray[n-1]);
  printf("]\n");
}

// To compare two tagged unions that store chars //
int compCH(Variant * tagged_union1, Variant * tagged_union2) {
  return (tagged_union1->val.ch - tagged_union2->val.ch);
}

// To compare two tagged unions that store unsigned ints //
int compUINT(Variant * tagged_union1, Variant * tagged_union2) {
  return (tagged_union1->val.uint - tagged_union2->val.uint);
}

// To compare two tagged unions that store ints //
int compINT(Variant * tagged_union1, Variant * tagged_union2) {
  return abs(tagged_union1->val.integer) - abs(tagged_union2->val.integer);
}

// To compare two tagged unions with the same tag by using an array of function pointers rather than a switch for ascending order //
int compareUnionAscending(const void * tagged_union1, const void * tagged_union2, void * fun_ptrs) {
  Variant * tU1 = (Variant*) tagged_union1;
  Variant * tU2 = (Variant*) tagged_union2;
  VarFun fun_ptr = ((VarFun*)fun_ptrs)[tU1->tag];
  return fun_ptr(tU1, tU2);
}

// To compare two tagged unions with the same tag by using an array of function pointers rather than a switch for descending order (Returns the negation of what compareUnionAscending returns) //
int compareUnionDescending(const void * tagged_union1, const void * tagged_union2, void * fun_ptrs) {
  return -1*compareUnionAscending(tagged_union1, tagged_union2, fun_ptrs);
}

int main() { 
  // Initialize array and determine number of elements //
  int arr[] = {-2, 0, 4, -9, 5, -7}; 
  int n = sizeof(arr)/sizeof(arr[0]);

  // Initialize array of Variants and populate with values in arr //
  Variant tUnionArray[n];
  for(int i=0; i<n; i++) {
	setUnion(&tUnionArray[i], INT, (void*)&arr[i]);
  }

  // Initialize array of function pointers //
  void * fun_ptrs[3];
  fun_ptrs[CH] = &compCH;
  fun_ptrs[UINT] = &compUINT;
  fun_ptrs[INT] = &compINT;
  
  printf("Sort Unions with integers using sortUnion:\n");
  sortUnion(tUnionArray, n);
  
  printf("\nSort Unions with integers using compareUnionDescending:\n");
  
  printf("Unsorted: [");
  for(int i = 0; i<n-1; i++) {printf("%d, ", *(int*)getUnion(&tUnionArray[i]));}
  printf("%d]\n", *(int*)getUnion(&tUnionArray[n-1]));
  
  printf("Sorting...\n");
  qsort_r(tUnionArray, n, sizeof(tUnionArray[n-1]), compareUnionDescending, fun_ptrs);
  
  printf("Sorted: [");
  for(int i = 0; i<n-1; i++) {printf("%d, ", *(int*)getUnion(&tUnionArray[i]));}
  printf("%d]\n", *(int*)getUnion(&tUnionArray[n-1]));

  // Initialize array and determine number of elements //
  unsigned int uarr[] = {10, 99, 6000, 8, 45, 29, 77, 1, 7}; 
  int u = sizeof(uarr)/sizeof(uarr[0]);

  // Initialize array of Variants and populate with values in uarr //
  Variant tUnionuArray[u];
  for(int i=0; i<u; i++) {
	setUnion(&tUnionuArray[i], INT, (void*)&uarr[i]);
  }

  printf("\nSort Unions with unsigned integers using compareUnionAscending:\n");
  
  printf("Unsorted: [");
  for(int i = 0; i<u-1; i++) {printf("%u, ", *(int*)getUnion(&tUnionuArray[i]));}
  printf("%d]\n", *(int*)getUnion(&tUnionuArray[u-1]));
  
  printf("Sorting...\n");
  qsort_r(tUnionuArray, u, sizeof(tUnionuArray[u-1]), compareUnionAscending, fun_ptrs);
  
  printf("Sorted: [");
  for(int i = 0; i<u-1; i++) {printf("%d, ", *(int*)getUnion(&tUnionuArray[i]));}
  printf("%d]\n", *(int*)getUnion(&tUnionuArray[u-1]));
  
  // Initialize array and determine number of elements //
  char carr[] = {'C','S','E','3','1','0','0',' ','i','s',' ','f','u','n'}; 
  int c = sizeof(carr)/sizeof(carr[0]);

  // Initialize array of Variants and populate with val in carr //
  Variant tUnioncArray[c];
  for(int i=0; i<c; i++) {
	setUnion(&tUnioncArray[i], CH, (void*)&carr[i]);
  }
  
  printf("\nSort Unions with chars using sortUnion:\n");
  sortUnion(tUnioncArray, c);
}
