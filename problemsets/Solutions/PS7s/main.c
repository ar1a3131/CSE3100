#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "matrix.h"

Matrix* loadMatrix(char* fName)
{
   FILE* fd = fopen(fName,"r");
   if (fd==NULL) return NULL;
   Matrix* m = readMatrix(fd);
   fclose(fd);
   return m;
}

int main(int argc,char* argv[])
{
   if (argc < 4) {
      printf("usage: mult <file1> <file2> <#workers>\n");
      exit(1);
   }

   /* load the two matrices and obtain the number of worker processes */
   Matrix* a = loadMatrix(argv[1]);
   Matrix* b = loadMatrix(argv[2]);
   if (a==NULL || b==NULL) {
      printf("Wrong filenames. Couldn't load matrices [%s,%s]\n",argv[1],argv[2]);
      exit(2);
   }
   int nbW = atoi(argv[3]);

   /* Setting up a shared memory object */
   const char* zone = "/matzone1";
   // create a map big enough to store the matrix - the size must be a multiple of page size.
   size_t szResult = sizeMatrix(a->r,b->c);
   szResult = 1 + (szResult / 4096); // How many pages we would need (in pages)
   size_t mapSz = szResult * 4096;
   int md = shm_open(zone,O_RDWR|O_CREAT,S_IRWXU);
   int status = ftruncate(md,mapSz);
   // create the mapping in memory
   void* ptr = mmap(NULL,mapSz,
                    PROT_READ|PROT_WRITE,
                    MAP_SHARED,
                    md,0); 
   if (ptr == MAP_FAILED) {
      printf("mmap failure: %s\n",strerror(errno));
      exit(1);
   }
   memset(ptr,0,mapSz);     /* initialize the map with 0 values */

   /* Creating a semaphore */
   const char* sname = "/semaMatrix";
   sem_t* s = sem_open(sname,O_CREAT,0666,0);

   /* Map the matrix to memory */
   Matrix* c = makeMatrixMap(ptr,a->r,b->c);

   /* Multiply the matrices using multiple workers*/
   parMultMatrix(nbW,s,a,b,c); /* you can verify your answer by printing the result using multMatrix function, which multiplies the matrices sequentially */

   /* wait for all the sub-processes to complete their task */
   for(int k=0;k < nbW;k++)
      sem_wait(s);

   /* Print the result matrix */   
   printMatrix(c);
   
   /* Destroy the shared object and semaphore */
   munmap(ptr,mapSz);  
   close(md);
   shm_unlink(zone);
   sem_close(s);
   sem_unlink(sname);
   
   return 0;
}
