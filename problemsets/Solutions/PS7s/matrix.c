#include "matrix.h"
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>

size_t sizeMatrix(int r,int c) {
   return r * c * sizeof(int) + sizeof(Matrix);
}

Matrix* makeMatrix(int r,int c) {
   Matrix* m = malloc(sizeof(Matrix) + r * c * sizeof(int));
   m->r = r;
   m->c = c;
   return m;
}

Matrix* makeMatrixMap(void* zone,int r,int c) {
   Matrix* m = zone;
   m->r = r;
   m->c = c;
   return m;
}

int readValue(FILE* fd) {
   int v = 0;
   char ch;
   int neg=1;
   while (((ch = getc_unlocked(fd)) != EOF) && isspace(ch)); // skip WS.      
   while (!isspace(ch)) {
      if (ch=='-')
         neg=-1;
      else
         v = v * 10 + ch - '0';
      ch = getc_unlocked(fd);
   }
   return neg * v;
}

Matrix* readMatrix(FILE* fd) {
   int r,c,v;
   int nv = fscanf(fd,"%d %d",&r,&c);
   Matrix* m = makeMatrix(r,c);
   flockfile(fd);
   for(int i=0;i < r;i++)
      for(int j=0;j < c;j++) {
         v = readValue(fd);
         M(m,i,j) = v;
      }
   funlockfile(fd);
   return m;
}

void freeMatrix(Matrix* m) {
   free(m);
}

void printMatrix(Matrix* m) {
   for(int i=0;i<m->r;i++) {
      for(int j=0;j < m->c; j++)
         printf("%3d ",M(m,i,j));     
      printf("\n");
   }
}

Matrix* multMatrix(Matrix* a,Matrix* b,Matrix* into) {  // mxn * nxp yields an mxp matrix
   if (a->c != b->r)
      return NULL;
   into->r = a->r;
   into->c = b->c;
   for(int i=0;i < a->r;i++)
      for(int j=0;j < b->c;j++) {
         int ttl = 0;
         for(int k=0; k < a->c;k++) 
            ttl += M(a,i,k) * M(b,k,j);
         M(into,i,j) = ttl;
      }
   return into;
}

Matrix* parMultMatrix(int nbW,sem_t* sem,Matrix* a,Matrix* b,Matrix* into) {
   if (a->c != b->r)
      return NULL;
   into->r = a->r;
   into->c = b->c;

   int from = 0,slice = a->r / nbW,rem = a->r % nbW;
   for(int w=0;w < nbW;w++) {
      int to = from + slice + (w < rem) - 1;
      pid_t child = fork();
      if (child==0) {
         for(int i=from;i <= to;i++)
            for(int j=0;j < b->c;j++) {
               int ttl = 0;
               for(int k=0; k < a->c;k++) 
                  ttl += M(a,i,k) * M(b,k,j);
               M(into,i,j) = ttl;
            }
         sem_post(sem);
         exit(0);
      }
      from = to + 1;
   }
   return into;
}
