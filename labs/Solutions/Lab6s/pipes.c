#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define READ_END 0
#define WRITE_END 1

#define BUFFER_SIZE 2

void write_message(char * message, int fd) {
  char c;
  for(int i=0;i<strlen(message);i++) {
    c = message[i];
    if(write(fd,&c,1)!=1) //write function:
                          //ssize_t write(int filedescriptor,
                          //              const void *buf,
                          //              size_t numbytes)
                          /*Parameters

                          - fd: file descriptor of the file from which data is to be read.
                          - buf: buffer to read data from
                          - cnt: length of the buffer

                      Return Value

                          - return Number of bytes read on success
                          - return 0 on reaching the end of file
                          - return -1 on error
                          return -1 on signal interrupt

                      Important Points

                          - buf needs to point to a valid memory location 
                          with a length not smaller than the specified size 
                          because of overflow.
                          - fd should be a valid file descriptor returned 
                          from open() to perform the read operation because 
                          if fd is NULL then the read should generate an error.
                          - cnt is the requested number of bytes read, while the 
                          return value is the actual number of bytes read. Also, 
                          some times read system call should read fewer bytes than cnt.
*/
      exit(-1);
  }
}

char* read_message(int fd) {
  char c;
  int count = 0;
  char* buffer = calloc(BUFFER_SIZE, sizeof(char)); //calloc(# of blocks, bytes per block)
  while(read(fd, &c , 1) > 0){  //read function:
                                //ssize_t read(int filedescrip.,
                                //             void *buf,
                                //             size_t numbyte)
    if(count >= BUFFER_SIZE) 
        buffer = realloc(buffer, sizeof(char)*(count + 1)); /*realloc deallocates the old object
                                                            pointed to by ptr and returns a pointer 
                                                            to a new object that has the size specified by size. 
                                                            The contents of the new object is identical to that of 
                                                            the old object prior to deallocation, up to the lesser 
                                                            of the new and old sizes. Any bytes in the new object 
                                                            beyond the size of the old object have 
                                                            indeterminate values. 
                                                            only used for synamic mem. allocation*/
    buffer[count] = c;
    count++;
    if(c == '\n'){
      if(count >= BUFFER_SIZE) 
        buffer = realloc(buffer, sizeof(char)*(count + 1));
      buffer[count] = '\0';
      return buffer;
    }
  }
}

void encrypt(char * message) {
  for(int i=0;i<strlen(message);i++) {
    if(message[i] == '\n') break;
    message[i] += 1;
  }
}

void decrypt(char * message) {
  for(int i=0;i<strlen(message);i++) {
    if(message[i] == '\n') break;
    message[i] -= 1;
  }
}

int main() {
  // Set up pipes
  int pTOc1[2], c1TOc2[2], c2TOp[2]; //3 sets of arrays of length 2 (2 file descriptors)
  if (pipe(pTOc1) == -1) exit(-1);
  if (pipe(c1TOc2) == -1) exit(-1);
// you can put an if (pipe(c2TOp) == -1) exit(-1); if you want

  pid_t pid1 = fork();
  if (pid1 == 0) {
    // Child 1
    char* msg;
    close(pTOc1[WRITE_END]);
    close(c1TOc2[READ_END]);
    msg = read_message(pTOc1[READ_END]);
    printf("Child 1 is Encrypting!\n");
    encrypt(msg);
    write_message(msg,c1TOc2[WRITE_END]);
    free(msg);
    close(pTOc1[READ_END]);
    close(c1TOc2[WRITE_END]);
    exit(0);
  } else {
    if (pipe(c2TOp) == -1) exit(-1);
    pid_t pid2 = fork();
    if (pid2 ==0) {
      // Child 2
      char* msg;
      //second child doesn't need access to the first pipe
      //so you close the first pipe
      close(pTOc1[WRITE_END]);
      close(pTOc1[READ_END]);
      close(c1TOc2[WRITE_END]);
      close(c2TOp[READ_END]);
      msg = read_message(c1TOc2[READ_END]);
      printf("Child 2: %s", msg);
      write_message(msg, c2TOp[WRITE_END]);
      free(msg);
      close(c1TOc2[READ_END]);
      close(c2TOp[WRITE_END]);
      exit(0);
    } else {
      // Parent
      close(pTOc1[READ_END]);
      close(c1TOc2[WRITE_END]);
      close(c1TOc2[READ_END]);
      close(c2TOp[WRITE_END]);
      char* msg = "This is a test.\n";
      printf("Parent: %s", msg);
      write_message(msg,pTOc1[WRITE_END]);
      msg = read_message(c2TOp[READ_END]);
      decrypt(msg);
      printf("Parent: %s", msg);
      free(msg);
      close(pTOc1[WRITE_END]);
      close(c2TOp[READ_END]);
      return 0;
    }
  }
}
