#include "compression.h"
#include <stdlib.h>
#include <assert.h>


unsigned long countBits(char* text, unsigned long sz, Code* codes)
{
  /*
    This takes in a string text of length sz, and a set of Codes. Each
    character corresponds to a Code consisting of nbits, this simply
    sums the number of bits for each Code of each character. To put it
    in a less verbose way: it computes the number of bits needed to
    store the text using the codes.
   */
  unsigned long numBits = 0;
  for(unsigned long i = 0; i < sz; i++){
    numBits += codes[text[i]].nbits;
  }
  return numBits;
}

BitStream compress(char* text, unsigned long sz, Code* codes)
{
  /*
    As the name suggests, this compresses text. The bits is a pointer
    to a char*; this means that *bits is the actual array storing the
    bits. We did it this way so that the compress function would be
    responsible for allocating the array of bits, rather than the
    function that calls compress. Each bit of the compressed text is
    saved as a char in *bits. So, for example, suppose we had the
    following binary codes for the characters A, B and C:

    A: 0
    B: 10
    C: 11
    
    And the string ABAC. We would need 1 bit for each A, 2 bits for
    each B, and 2 bits for each C. For the string, it's a total of 1 +
    2 + 1 + 2 = 6 bits. So, numBits would be set to 6, and we would
    allocate space for 6 characters. And, *bits = {0, 1, 0, 0, 1, 1}.

    Then, we return numBits.
   */
  unsigned long numBits = countBits(text, sz, codes);
  char *bits = calloc(numBits, sizeof(char));
  unsigned long bitIndex = 0;
  for(unsigned long i = 0; i < sz; i++){
    Code c = codes[text[i]];
    for(unsigned long j = 1; j <= c.nbits; j++){
       bits[bitIndex] = (c.bits & 1 << (c.nbits - j)) ? 1 : 0;
      bitIndex++;
    }
  }
  BitStream bs = { numBits, bits};
  return bs;
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
