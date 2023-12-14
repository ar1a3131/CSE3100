/* Diverging further - loading an executable in the child process 
 * This is done using a system call - execl()
     * First, the child 'guts himself' - discards the code in its address space
    * Second, reloads the entire address space with another executable
    * Args: path to the executable, arguments to be passed to the new executable, NULL pointer to end the list
    * If execl is successful, it DOES NOT return! Control is instead passed to the main of new executable
    * Note: the opened files are not affected by the operation
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    pid_t value = fork();
    if(value==0) {
        printf("In child!\n");
        execl("./p5_childExec", "./p5_childExec", "10", "20", "30", NULL);
        printf("Oops.... something went really wrong. Shouldn't be here!\n");
        char* msg = strerror(errno);
        printf("Error was: %s\n", msg);
        return -1;
    } else {
        printf("In parent!\n");
        execl("./p5_childExec", "./p5_childExec", "100", "200", "300", NULL);
        printf("Oops.... something went really wrong. Shouldn't be here!\n");
        char* msg = strerror(errno);
        printf("Error was: %s\n", msg);
        return -1;
    }
}
