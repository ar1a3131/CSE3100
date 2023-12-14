/* Implementing a remote shell */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/wait.h>

void checkError(int status)
{
   if (status < 0) {
      printf("socket error(%d): [%s]\n",getpid(),strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[])
{
   // Create a socket
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(9025);
   addr.sin_addr.s_addr = INADDR_ANY;
   int status = bind(sid,(struct sockaddr*)&addr,sizeof(addr));
   checkError(status);
   status = listen(sid,10);
   checkError(status);

   while(1) {
      struct sockaddr_in client;
      socklen_t clientSize = sizeof(client);
      int rshSocket = accept(sid,(struct sockaddr*)&client,&clientSize);
      checkError(rshSocket);
      printf("We accepted a socket: %d\n",rshSocket);
      
      // Create a child process to execute the remote shell
      pid_t child = fork();
      if (child == 0) { // child process - reads and writes to the socket
         //the purpose of these dup2 lines
         //is redirection... we redirect the socket
         //so that it becomes both the stdin and
         //the stdout of our sub-process we forked
         dup2(rshSocket,0); // close 0 and dup rshsocket
         dup2(rshSocket,1); // close 1 and dup rshsocket
         dup2(rshSocket,2); // close 2 and dup rshsocket
                            //this means that stdin, stdout,
                            //and stderr have been redirected to the socket
         close(rshSocket);   //no longer need rshsocket bc you've done dup,
                             //rshsocket is now in slots 0, 1, and 2
         int status = execlp("/bin/bash","/bin/bash","-l", NULL); // create a login shell (-l)...
                                                                  //child process will execute this executable
                                                                  // and that executable will read its input
                                                                  // from the socket and send its output to the
                                                                  //socket 
         checkError(status);
         return -1;         
      } else if (child > 0) { // parent process
         close(rshSocket);
         pid_t deadChild;
         int status = 0;
         do {
            deadChild = waitpid(0,&status,WNOHANG);
            checkError(deadChild);
            if (deadChild > 0)
               printf("Reaped: %d\n",deadChild);
         } while (deadChild > 0);
      }
   }
   return 0;
}
