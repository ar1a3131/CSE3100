#include <stdio.h>

char lockerState(int l, int t)
{
  if(t == 1){
    return 1;
  }
  int state = lockerState(l, t - 1);
  if(l % t == 0){
    return !state;
  } else{
    return state;
  }
}



int main(int argc, char* argv[])
{
  int locker;
  while(1){
    printf("Enter locker number: ");
    scanf("%d", &locker);
    if(locker < 0){
      break;
    }else if(lockerState(locker, locker) == 0){
      printf("Closed\n");
    }else{
      printf("Open\n");
    }
  }
  return 0;
}
