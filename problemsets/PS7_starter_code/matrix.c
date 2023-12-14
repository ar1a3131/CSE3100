#include "matrix.h"
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>


size_t sizeMatrix(int r,int c)
{
   /* TODO:
    * Return the number of bytes needed to hold a matrix of r x c values (type int)
    */
    int size = sizeof(Matrix) + r * c * sizeof(int);
    return size;
    
}

Matrix* makeMatrix(int r,int c)
{
   /* TODO:
    * Allocate space to hold a matrix of r x c values (type int)
    * Initialize the number of rows to r and number of columns to c
    */
    Matrix* matrix = malloc(sizeMatrix(r, c));
    if (matrix == NULL) {
        return NULL;
    }

    matrix->r = r;
    matrix->c = c;
    return matrix;
}

Matrix* makeMatrixMap(void* zone,int r,int c)
{
   /* TODO:
    * Store a matrix of size r x c at the address 'zone'. 
    * Note: you do not need to use memory allocation here. Only point a matrix of size r and c to the memory zone.
    */
    Matrix* matrix = (Matrix*)zone;
    matrix->r = r;
    matrix->c = c;
    return matrix;
}

int readValue(FILE* fd)
{
   /* 
    * Reads a single integer from the FILE fd
    * Note: the function uses getc_unlocked to _not_ pay the overhead of locking and
    * unlocking the file for each integer to be read (imagine reading a matrix of 1000x1000,
    * that's 1,000,000 calls to getc and therefore 1,000,000 calls to locking the file. 
    */
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

Matrix* readMatrix(FILE* fd)
{
   /*
    * Reads a matrix from a file (fd). The matrix is held in a text format that first conveys
    * the number of rows and columns, then, each row is on a line of text and all the 
    * values are separated by white spaces. 
    * Namely:
    * r c
    * v0,0 v0,1 .... v0,c-1
    * v1,0 v1,1 .... v1,c-1
    * ....
    * vr-1,0 ....    v_r-1,c-1
    */
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


void freeMatrix(Matrix* m)
{
   /* TODO:
    * Deallocate the space used by matrix m
    */
   if (m != NULL) {
      free(m->data);
      free(m);
    }
}

void printMatrix(Matrix* m)
{
   /* TODO:
    * Print the matrix on the standard output. 
    * Print one row per line, values for the row separated by white spaces. 
    */
    for (int i = 0; i < m->r; i++)
    {
        for (int j = 0; j < m->c; j++)
        {
            printf("%3d ", M(m, i, j));  // Adjusted format specifier to include padding
        }
        printf("\n");
    }
}

Matrix* multMatrix(Matrix* a,Matrix* b,Matrix* into)
{  
   /* TODO:
    * Compute the product of A * B and store the result in `into`.
    * * The computation here is sequential (done by 1 process) and is only meant to be used as a check for your parallel code. 
    * A matrix multiplication of size m x n * n x p yields an m x p matrix.
    *
    * Return the matrix `into` that stores the result.
    */
   into->r = a->r;
   into->c = b->c;

   for (int i = 0; i < into->r; i++) {
      for (int j = 0; j < into->c; j++) {
         M(into, i, j) = 0;
         for (int k = 0; k < a->c; k++) {
               M(into, i, j) += M(a, i, k) * M(b, k, j); //***********
         }
      }
   }

   return into;
}



Matrix* parMultMatrix(int nbW,sem_t* sem,Matrix* a,Matrix* b,Matrix* into)
{
   /* TODO:
    * Compute the product of A * B and store the result in `into`
    * The computation is done in parallel with nbW worker processes that you fork from here.
    * It should have the same output as the sequential version, but faster. ;-)
    * How you use the semaphore is up to you (as long as you use it!)
    * You CANNOT use the wait system call. 
    *
    * Return the matrix `into` that stores the result.
    */
   into->r = a->r;
   into->c = b->c;

   pid_t pid;

   // Create a semaphore for printing
   sem_t* print_sem = sem_open("/printSemaphore", O_CREAT, 0666, 0);

   // Create pipes for inter-process communication
   int** pipes = malloc(sizeof(int*) * nbW);
   for (int i = 0; i < nbW; i++)
   {
      pipes[i] = malloc(sizeof(int) * 2);
      pipe(pipes[i]);
   }

   // Fork processes for parallel computation
   for (int i = 0; i < nbW; i++)
   {
      pid = fork();
      if (pid == 0)
      {
         // Child process
         close(pipes[i][0]); // Close unused read end of the pipe

         for (int row = i; row < a->r; row += nbW)
         {
               for (int col = 0; col < b->c; col++)
               {
                  int result = 0;
                  for (int k = 0; k < a->c; k++)
                  {
                     result += M(a, row, k) * M(b, k, col);
                  }
                  write(pipes[i][1], &result, sizeof(int));
               }
         }

         sem_post(sem);
         exit(0);
      }
      else
      {
         // Parent process
         close(pipes[i][1]); // Close unused write end of the pipe
      }
   }

   // Read the results from the pipes and update the shared memory
   for (int i = 0; i < nbW; i++)
   {
      for (int row = i; row < a->r; row += nbW)
      {
         for (int col = 0; col < b->c; col++)
         {
               int result;
               read(pipes[i][0], &result, sizeof(int));
               M(into, row, col) = result;
         }
      }
   }

   // Clean up pipes
   for (int i = 0; i < nbW; i++)
   {
      close(pipes[i][0]);
      close(pipes[i][1]);
      free(pipes[i]);
   }
   free(pipes);

   sem_post(print_sem); // Signal that printing can occur

   return into;
}
