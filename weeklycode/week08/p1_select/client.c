/* Implementing a remote shell 
 * Client receives input from the keyboard - commands to be sent to the remote shell
 * Client receives input from the socket - output from the execution of commands on the remote shell
*/

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

void checkError(int status,int line)
{
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[]) 
{
   if (argc < 2) {
      printf("usage is: rup <hostname>\n");
      return 1;
   }
   char* hostname = argv[1];
   int sid = socket(PF_INET,SOCK_STREAM,0);
   
   // Set server address
   struct sockaddr_in srv;
   struct hostent *server = gethostbyname(hostname); //retrieves IP address
                                                     //of the host we want to
                                                     //connect to
   srv.sin_family = AF_INET;
   srv.sin_port   = htons(9025);
   memcpy(&srv.sin_addr.s_addr,server->h_addr,server->h_length);
   int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
   checkError(status,__LINE__);

   // Using the select API to use multiple file descriptors concurrently
   fd_set afd; //set of file descriptors
   char buf;
   int done = 0;
   do {
	FD_ZERO(&afd); //clear the set of file descriptors
	FD_SET(STDIN_FILENO, &afd); // add stdin to the set
	FD_SET(sid, &afd); // add socket to the set
                      //we do the two fd_sets so that
                      // we are listening to both
                      //stdin file descr. and the
                      //socket, sid, which is a file
                      // descriptor as well 

                      //so both file descriptors are currently 
                      //in the set at address &afd


   //select: if there is an input that's ready on either stdin
   //or the socket, it will come out of select and nbReady will be positive
   //... there could be 1 or 2 
	int nbReady = select(sid+1, &afd, NULL, NULL, NULL); //highest value of a file descriptor + 1, file descriptor for read, write, error, timeout to get out of select otherwise it blocks; returns the number of file descriptors that are ready
	if(nbReady > 0) {
		if(FD_ISSET(STDIN_FILENO, &afd)) { //input available to read from the keyboard
			int rc = read(STDIN_FILENO, &buf, 1); //reading 1 byte at a time
			assert(rc>0);
			int wc = write(sid, &buf, 1); //send the input to the remote shell through the socket
			assert(wc > 0);
		} else if(FD_ISSET(sid, &afd)) { //input available to read from the socket
			int rc = read(sid, &buf, 1); //reading 1 byte at a time
			done = rc == 0; //we may receive 0 bytes if remote shell dies
			if(!done) {
				int wc = write(STDOUT_FILENO, &buf, 1); //send the output received from the socket to stdout
				assert(rc>0);
			}
		}

//^^^read and write could still block in this case^^^
//you have to use fcntl to make the socket non-blocking
//if you want to read more than 1 byte at a time... but fcntl
//needs to be called on EACH file descriptor that you
//want to make non-blocking
      
	} else done = 1; //remote shell dies	
   } while(!done);
   return 0;
}
