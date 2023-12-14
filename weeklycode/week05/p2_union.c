/* Union data type 
 * Have the ability for a value to take different form / types 
 * The forms are mutually exclusive and occupy the same space in memory
 * Only enough space to hold the largest attribute
*/

#include <stdio.h>

typedef union uTag {
    int iVal;
    float fVal;
    char* sVal;
} UType;

int main() {
    UType u;
    u.iVal = 5;
    printf("as an int: %d\n", u.iVal);
    printf("as a float: %f\n", u.fVal);
    printf("as a string: %s\n", u.sVal);
}
