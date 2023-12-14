/* Setting pipes to communicate between 2 processes 
 * Redirections and pipes in Unix
    * Redirections: input <
    * Redirections: output >
      * E.g., sort is a function that reads strings as input from stdin till EOF and prints them in sorted order.
      * E.g., sort < file.txt > sorted.txt
    * Redirections: error 2>

    * Pipes: | (chain the outputs of various programs)
      * E.g., echo is a function that takes input from stdin and prints it on stdout 
         * echo "Hello world"
      * We could chain the output of echo with another command
         * echo "Hello world" | sha1sum
         * echo "Hello world" | md5sum
*/

/* Implementing redirections and pipes in C 
 * Recall Buffered IO APIs - fopen, fclose, fread, fgetc, fscanf, fprintf,... - all use FILE* abstraction
 * Recall Unbuffered IO APIs - open, close, read, write - Raw IO that use file descriptors
 * Additional Unbuffered IO APIs  to duplicate file handles
    * int dup(int fildes)
    * int dup2(int fildes, int fildes2)
 
 * Redirecting stdin
  * open(3) - opens a file pointed to by entry 3
  * close(0) - resets entry 0 to undefined
  * dup(3) - first unused entry of the FDT will get set by 3 i.e. 0 (stdin) is set to point to file pointed by 3
  * close(3)
*/

/* Some important questions
 * Who should call dup? The parent? The child? answer: the child
 * When should dup be called? before execl ? after execl ? answer: before execl
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

char* md5sum(char* payload,size_t sz);

char* readFile(char* fName,long* sz) {
   FILE* src = fopen(fName,"r");
   fseek(src,0,SEEK_END);
   *sz = ftell(src);
   fseek(src,0,SEEK_SET);
   char* payload = malloc(sizeof(char)* *sz);
   fread(payload,sizeof(char),*sz,src);
   fclose(src);
   return payload;
}

int main(int argc,char* argv[])
{
   long sz = 0;
   char* payload = readFile(argv[1],&sz);
   char* rv = md5sum(payload,sz);
   printf("Got md5 [%s]\n",rv);
   free(rv);
   return 0;
}

void sendPayload(int fd,char* payload,size_t sz) {
   int sent=0,rem = sz;
   while(sent != sz) {
      int nbSent = write(fd,payload+sent,rem);
      sent += nbSent;
      rem  -= nbSent;
   }
}

char* readResponse(int fd) {
   int sz = 8;
   char* buf = malloc(sz);
   int ttl = 0,at = 0;
   int recvd;
   do {
      recvd = read(fd,buf+at,sz - at);
      ttl += recvd;
      at  += recvd;
      if (recvd > 0 && ttl == sz) {
         buf = realloc(buf,sz*2);
         sz *= 2;
      }
   } while (recvd > 0);
   char* final = malloc(ttl+1);
   memcpy(final,buf,ttl);
   final[ttl] = 0;
   free(buf);
   return final;
}

char* md5sum(char* payload,size_t sz)
{
   int pc[2],cp[2];
   pipe(pc);
   pipe(cp);
   pid_t child = fork();
   switch(child) {
      case 0: {         // I'm the child!
         close(STDIN_FILENO);  // close my own stdin
         dup(pc[READ_END]);    // dup read end of parent->child into stdin
         close(pc[READ_END]);  // duped in my stdin
         close(pc[WRITE_END]); // do not need write end of parent->child
         close(STDOUT_FILENO); // close my own stdout
         dup(cp[WRITE_END]);   // dup write end of child->parent into stdout
         close(cp[WRITE_END]); // duped into my stdout, so close
         close(cp[READ_END]);  // do not need the read end of child->parent
         execlp("/usr/bin/md5sum","/usr/bin/md5sum",NULL);
         return NULL;
      }break;
      case -1: {
         printf("Something went really wrong here...\n");
         exit(1);
         return NULL;
      }break;
      default: {        // we are the parent.
         close(pc[READ_END]);  // close the read end of parent->child
         close(cp[WRITE_END]);  // close the write end of child->parent.
         sendPayload(pc[WRITE_END],payload,sz);
         close(pc[WRITE_END]);  // we are done sending. Close the file handle.
         char* rv = readResponse(cp[READ_END]);
         close(cp[READ_END]);  // we are done reading the response. close the file handle
         int exitStatus;
         pid_t deadChild = wait(&exitStatus);
         printf("Dead child %d\n",deadChild);
         return rv;
      }break;
   }
}