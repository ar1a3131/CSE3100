/* The use of macros define 
 * Defining booleans
 */

#include <stdio.h>      

/* defining macros to emulate boolean data type */
#define BOOL char       /* defining macros BOOL - by convention its in uppercase */
#define TRUE 1
#define FALSE 0

int main(void) {
    BOOL x = TRUE;   /* equivalent to char x = 1; can check preprocessed result using -E (remove #includes) */
    BOOL y = FALSE;  /* equivalent to char y = 0 */
    BOOL w, z;

    w = x && y;
    z = x || y;

    printf("z is %d\n", z);
    printf("w is %d\n", w);

    return 0;
}
