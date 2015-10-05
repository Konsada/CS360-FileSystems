#ifndef SH_H
#define SH_H

int mkdir();
int rmdir();
int ls();
int cd();
int pwd();
int creat();
int rm();
int (*fptr[])(char *);
int menu(char *cmd);
int shell(int argc, char *argvp[], char *env[], char *cmd[]);

#endif
