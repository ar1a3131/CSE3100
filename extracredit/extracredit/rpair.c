#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SHARED_MEM_NAME "/memzone1"

/* Updated createSamples function */
long createSamples(unsigned wID, long nbSamples, double d, long *ptr) {
   long ttl = 0; // Number of events detected by a process
   unsigned seed = wID; // Seed for random number generation

   double x, y;
   for (int k = 0; k < nbSamples; k++) {
      x = rand_r(&seed) / (double)RAND_MAX;
      y = rand_r(&seed) / (double)RAND_MAX;

      if (fabs(x - y) < d) {
         ttl++;
      }
   }

   /* Update the shared memory with the results of this process */
   ptr[wID] = ttl;

   return ttl;
}

int main(int argc, char *argv[]) {
   if (argc < 4) {
      printf("usage: rpair <d> <samples> <workers>\n");
      exit(1);
   }
   float d = atof(argv[1]); // value of d
   long s = atol(argv[2]);  // total number of trials to be executed
   int nbW = atoi(argv[3]); // number of worker processes

   long ttl = 0; // total number of events
   double p = 0; // Probability of the occurrence of events

   long *ptr;

   /* Create shared memory */
   const char *zone = SHARED_MEM_NAME;
   int md = shm_open(zone, O_RDWR | O_CREAT, S_IRWXU);
   int x = ftruncate(md, (nbW + 1) * sizeof(long)); // +1 for total counter
   ptr = mmap(NULL, (nbW + 1) * sizeof(long), PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
   memset(ptr, 0, (nbW + 1) * sizeof(long));

   long work_per_process = s / nbW;

   for (int i = 0; i < nbW; i++) {
      int pid = fork();
      if (pid == 0) {
         createSamples(i, work_per_process, d, ptr);
         exit(0);
      }
   }

   for (int i = 0; i < nbW; i++) {
      wait(NULL);
   }

   /* Aggregate the results from all worker processes */
   for (int i = 0; i < nbW; i++) {
      ttl += ptr[i];
   }

   /* Update the total counter in shared memory */
   ptr[nbW] = ttl;

   p = (double)ttl / s;
   printf("Total trials = %ld \t Total events = %ld \t Probability = %.6lf\n", s, ttl, p);

   /* Cleanup: Unmap shared memory and close file descriptor */
   munmap(ptr, (nbW + 1) * sizeof(long));
   close(md);
   shm_unlink(zone);

   return 0;
}