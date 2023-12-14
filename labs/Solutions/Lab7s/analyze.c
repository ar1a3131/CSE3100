#include "analyze.h"
#include <string.h>
#include <assert.h>

void analyzeURL(char* url,char* host,int* port,char* doc)
{
   if (strncmp(url,"http://",7) == 0) 
      url += 7;
   char* h = url;
   while (*url && *url != ':' && *url != '/') ++url;
   strncpy(host,h,url-h);
   host[url-h] = 0;
   if (*url == ':') {
      url += 1; // to skip ':'
      int x = 0;      
      while (*url && *url != '/') {
         x = x * 10 + *url - '0';
         ++url;
      }
      *port = x;
   }
   assert(*url == '/');
   strcpy(doc,url);
}
