#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>

/* prints the error that is encountered and terminate the program */
void checkError(int status,int line)
{
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

/* creates a server-side socket that binds to the given port number and listens for TCP connect requests */
int bindAndListen(int port)
{
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(port);
   addr.sin_addr.s_addr = INADDR_ANY;
   int status = bind(sid,(struct sockaddr*)&addr,sizeof(addr));
   checkError(status,__LINE__);
   status = listen(sid,10);
   checkError(status,__LINE__);
   return sid;
}

/* reaps dead children */
void cleanupDeadChildren()
{
   int status = 0;
   pid_t deadChild; // reap the dead children
   do {
      deadChild = waitpid(0,&status,WNOHANG);
   } while (deadChild > 0);
}

int main(int argc,char* argv[])
{
   if (argc < 3) {
      printf("usage is: Q2server <port1> <port2>\n");
      return 1;
   }

   /* Create two sockets: one to receive SQL commands and another for service */
   int sid = bindAndListen(atoi(argv[1]));  
   int srv = bindAndListen(atoi(argv[2]));  

   while(1) {
      fd_set afd;     /* That's to listen to both sockets */
      FD_ZERO(&afd);
      FD_SET(sid,&afd);
      FD_SET(srv,&afd);
      int mx = sid > srv ? sid : srv;
      int nbReady = select(mx+1,&afd,NULL,NULL,NULL);
      if (nbReady > 0) {
         if (FD_ISSET(sid,&afd)) {
            /* This one is completely classic. Like RSH */
            struct sockaddr_in client;
            socklen_t clientSize = sizeof(client);
            int chatSocket = accept(sid,(struct sockaddr*)&client,&clientSize);
            checkError(chatSocket,__LINE__);
            pid_t child = fork();
            if (child == 0) {
               dup2(chatSocket,fileno(stdin));
               dup2(chatSocket,fileno(stdout));
               dup2(chatSocket,fileno(stderr));
               close(chatSocket);
               close(sid);
               int status = execlp("sqlite3","sqlite3","foobar.db",NULL);
               checkError(status,__LINE__);
               return -1; /* Report that I died voluntarily */
            } else if (child > 0) {
               close(chatSocket);
               cleanupDeadChildren();
            }
         }
         if (FD_ISSET(srv,&afd)) {
            /* This is the service port to ask the server to die! */
            /* Again, it starts with boilerplate */
            struct sockaddr_in client;
            socklen_t clientSize = sizeof(client);
            int chatSocket = accept(srv,(struct sockaddr*)&client,&clientSize);
            checkError(chatSocket,__LINE__);
            /* That's the code that does it for real... */
            char line[512];
            int rc = read(chatSocket,line,sizeof(line)-1);
            line[rc] = 0;
            close(chatSocket);
            if (strncmp("$die!",line,5) == 0)
               break;
            printf("bad command [%s]\n",line);
         }
      }
   }
   close(srv);
   close(sid);
   cleanupDeadChildren();
   printf("terminated...\n");
   return 0;
}
