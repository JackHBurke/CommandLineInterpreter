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
#include <sys/wait.h>
#include "ttymode.h"
#include "tokenizer.h"
#include "cmiBody.h"
#include "cmiPipe.h"
#include "cmiCbreak.h"

#define MAX_BUFFER 1024

int main (int argc, char** argv) {
	atexit(tty_atexit);
	pid_t cmdPid;  
	printf("$> "); 
	addToPath();

	int endOfInput;
	initializeHistory();
	while (1){           //exit proceedures are done in other functions
		tty_cbreak(STDIN_FILENO);
		endOfInput = 0;
		while (!endOfInput) { //loops until enter key is pressed, or ctrl-d to exit
			if (!translateChar()) {
				endOfInput = 1;
			}
		}
		addToHistory();
		char *line = getLine(); //This gets the current line displayed
		printf("\n");
		int status;             //used to wait for child
		char **oldArgs = getTokens(line);
		char **args = redirectionCheck(oldArgs); //this returns and redirects i/o
		tty_reset(STDIN_FILENO);
		if (pipeCheck(args)) {
			int pipeIndex = getPipeIndex(oldArgs);
			//splits up array of strings into two commands
			char **firstCmd = getFirstCmd(oldArgs, pipeIndex);  
			char **secondCmd = getSecondCmd(oldArgs, pipeIndex);
			pipeChildExec(firstCmd, secondCmd);
		} else if(checkInternal(args[0], args)!= 0) {
			if ((cmdPid = vfork()) < 0) {
				perror("Fork failed");
			} else if (cmdPid == 0) {
				if (execvp(args[0],args) < 0) {
					printf("Exec failed, not a valid command\n");
				}
			} else {
				wait(&status);
			}
		}
		//This is to make sure that stdout and stdin are returned after each call
		freopen("/dev/tty", "w", stdout);
		freopen("/dev/tty", "r", stdin);
		printf("$> ");
	}
	return 0;
}
