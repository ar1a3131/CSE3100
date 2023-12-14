#include<stdio.h>

int main() {
	Heap* h = makeHeap(128);
	for(int i=0; i<10; i++)
		insertHeap(h, 10-i);
	printHeap(h);
	while(!emptyHeap(h)) {
		int v = extractMin(h);
		printf("Got: %d\n", v);
	}
	freeHeap(h);
	return 0;
}
