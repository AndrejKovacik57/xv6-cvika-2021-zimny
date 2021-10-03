#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void
find(char *startDir, char *finalDest)
{


  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(startDir, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", startDir);
    return;
  }


  if(fstat(fd, &st) < 0){
    //with fd of open file obtains information about that file and saves it to structure pointed to by st
    fprintf(2, "find: cannot stat %s\n", startDir);
    close(fd);
    return;
  }

  switch(st.type){

    case T_FILE:
      fprintf(2, "find: %s is not startDirectory\n", startDir);
      //if in startDir parameter was not entered folder name then we end program
      break;

    case T_DIR:
      if(strlen(startDir) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2, "find: path too long\n");
        break;
      }
      strcpy(buf, startDir);
      p = buf + strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){

        if(de.inum == 0)
          // folder with inum 0 is not valid folder
          continue;

        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = '\0';

        if(stat(buf, &st) < 0){
          printf("find: cannot stat %s\n", buf);
          continue;
        }

        //recursion if folder
        if(st.type == T_DIR){
          find(buf, finalDest);
        }
        if (st.type == T_FILE && strcmp(de.name, finalDest)==0)
        {
          printf("%s\n", buf);
        }


      }
      break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc != 3){
    fprintf(2,"Wrong arguments!\nUse: find(start_directory, final_destination_name)\n");
    exit(-1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
