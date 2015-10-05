#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>

void menu(char cmd[], char **env);

void main(int argc, char **argv, char **env)
{
  char cmd[200];
  int i = 0;
  while(1)
    {
      printf("Command: \n");
      fgets(cmd, 200, stdin);
      sprintf(cmd, "%s%s",cmd,"\0");
      while(cmd[i])
      {
	if(cmd[i] == '\n')
	  {
	    cmd[i] = '\0';
	  }
	i++;
      }
      menu(cmd,env);
      cmd[0]=0;
    }
}

void menu(char cmd[], char **env)
{
  char *cmds[200];
  int i = 0;
  char *paths[200];
  char buff[200];
  
  printf("CMD: %s\n", cmd);
  strtok(cmd, " ");
do
  {
    cmds[i] = cmd;
    printf("CMDS: %s\n", cmds[i]);
    i++;
  }while(strtok(NULL, " "));

  i=0;
  while(strncmp("PATH=",env[i], 5))
  {
    i++;
  }
  printf("%s\n", env[i]);
  strtok(env[i], ":");
  i = 0;
  
  while(paths[i] = strtok(NULL, ":"))
  {
    printf("%s\n", paths[i]);
    i++;
  }
  i = 0;
  do  
  {
    printf("Buff: %s\n", buff);
    sprintf(buff, "%s/%s", paths[i], cmd);   
    i++;
  }while(access(buff, F_OK|X_OK) == -1);

  printf("After checking for file exist\n");
  printf("BUFF: %s\n", buff);
  i = 0;
  while(cmds[i])
    {
      printf("%s\n", cmds[i]);
      i++;
    }
  printf("%x %x %x\n", buff, cmds[0], env);
  execve(buff, &cmds[0], env);
}
