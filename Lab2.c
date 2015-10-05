#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node{
  struct node *childPtr, *siblingPtr, *parentPtr;
  char name[64];
  char nodeType;
}Node;

typedef struct fileSystem{
  Node **head;
}FileSystem;

Node *root, *cwd;                             /* root and CWD pointers */
char line[128], cwdPath[64];                               /* user input line */
char command[16], pathname[64];               /* user inputs */
char dirname[64], basename[64];               /* string holders */

//Node *creat();
Node *makeNode(char *name, char type);
int findCommand(char *input);
void processLine();
void initialize();
void menu();
void mkdir();
void rmdir();
void cd();
void ls();
void pwd();
void creat();
void rm();
void save();
void reload();
void erase(char str[]);
Node *findChildDir(Node *list, char *value); // searches sibling list and returns the dir with the matching name
Node *findChildFile(Node *list, char *value); // searches sibling list and returns the file with matching name
void add(Node *list, Node *thisNode); // adds a node to the end of the list
void resetGlobal();
void processPathname();
int traverseTree(Node **pCur, Node *pPrev, Node *pNext, char *path);
void printList(Node *pCur);
void removeFromList(Node *list, char *value);
void printToFile(FILE **fp, Node *pCur);
void printParentPathToFile(FILE **fp, Node *pCur);


void main (int argc, char *argv[], char *env[])
{
  int run = 1;
      initialize();
  while(run)
    {
      resetGlobal();
      menu();
    }
}

