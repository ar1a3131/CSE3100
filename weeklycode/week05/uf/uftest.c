#include "ufind.h"
#include <stdio.h>

int main()
{
   UFind uf;
   int sz = 0;
   scanf("%d",&sz);
   initUFind(&uf,sz);
   while(1) {
      int a,b;
      if (scanf("%d %d",&a,&b) < 2)
         break;
      mergeUFind(&uf,a,b);
   }
   printUFind(&uf);
   return 0;
}
