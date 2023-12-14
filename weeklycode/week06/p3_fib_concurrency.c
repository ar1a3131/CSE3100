/* Using process concurrency */

#include <stdio.h>
#include <unistd.h>
#include <time.h>

double fib(int n) {
    if(n<=1)
        return n;
    else return fib(n-1) + fib(n-2);
}

int main() {
    pid_t value = fork(); 
    int i;
    if(value==0) { /* child process */
        for(i=0; i < 11; i++)
            printf("fib(%2d) = %lf\n", i*2, fib(i*2));
    } else {
        long begin = time(NULL);
        for(i=0;i<10;i++) {
            sleep(1);
            printf("Elapsed time in parent: %ld\n", time(NULL) - begin);
        }
    }
    return 0;
}