void menu()
{
  
  printf("input a command: ");
  while(!fgets(line, 128, stdin)/*scanf("%s", line)*/)
    printf("input cannot exceed 128 characters\n");
  processLine(); // do I need to pass reference??
  switch(findCommand(command)){
  case 0 : mkdir(); break;
  case 1 : rmdir(); break;
  case 2 : cd(); break;
  case 3 : ls(); break;
  case 4 : pwd(); break;
  case 5 : creat(); break;
  case 6 : rm(); break;
  case 7 : save(); break;
  case 8 : reload(); break;
  case 9 : exit(1); break;
  default : printf("command not found!\n");break;
  }
}
/*** WORKING ***/
void mkdir()
{
  Node* pCur, *pNext, *pPrev, *pTemp, *pFile, *pDir;

  pFile = '\0';
  pDir = '\0';

  printf("mkdir() running...\n");

 if(pathname[0] == '/') // absolute
    {
      pCur = root;
    }
  else // relative
    {
      pCur = cwd;
    }
  processPathname();
 
 if(!(traverseTree(&pCur, pPrev, pNext,dirname)))
   return;

 pFile = findChildFile(pCur->childPtr, basename); // detects if file already exists
 pDir = findChildDir(pCur->childPtr, basename); // detects if dir already exists, returns ptr if found
 if(pFile)
   pNext = pFile;
 else if (pDir)
   pNext = pDir;
 if(pNext)
   {
     printf("Basename already exists as a %c under \"%s\" dir!\n", pNext->nodeType,pNext->parentPtr->name);
     return;
     
   }
 pTemp = makeNode(basename, 'D');
 pTemp->parentPtr = pCur;
 printf("Assigning parent \"%s\" child \"%s\"\n",pTemp->parentPtr->name, pTemp->name);
 if(pCur->childPtr)
   add(pCur->childPtr,pTemp);
 else
   pCur->childPtr = pTemp;
 pNext = findChildDir(pCur->childPtr,basename);
 if(pNext)
   printf("\"%s\" successfully added!\n",findChildDir(pCur->childPtr,basename)->name);
 return;
}
/*** WORKING ***/
void rmdir()
{
  Node *pCur, *pPrev, *pNext, *pTemp;
  printf("rmdir() running...\n");
  
  if(pathname[0] == '/')
    pCur = root;
  else
    pCur = cwd;
  processPathname();
  if(traverseTree(&pCur, pPrev, pNext, dirname))
    {
      pTemp = findChildDir(pCur->childPtr, basename);
      if(pTemp)
	{
	  if(pTemp->childPtr)
	  {
	    printf("cannot delete dir, \"%s\" is not empty!\n",pTemp->name);
	  }
	  else
	    removeFromList(pCur->childPtr, pTemp->name);// will remove node from list, wont check for nodetype or children
	}
      else
	{
	  printf("Dir \"%s\" does not exist in \"%s\"!\n", basename, pCur->name);
	}
    }
  else
    {
      printf("Invalid pathname \"%s\"\n", pathname);
    }
  return;
}
/*** WORKING ***/
void cd()
{
  Node *pCur, *pPrev, *pNext;
  char *path, temp[64];

    pCur = cwd;
  if(pathname[0])
    {
      if(pathname[0] == '/')
	pCur = root;
      strcpy(temp,pathname); // need to pass pathname instead of dirname for cd
      path = temp;
    if(traverseTree(&pCur, pPrev, pNext, path))
      {
      cwd = pCur;
      if(pathname[0] == '/')
	strcpy(cwdPath, pathname);
      else
	{
	  if(strcmp(cwdPath,root->name))
	    strcat(cwdPath, "/");
	  strcat(cwdPath,pathname);
	}
      printf("cwd now set to \"%s\"\n",cwdPath);
      }
    else
      printf("pathname \"%s\" does not exist, could not change cwd.\n",pathname);
    }
  else
    {
      cwd = root;
      strcpy(cwdPath,root->name);
      printf("cwd now set to \"%s\"\n",cwd->name);
    }
  return;
}
/*** WORKING ***/
void ls()
{
  Node *pCur, *pPrev, *pNext;
  char *path, temp[64];
  if(pathname[0])
    {
      pCur = root;
      strcpy(temp,pathname);
      path = temp;
      if(traverseTree(&pCur, pPrev, pNext, path))
	printList(pCur->childPtr);
      else
	printf("pathname \"%s\" does not exist, could not list files.\n",pathname);
    }
  else
    {
      pCur = cwd;
      printList(pCur->childPtr);
    }
  return;
}
/*** WORKING ***/
void pwd()
{
  printf("cwd: \"%s\"\n",cwdPath);
  return;
}
/*** WORKING ***/
void creat()
{
  Node* pCur, *pNext, *pPrev, *pTemp;

  printf("creat() running...\n");

 if(pathname[0] == '/') // absolute
    {
      pCur = root;
    }
  else // relative
    {
      pCur = cwd;
    }
 
  processPathname();
 if(!(traverseTree(&pCur, pPrev, pNext,dirname)))
   return;
      pNext = findChildDir(pCur->childPtr,basename);      
      if(pNext)
	{
	  printf("Basename already exists as a %c under \"%s\" dir!\n", pNext->nodeType,pNext->parentPtr->name);
	  return;
	}
      pTemp = makeNode(basename, 'F');
      pTemp->parentPtr = pCur;
      printf("Assigning parent \"%s\" child \"%s\"\n",pTemp->parentPtr->name, pTemp->name);
      if(pCur->childPtr)
	add(pCur->childPtr,pTemp);
      else
	pCur->childPtr = pTemp;
	printf("\"%s\" successfully added!\n",findChildFile(pCur->childPtr,basename)->name);
  return;
}

void rm()
{
  Node *pCur, *pPrev, *pNext, *pTemp;
  printf("rm() running...\n");

  if(pathname[0] == '/')
    pCur = root;
  else
    pCur = cwd;
  processPathname();
  if(traverseTree(&pCur, pPrev, pNext, dirname))
    {
      pTemp = findChildFile(pCur->childPtr,basename);
      if(pTemp)
	removeFromList(pCur->childPtr, pTemp->name); // will remove node form list, and related pointers
      else
	printf("File \"%s\" does not exist in \"%s\"!\n", basename, pCur->name);   
    }
  else
    printf("Invalid pathname \"%s\"\n",pathname);
  return;
}

void save()
{
  FILE *fp = fopen("myfile","w+");
  fprintf(fp, "%c %s\n",'D', "name");
  //recursive function to pre-order traverse tree
  printf("save() running... \n");
  printToFile(&fp,root);
  fclose(fp);
  return;
}

void reload()
{
  FILE *fp = fopen("myfile","r");
  char str[64], c;
  int i = 0;
  c = fgetc(fp);
  while (c != EOF)
    {
      erase(str);
      while(c != '\n')
	{
	  str[i] = c;
	  c = fgetc(fp);
	  i++;
	}
      printf("%s\n", str);
      i = 0;
    }
  return;
}

