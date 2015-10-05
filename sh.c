#include "lab3.h"
int mkdir();
int rmdir();
int ls();
int cd();
int pwd();
int creat();
int rm();
int (*fptr[])(char *);
int menu(char *cmd);
int shell(int argc, char *argv[], char *env[], char *cmd[]);


int (*fptr[])(char *) = {(int (*)())mkdir,rmdir,ls,cd,pwd,creat,rm};

int shell(int argc, char *argv[], char *env[], char *cmd[])
{
  int r;
  printf("menu option %d selected\n", menu(cmd[0]));
  r = fptr[menu(cmd[0])](argv[0]);
}

int menu(char *cmd)
{
  int i;
  char *options[8] = {"mkdir","rmdir","ls","cd","pwd","creat","rm",0};
  for(i=0;options[i];i++)
    {
      if(!strcmp(cmd,options[i]))
	 return i;
    }
      return 0;
}

  int mkdir()
  {
    return 0;
  }

  int rmdir()
  {
    return 0;
  }

  int ls()
  {
    return 0;
  }

  int cd(char *cmds[])
  {
    printf("cd() running...\n");
    int i = 0;
    for(i=0;*cmds[i];i++)
      {

      }
    return 0;
  }

  int pwd()
  {
    return 0;
  }

  int creat()
  {
    return 0;
  }

  int rm()
  {
    return 0;
  }
