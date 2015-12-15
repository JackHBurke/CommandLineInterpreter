/* Jack Burke
   10/12/15
   cmiPipe.c

   This file contains functions related to pipes, for the cmi  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>
#include "cmiPipe.h"

#define MAX_BUFFER 1024

/* pipeCheck
 * 
 *    input: array of strings from tokenizer
 *    output: 1 if pipe exists, 0 if not
 */
int pipeCheck(char **args) {
	int i = 0;
	while (*(args + i) != '\0') {
		if (**(args + i) == '|') {
			return 1;
		}
		i++;
	}
	return 0;
}

/* getFirstCmd
 * 
 *    input: array of strings from tokenizer, index of the pipe
 *    output: array of strings before the pipe
 */
char **getFirstCmd(char **oldArgs, int pipeIndex) {
	char **firstCmd;
	int i = 0;
	firstCmd = (char **)calloc(10, sizeof(char *));
	while (i < pipeIndex) {
		firstCmd[i] = (char *)malloc(strlen(oldArgs[i]) * sizeof(char));
		strncpy(firstCmd[i], oldArgs[i], strlen(oldArgs[i]));
		firstCmd[i][strlen(oldArgs[i])] = '\0';
		i++;
	}
	firstCmd[i] = (char *)malloc(sizeof(char));
	firstCmd[i][0] = '\0';
	return firstCmd;
}

/* getSecondCmd
 *
 *    input: array of strings from tokenizer, index of the pipe
 *    output: array of strings after the pipe
 */
char **getSecondCmd(char **oldArgs, int pipeIndex) {   
	char **secondCmd;
	int i = pipeIndex + 1;
	int cmdCounter = 0;
	secondCmd = (char **)calloc(10, sizeof(char *));
	while ((*(oldArgs + i) != '\0')) {
		secondCmd[cmdCounter] = (char *)malloc(strlen(oldArgs[i]) * sizeof(char));
		strncpy(secondCmd[cmdCounter], oldArgs[i], strlen(oldArgs[i]));
		secondCmd[cmdCounter][strlen(oldArgs[i])] = '\0';
		i++;
		cmdCounter++;
	}
	return secondCmd;
}

/* getPipeIndex
 * 
 *    input: array of strings from tokenizer
 *    output: index of pipe
 */
int getPipeIndex(char **oldArgs) {
	int i = 0;
	while ((**(oldArgs + i) != '|') && (**(oldArgs + i) != '\0')) {
		i++;
	}
	return i;
}

/* pipeChildExec
 * 
 *    This function takes in two arrays of strings, one before the pipe, and the other after.
 * It forks, and calls pipeExec to execute the pipe command
 *
 *    input: array of strings before the pipe, and array of strings after the pipe
 */
void pipeChildExec(char **firstCmd, char **secondCmd) {
	int status;
	pid_t cmdPid;
	if ((cmdPid = fork()) < 0) {
		perror("Fork failed");		
	} else if (cmdPid == 0) {	
		pipeExec(firstCmd, secondCmd);
	} else {
		wait(&status);
	}
}

/* pipeExec
 * 
 *    This function takes in two arrays of strings, one before the pipe, the other after.
 * It forks, has the parent exec the second command, the child run the first, and both pipe
 * 
 *    input: array of strings before the pipe, and array of strings after the pipe
 */
void pipeExec(char **firstCmd, char **secondCmd) {
	int fd[2];
	pid_t pid;
	int status;
	
	pipe(fd);
	pid = vfork();

	if (pid > 0)       // PARENT 
	{
		wait(&status);

		close(fd[1]);	//close write end 
		if (fd[0] != STDIN_FILENO) 
		{
			if ((dup2(fd[0], STDIN_FILENO)) != STDIN_FILENO)
			{
				printf("dup2 error to stdin\n");
				close(fd[0]);	
			}
		}
		if (execvp(secondCmd[0], secondCmd) < 0)
			printf("parent error \n"); 
		if (wait(NULL) < 0)
			printf("couldn't wait\n");
	}


	else    // CHILD
	{

		close(fd[0]);
		if (fd[1] != STDOUT_FILENO)
		{
			if ((dup2(fd[1], STDOUT_FILENO)) != STDOUT_FILENO)
			{
				printf("dup2 error to stdout\n");
				close(fd[1]);
				}
		}
		if (execvp(firstCmd[0], firstCmd) < 0)
			printf("child error \n");	
	}
}