void initialize()
{
  root = makeNode("/", 'D');
  root->siblingPtr = root;
  root->parentPtr = root;
  cwd = root;
  resetGlobal();
  strcpy(cwdPath,root->name);
  printf("%s siblingPtr -> \"%s\"\n",root->name,root->siblingPtr->name);
  printf("%s parentPtr -> \"%s\"\n",root->name, root->parentPtr->name);
  printf("CWD = \"%s\"\n",cwdPath);
}

void processLine()
{
  int i = 0;
  int j  = 0;
  char *temp;
  while(line[i]) 
    {
      if(line[i] == '\n')
	line[i] = '\0';
      i++;
    }
  i = 0;
  while((line[i] != ' ') && line[i]) // loop to copy command
    {
      i++;
    }
  strncpy(command, line, i); // line[i] is either null or space
  printf("Command =\"%s\"\n", command);
  if(line[i])
    {
    while(line[i])
      {
	pathname[j]= line[i+1];
	i++;
	j++;
      }
    if(pathname[0] != '/')
      {
	strcpy(temp,cwdPath);
	if(cwd != root)
	  strcat(temp,"/");
	strcat(temp,pathname);
	strcpy(pathname,temp);
      }
  printf("Pathname:\"%s\"\n", pathname);
    }
  return;

}
int findCommand(char *input)
{
  char *cmds[64] = {"mkdir", "rmdir", "cd", "ls", "pwd", "creat", "rm", "save", "reload", "quit", 0};
  int i = 0;
  while(cmds[i])
    {
      if(!(strcmp(input,cmds[i])))
	{
	  return i;
	}
	i++;
    }
    return 10;
}

Node *makeNode(char *name, char type)
{
  Node *temp;
  temp  = (Node*) malloc(sizeof(struct node));
  strcpy(temp->name, name);
  temp->nodeType = type;
  temp->childPtr = '\0';
  temp->siblingPtr= '\0';
  temp->parentPtr = '\0';
  if(temp->nodeType == 'D')
    printf("Created dir \"%s\"...\n",temp->name);
  else
    printf("Created file \"%s\"!\n",temp->name);
  return temp;
}

void erase(char str[])
{
  int i = 0;
  while(str[i])
    {
      str[i] = '\0';
      i++;
    }
  str = '\0';
}

Node *findChildDir(Node *list, char *value)
{
  Node *pCur;
  pCur = list;
  while(pCur)
    {
      if(!(strcmp(pCur->name,value)))
	{
	  if(pCur->nodeType == 'F')
	    {
	      printf("Node \"%s\" found, but was of type file\n",pCur->name);
	      return '\0';
	    }
	  return (pCur);
	}
      pCur = pCur -> siblingPtr;
    }
  return '\0';
}

Node *findChildFile(Node *list, char *value)
{
  Node *pCur;
  pCur = list;
  while(pCur)
    {
      if(!strcmp(pCur->name,value))//matching name
	{
	  if(pCur->nodeType == 'D')
	    {
	      printf("Node \"%s\" found, but was of type dir\n",pCur->name);
	      return '\0';
	    }
	  return (pCur);
	}
      pCur = pCur->siblingPtr;
    }
  return '\0';
}
void add(Node *list, Node *thisNode)
{
  Node *pCur;
  pCur = list;
  while (pCur->siblingPtr)
    {
      pCur = pCur -> siblingPtr;
    }
  pCur->siblingPtr = thisNode;
  return;
}
void resetGlobal()
{
  erase(pathname);
  erase(dirname);
  erase(basename);
  erase(line);
  erase(command);
  return;
}
/*****************************************************
takes care of dirname and basename even if absolute
or relative pathname.
*****************************************************/
void processPathname() // ***NEEDS TO RETURN PATH*** //
{
  char tempStr[64];
  int i = 0, j = 0;
  if(!pathname[0])
    {
      printf("command requires a pathname!\n");
      return;
    }
 
  if(pathname[0] != '/')
    {
      printf("pathway is relative to \"%s\"\n",cwd->name);
      strcpy(tempStr,(cwd->name));
      if(strcmp(cwd->name, "/"))  // if cwd is not root, cat '/' onto back of cwd    
	strcat(tempStr,"/");
      strcat(tempStr,pathname);
      erase(pathname);
      strcpy(pathname,tempStr);
    }
  while(pathname[i]) // traverses to end of pathname
      i++;
  j = i;
  while((pathname[j] != '/') && (j>0))
    j--;
  if(j == 0)
    strcpy(dirname,"/");
  else
    strncpy(dirname,pathname,j);
  i = 0; // resets i to 0 to copy basename starting from 'j', which should be after '/'
  j++;
  while(pathname[j])
    {
      basename[i] = pathname[j];
      i++;
      j++;
    }
  basename[i] = '\0';
  printf("pathname=\"%s\"\ndirname=\"%s\"\nbasename=\"%s\"\n",pathname,dirname,basename);
}
/*******************************************************************
 *dirPath - the directory path to traverse
 ******************************************************************/ 
