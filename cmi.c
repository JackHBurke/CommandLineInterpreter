/* Jack Burke 
   10/26/15
   cmi.c

   This main program runs a simulated command line intepreter, which
   calls various bash commands, as well as a few built in ones  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "tokenizer.h"

#define MAX_BUFFER 1024

int main (int argc, char** argv) {
   char line[MAX_BUFFER];
   pid_t cmdPid, startPid;  
   printf("$> "); 
   char *cwd;
   char buff[PATH_MAX + 1];
   cwd = getcwd(buff, PATH_MAX + 1);
   addToPath(cwd);

   //this is called multiple times to make sure
   //all cases where ctrl-d can be pressed are 
   //handled
   if (feof(stdin)) {
      printf("\n"); 
      exit(4);
   }
   
 
   while(fgets(line, MAX_BUFFER, stdin)) {
      if (feof(stdin)) {
         printf("\n");
         exit(2);
      }
      int status1;
      char **oldArgs = getTokens(line);
      char **args = redirectionCheck(oldArgs); //this returns and redirects i/o
      if (checkInternal(args[0], args)!= 0) {
         if ((cmdPid = vfork()) < 0) {
            perror("Fork failed");
         } else if (cmdPid == 0) {
            char *cwd;
            char buff[PATH_MAX + 1];
            cwd = getcwd(buff, PATH_MAX + 1);
            execvp(args[0],args);
         } else {
            wait(&status1);
         }
      }
      //This is to make sure that stdout and stdin are returned after each call
      freopen("/dev/tty", "w", stdout);
      freopen("/dev/tty", "r", stdin);
      printf("$> ");
   }
   printf("\n");
   return 0;
}
