#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <assert.h>

/* readString reads input from the stdin and returns the \n\0 terminated string  
 * If EOF is read, it returns NULL */
char* readString() {
   int   l = 2; // start with enough space to hold the emtpy string (with \n).
   char* s = malloc(l * sizeof(char));
   int i = 0;
   char ch;
   while ((ch = getchar()) != '\n' && ch != EOF) {
      if (i == l-2) {
         s = realloc(s,l*2*sizeof(char));
         l *= 2;
      }
      s[i++] = ch;
   }
   if (ch == EOF) {
      free(s);
      return NULL;
   } else {
      s[i] = ch;
      s[i+1] = 0;
      return s;
   }
}

/* prints the error that is encountered and terminate the program */
void checkError(int status,int line)
{
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[]) 
{
   if (argc < 3) {
      printf("usage is: Q2client <hostname> <port>\n");
      return 1;
   }

   /* Connect to the server hosted on <hostname> <port>. This should be the same as port1 of server */
   char* hostname = argv[1];
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in srv;
   struct hostent *server = gethostbyname(hostname);
   srv.sin_family = AF_INET;
   srv.sin_port   = htons(atoi(argv[2]));   //same as sid in server
   memcpy(&srv.sin_addr.s_addr,server->h_addr,server->h_length);
   int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
   checkError(status,__LINE__);

   /* ALL of the above is the boilerplate code we give away */
   /* hasKBD is to avoid FAILING if EOF came from the keyboard
      We still want to consume whatever is ready from the socket
   */
   int done = 0,hasKBD= 1; 
   fd_set afd;
   do {
     FD_ZERO(&afd);
     if (hasKBD)
        FD_SET(STDIN_FILENO,&afd);
     FD_SET(sid,&afd);
     int nbReady = select(sid+1,&afd,NULL,NULL,NULL);
     if (nbReady > 0) {
       if (FD_ISSET(0,&afd)) {
          char* line = readString();          
          if (line != 0) {
             int rem = strlen(line),sent=0;
             do {
                int wc = write(sid,line+sent,rem);
                rem -= wc;
                sent += wc;
             } while (rem > 0);
             free(line);
          } else hasKBD=0;
       }
       if (FD_ISSET(sid,&afd)) {
          char buf[256];
          int rc = read(sid,buf,sizeof(buf));
          buf[rc] = 0;
          done = rc == 0;
          if (!done) {
             int rem = rc,sent=0;
             do {
                int wc = write(STDOUT_FILENO,buf+sent,rem);
                rem -= wc;
                sent += wc;
             } while (rem > 0);
          }
       }
     } else done = 1;
   } while(!done);

   return 0;
}
