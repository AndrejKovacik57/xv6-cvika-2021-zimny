#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
//check numbers in pipe if they are prime number
void primeChek(int cp[], int np[], int curNum){
  int num;
  close(cp[1]);

  while(read(cp[0], &num,sizeof(num)) > 0){

    if(num % curNum != 0){
      write(np[1], &num, sizeof(num));
    }
  }

  close(np[1]);
}

void nextProcess(int cp[]){
  //np as next pipe cp as current pipe
  int np[2];
  int num;
  int pid;
  close(cp[1]);
  //reading prime number sent trough pipe from parent process

  if (read(cp[0], &num, sizeof(num))){
    printf("prime %d\n", num);

    if(pipe(np) == -1){
      fprintf(2, "An error occurred with opening next pipe!\n");
      exit(-1);
    }

    pid = fork();

    if(pid < 0){
      fprintf(2, "Fork failed");
      exit(-1);
    }

    if (pid == 0){// child
      //we sent next pipe (which will have next prime number sent from parent process) to this recursive function
      nextProcess(np);

    }else{    // parent
      primeChek(cp, np, num);
      wait(0);
    }
  }

}
int main(int argc, char *argv[]){
  //fp as first pipe
  int fp[2];
  int pid;

  if(pipe(fp) == -1){
    fprintf(2, "An error occurred with opening first pipe!\n");
    exit(-1);
  }

  pid = fork();

  if(pid < 0){
    fprintf(2, "Fork failed");
    exit(-1);
  }

  if (pid == 0){   //child
    nextProcess(fp);

  }else{   // parent
    close(fp[0]);

    for (int i = 2; i < 36; i++){
      write(fp[1], &i, sizeof(i));
    }

    close(fp[1]);
    wait(0);
  }

  exit(0);
}