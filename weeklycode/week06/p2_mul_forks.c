/* Multiple calls to fork() */

/*I added the if statements in btwn the forks so i can label*/

#include <stdio.h> 
#include <unistd.h>

int main() {
     pid_t value;
     int label = 0;
     value = fork();
     if (value == 0){
          label = 1;
     }
     value = fork();
     if (value == 0){
          label = 2;
     }
     value = fork();
     if (value == 0){
          label = 3;
     }
     printf("In main of pid %d, the value = %d... %d\n", getpid(), value, label);

     return 0;
}