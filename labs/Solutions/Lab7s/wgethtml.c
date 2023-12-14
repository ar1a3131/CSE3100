#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <assert.h>
#include "analyze.h"

void checkError(int status,int line)
{
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

char* getURL(int sid,char* url);

int main(int argc,char* argv[])
{
   // Create a socket
   if (argc < 2) {
      printf("Usage is: wgettext <url>\n");
      return 1;
   }
   char host[512];
   int  port = 80;
   char url[512];
   analyzeURL(argv[1],host,&port,url);
   printf("[%s] [%d] [%s]\n",host,port,url);
   
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in srv;
   struct hostent *server = gethostbyname(host);
   if (server==NULL) {
      printf("Couldn't find a host named: %s\n",argv[1]);
      return 2;
   }
   srv.sin_family = AF_INET;
   srv.sin_port   = htons(port);
   memcpy(&srv.sin_addr,server->h_addr_list[0],server->h_length);
   int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
   checkError(status,__LINE__);

   char* response = getURL(sid,url);
   printf("%s\n",response);
   free(response);
   close(sid);
   return 0;
}

char* readResponse(int sid)
{
   size_t sz = 8;
   char* buf = malloc(sizeof(char)* sz);
   int done = 0,received = 0;  
   while (!done) {
      int nbb = read(sid,buf + received,sz - received);
      if (nbb > 0)
         received += nbb;
      else if (nbb == 0)
         done = 1;
      if (received == sz) {
         buf = realloc(buf,sz * 2);
         sz  *= 2;
      }
   }
   if (received == sz)
      buf = realloc(buf,sz + 1);
   buf[received] = 0;
   return buf;
}

char* getURL(int sid,char* url)
{
   char request[1024];
   sprintf(request,"GET %s\n",url);
   int sent = 0,toSend = strlen(request);
   while (sent != toSend) {
      int nbb = write(sid,request + sent,toSend - sent);
      sent += nbb;
   }
   char* buf = readResponse(sid);
   return buf;
}
