#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"
#include "heap.h"

void extractCodesAux(Node* node, Code* codes,unsigned long long path, char nbits)
{
  /* TODO: extract code for all symbols
   * If the node is internal, recursively call the function on left and right nodes. Do not forget to update the path and nbits value.
   * If the node is a leaf, update the nbits and bits value in codes for the corresponding symbol.
   * 
   * HINT: The bits value is set by the path followed. The left shift operator will come in handy to set the path as you traverse the tree.
   */
}

Code* extractCodes(Node* root)
{
  /* TODO: extract the code for each symbol
   * To do this, allocate memory to store codes for NUM_SYMBOLS number of characters
   * Call the extractCodesAux function to recursively extract codes for all leaves. The call begins from the root node.
   */
}

char* codeToString(Code code)
{
  /* TODO: convert the code into a string
   * To do this, read the bits value and convert nbits number of bits into a char*
   */
}

void freeHuffmanTree(Node* root)
{
  /* TODO: free the huffman tree
   * To do this, recursively free each node of the tree.
   */ 
}

Node* createHuffmanTree(unsigned long* freq)
{
  /* TODO: create a huffman tree. this includes the following steps
   * Create a heap (PQ) using the makeheap function 
   * Create a node for each symbol with frequency > 1 and insert the node in the PQ. 
   * While there is more than 1 node in PQ, extract min 1 and min 2, combine the nodes to create an internal node and insert it into the PQ.
   * When only one node remains, assign it as the root and return it.
   *
   * Note: do not forget to free memory that you no longer need.
   */
}
