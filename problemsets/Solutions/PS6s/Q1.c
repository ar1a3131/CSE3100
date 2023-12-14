#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define READEND 0
#define WRITEEND 1

int main(int argc,char* argv[]) {
   /* Implement "ls /dev | xargs | cut -d' ' -f<a>-<b> 
      where <a> and <b> are from the command line
      Prohibit the use of system (man -S2 system). That 
      would be  abusive. We need to be clear on that. 
   */
   if (argc < 3) {
      printf("usage: Q1 a b with a,b two natural numbers\n");
      exit(1);
   }
   int p1[2],p2[2];
   pipe(p2);
   if( fork() == 0 ) {        
      pipe(p1);
      dup2(p1[WRITEEND],STDOUT_FILENO);
      close(p1[WRITEEND]);
      close(p2[READEND]);
      if (fork() == 0) {
         dup2(p1[READEND],STDIN_FILENO);
         dup2(p2[WRITEEND],STDOUT_FILENO);
         close(p1[READEND]);
         close(p2[WRITEEND]);
         close(p2[READEND]);
         execlp("xargs","xargs",NULL);
         printf("Something wrong: xargs didn't run\n");
      }
      close(p2[WRITEEND]);       
      execlp( "ls", "ls", "/dev", NULL );
      printf("Something wrong: ls didn't run\n");
   }
   dup2(p2[READEND],STDIN_FILENO);
   close(p2[WRITEEND]);
   close(p2[READEND]);
   char lastArg[64];
   snprintf(lastArg, 63,"-f%s-%s",argv[1],argv[2]);
   execlp("cut","cut","-d ",lastArg,NULL);
   printf("Something wrong: cut didn't run\n");
   return 0;
}


