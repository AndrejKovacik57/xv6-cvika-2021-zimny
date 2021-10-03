#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{

  if(argc < 2){
    fprintf(2, "usage: data | xargs command\n");
    exit(1);
  }

  char *args[MAXARG];
  char ch;
  int pid;
  char char_list[512];

//starting from on 1 so we dont save "xargs" string to our array
  for (int i = 1; i < argc; i++){
    args[i - 1] = argv[i];
  }

  int ch_count = 0;
  int ch_index = 0;
  int args_i = argc-1; //argument index position after parameters we entered in array
  int reading;

  while((reading=read(0, &ch, sizeof(char)))){
    if(ch == '\n' || ch == ' '){
      // making token
      char_list[ch_count++] = '\0';
      args[args_i++] = &char_list[ch_index];
      ch_index = ch_count;

      if (ch == '\n'){
        args[args_i++] = '\0';
        pid = fork();

        if (pid < 0) {
          fprintf(2, "fork failed\n");
          exit(0);
        }

        if(pid == 0){//child
          exec(args[0], args);

        }else{//parent
          wait(0);
        }
      }

    }else{
      char_list[ch_count++] = ch;
    }
  }

  if(reading < 0){
    fprintf(2, "read failed\n");
    exit(0);
  }
  exit(0);
}