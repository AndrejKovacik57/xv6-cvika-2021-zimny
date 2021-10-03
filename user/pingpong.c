#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int to_parent[2];
  int to_child[2];

  if(pipe(to_parent) == -1){
    fprintf(2, "An error occurred with opening pipe to_parent!\n");
    exit(-1);
  }

  if(pipe(to_child) == -1){
    fprintf(2, "An error occurred with opening pipe to_child!\n");
    exit(-1);
  }
  //to_parent[0] read
  //to_parent[1] write
  //same with to_child

  int pid = fork();

  if (pid < 0) {
    fprintf(2, "An error occurred with fork!\n");
    exit(-1);

  }

  if(pid == 0) {
    //child
    char received;
    if(read(to_child[0], &received, 1) == -1){
      fprintf(2, "An error occurred with reading to child pipe!\n");
      exit(-1);
    }

    printf("%d: received ping\n",getpid());
    if(write(to_parent[1], "x", 1) == -1){
      fprintf(2,"An error occurred with writing to parent pipe!\n");
      exit(-1);
    }

  }else{
    //parent
    if(write(to_child[1], "b", 1) == -1){
      fprintf(2, "An error occurred with writing to child pipe!\n");
      exit(-1);
    }

    char received;

    if(read(to_parent[0], &received, 1) == -1){
      fprintf(2, "An error occurred with reading to parent pipe!\n");
      exit(-1);
    }

    printf("%d: received pong\n", getpid());
  }

  exit(0);
}