int traverseTree(Node **pCur, Node *pPrev, Node *pNext, char *dirPath)
{
  char *path;
  if(!(dirPath)) // path doesn't exist
    return 0; // fail to traverse tree
  printf("path before: \"%s\"\n",dirPath);
  path = strtok(dirPath, "/");
  printf("pCur: \"%s\"\n",(*pCur)->name);
 
      while(path != '\0') // step through path dirs
	{	  
	  printf("Scanning dir \"%s\" for \"%s\"\n",(*pCur)->name,path);
	  pNext = findChildDir((*pCur)->childPtr,path); // returns node if found, if not: NULL
	  if(pNext && (pNext->nodeType == 'D')) // next dir was found
	    {
	      pPrev = (*pCur);
	      (*pCur) = pNext; // iterate through tree
	      printf("pCur = \"%s\"\n",(*pCur)->name);
	    }
	  else // next dir was not found
	    {
	      printf("Could not find dir \"%s\" in current path!\n",path);
	      return 0;
	    }
	  path = strtok('\0',"/");
	}
      // make dir ****NEED TO CHECK IF NODE EXISTS AS FILE
      return 1;
}

void printList(Node *pCur)
{
   while(pCur)
    {
      printf("(%c) \"%s\"\n",pCur->nodeType, pCur->name);
      pCur = pCur->siblingPtr;
    }
   return;
}


void removeFromList(Node *list, char *value)
{
  Node *pCur, *pPrev;
  pCur = list;
  while(pCur)
    {
      if(!strcmp(pCur->name,value))
	{
	  if(pCur == pCur->parentPtr->childPtr) // if pCur is its parents childPtr, rmv from parent
	    {
	      if(pCur->siblingPtr) // if pCur has sibling, replace parent childPtr with siblingPtr
		pCur->parentPtr->childPtr = pCur->siblingPtr; 
	      else
		pCur->parentPtr->childPtr = '\0'; // no siblings => assign parent childPtr null
	    }
	  pPrev->siblingPtr = pCur->siblingPtr;
	  free(pCur);
	  pCur = '\0';
	  printf("Node \"%s\" removed from \"%s\".\n",value, list->parentPtr->name);
	  return;
	}
      pPrev = pCur;
      pCur = pCur->siblingPtr;
    }
  printf("Node \"%s\" not found in directory \"%s\".\n",value, list->parentPtr->name);
  return;
}

void printToFile(FILE **fp, Node *pCur)
{
  if(pCur != '\0')
    {
      printf("%c %s\n", pCur->nodeType, pCur->name);
      printf("pCur: %8x\n", pCur);
      fprintf((*fp),"%c ",pCur->nodeType);
      printParentPathToFile(fp, pCur);
      fprintf((*fp),"\n");
      printToFile(fp,pCur->childPtr);
      printToFile(fp,pCur->siblingPtr);
    }  
  else
    {
      printf("NULL\n");
    }
  return;
}

void printParentPathToFile(FILE **fp, Node *pCur)
{
  if(strcmp(pCur->name,"/"))
    {
      printParentPathToFile(fp,pCur->parentPtr);
    }
  if(!strcmp(pCur->name,"/"))
    {
      fprintf((*fp),"%s", pCur->name);
    }
  else if (strcmp(pCur->name, "/"))
    {
      fprintf((*fp),"/%s", pCur->name);
    }
  return;
}
