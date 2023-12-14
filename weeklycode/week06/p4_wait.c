/* Reaping - collecting dead children 
 * What happens when the child dies first?
    * Parent reaps the child process - recover resources that were used by the child
 * What happens when the parent dies first?
    * Child becomes orphaned and gets adopted by an ancestor - typically init!
    * A descendant of init can also become a reaper - prctl(PR_SET_CHILD_SUBREAPER, 1)
    * All the children die in solidarity if the parent dies - parent process 'kill's all processes before dying!
*/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

double fib(int n) {
    if(n<=1)
        return n;
    else return fib(n-1) + fib(n-2);
}

int main() {
    pid_t value = fork(); 
    int i;
    if(value==0) { /* child process */
        for(i=0; i < 16; i++)
            printf("fib(%2d) = %lf\n", i*3, fib(i*3));
    } else {
        long begin = time(NULL);
        for(i=0;i<10;i++) {
            sleep(1);
            printf("Elapsed time in parent: %ld\n", time(NULL) - begin);
        }
        int exitStatus;
        pid_t deadChild = wait(&exitStatus);
        printf("Child %d died\n", deadChild);
        printf("Exited normally? [%s] with status %d\n", 
                WIFEXITED(exitStatus) ? "yes" : "no",
                WEXITSTATUS(exitStatus));
    }
    return 0;
}