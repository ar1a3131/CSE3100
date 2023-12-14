#include "compression.h"
#include <stdlib.h>
#include <assert.h>

unsigned long countBits(char* text, unsigned long sz, Code* codes)
{
  /* TODO: count the bits needed to store the codes
    This takes in a string text of length sz, and a set of Codes. Each
    character corresponds to a Code consisting of nbits, this simply
    sums the number of bits for each Code of each character. To put it
    in a less verbose way: it computes the number of bits needed to
    store the text using the codes.
   */
}

BitStream compress(char* text, unsigned long sz, Code* codes)
{
  /* TODO: compress the given text
    Suppose the compressed string consists of m bits. To make things simple, we'll use an array of char, 
    where each element stores a bit (as either 0 or 1). So, for example, suppose we had the
    following binary codes for the characters A, B and C:

    A: 0
    B: 10
    C: 11
    
    And the string ABAC. We would need 1 bit for each A, 2 bits for
    each B, and 2 bits for each C. For the string, it's a total of 1 +
    2 + 1 + 2 = 6 bits. So, numBits would be set to 6, and we would
    allocate space for 6 characters. And, *bits = {0, 1, 0, 0, 1, 1}.

    This array (and its length) get put into a BitStream struct, which is defined in compression.h. 
    The countBits function returns the number of char you need to allocate for this array.
   */

}

char* decompress(char* bits, unsigned long numBits, unsigned long usz, Node* root)
{
  /*
    This takes a char array, which holds the bits of the compressed
    text. A bit is stored in each char. unz is the uncompressed size
    of the text.
    
    Use the huffman tree and the bits to decompress the text, and
    return a string. Be sure to include the null terminator at the
    end!
   */
  char* text = calloc(usz + 1, sizeof(char));
  unsigned long i = 0;
  unsigned long j = 0;
  while(i < numBits){
    Node* n = root;
    while(n->c == 0 && i < numBits){
      n = bits[i] ? n->right : n->left;
      i++;
    }
    assert(n->c != 0 && j < usz);
    text[j] = n->c;
    j++;
  }
  return text;
}
