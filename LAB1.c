#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int  BASE=10;

char *table= "0123456789ABCDEF";

int *FP;

//void mymain(int argc, char *argv[], char *env[]);

/*void main(int argc, char *argv[], char *env[])
{
  //prints("This now works");
  //rpu(532); ***works***
  //printu(532); ***works***
  //printu(0); ***works***
  //printd(-354); ***works***
  //printo(63); ***works***
  //printx(64); ***works***
  mymain(argc, argv, env);
  }*/

void mymain(int argc, char *argv[ ], char *env[ ])
{
  int i;

  myprintf("in mymain(): argc=%d\n", argc);

  for (i=0; i < argc; i++)
      myprintf("argv[%d] = %s\n", i, argv[i]);
  
   for (i=0; env[i]; i++)
     myprintf("env[%d] = %s\n", i, env[i]);
  
  myprintf("---------- testing YOUR myprintf() ---------\n");
  myprintf("this is a test\n");
  myprintf("testing a=%d b=%x c=%c s=%s\n", 123, 123, 'a', "testing");
  myprintf("string=%s, a=%d  b=%u  c=%o  d=%x\n",
           "testing string", -1024, 1024, 1024, 1024);
	   myprintf("mymain() return to main() in assembly\n"); 
}

int rpu(unsigned int x)
{

  char c;
  if (x){
    c = table[x%BASE];
    rpu(x/BASE);
    putchar(c);
  }
}

void prints(char *fmt)
{
  while((*fmt))
    {
      putchar((*fmt));
      fmt++;
    }
  putchar('\n');
 }

 int printu(unsigned int x)
 {
   if (x==0)
     putchar('0');
   else
     rpu(x);
   putchar(' ');
 }

 int printd(int x)
 {
   if(x<0)
     {
     putchar('-');
     x=x*(-1);
     }
   rpu(x);
   putchar(' ');
 }

 int printo(unsigned int x)
 {
   BASE=8;
   rpu(x);
   BASE = 10;
 }

 int printx(unsigned int x)
 {
   BASE = 16;
   putchar('0');
   putchar('x');
   rpu(x);
   BASE = 10;
 }

void myprintf(char *fmt, ...)
 {

  // char *cp = fmt;
  int *ip;
  char *cp;

  cp=fmt;

  /*while(cp != '\0') // iterate through fmt string until end of string
    {
      cp++;
    }
  cp++; // step over null character
  */
  asm("movl %ebp, FP"); // assign ip to the next parameter after fmt string

  //  ip = FP + 3; // moves the ip pointer over the fp, pc, and fmt
  ip = FP + 2;
  while(*cp != '\0')
    {
      if(*cp == '%')
	{
	  cp++;
	  ip++;
	  if(*cp == 'c')
	    {
	      putchar(*ip);
	    }
	  else if(*cp == 's')
	    {
	      prints((char *)(*ip));
	    }
	  else if(*cp == 'u')
	    {
	      printu(*ip);
	    }
	  else if(*cp == 'd')
	    {
	      printd(*ip);
	    }
	  else if (*cp == 'o')
	    {
	      printo(*ip);
	    }
	  else if(*cp == 'x')
	      printx(*ip);
	}
      else if(*cp == '\n')
	putchar('\n');
      putchar('\r');
      else
	{
	  putchar(*cp);
	}
      cp++;
    }
}
