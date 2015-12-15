/* Jack Burke
   10/26/15
   cmiBody.c

   This program supplies the supplimentary and built in functions that are
   to be used in the command line interpretter cmi.c   */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "cmiBody.h"

/*   This is essentially a switch statement, put in in order to determine if an
 * internal function has been called, and to execute if so. */

int checkInternal(char *cmd, char **args) {
	if (strcmp(cmd, "exit") == 0) {
		printf("\n");
		exit(2);
	}
	if (strcmp(cmd, "cd") == 0 ) {
		cd(cmd,args);
		return 0;
	}
	if (strcmp(cmd, "pwd") == 0) {
		pwd(cmd);
		return 0;
	}
	return 1;
}

int cd (char *cmd, char **args) {
	if (chdir(args[1])== 0) {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		printf("cwd changed to%s\n", cwd);
	} else {
		printf("No such directory: %s\n", args[1]);
	}
	return 0;
}

void pwd(char *cmd){
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s ",cwd);
}

/*  This program does two things (I know thats bad), it reconstructs my 
 * tokens previously gathered by tokenizer.c to remove i/o tokens, and 
 * also redirects io.    */

char **redirectionCheck(char **oldArgs) {
	char **newArgs;
	int i = 0;
	int k = 0;
	newArgs = (char **)calloc(18, sizeof(char *));
	//while ((*(oldArgs + i) != '\0') && (*(oldArgs + i) != "")) {
	while (*(oldArgs + i) != '\0') {
		if (**(oldArgs + i) == '>') {
			if (*(oldArgs + i) == '\0') {
				return newArgs;                       //this is to prevent faulty input
			} else if (*(*(oldArgs + i) + 1) == '\0') {
				freopen(oldArgs[i + 1], "w+", stdout);
				i++;
			} else { 
				freopen(&oldArgs[i][1], "w+", stdout);
			}
			i++;
		} else if (**(oldArgs + i) == '<') {
			if (*(oldArgs + i) == '\0') {
				return newArgs;
			} else if (*(*(oldArgs + i) + 1) == '\0') {
				freopen(oldArgs[i + 1], "r", stdin);
				i++;
			} else {
				freopen(&oldArgs[i][1], "r", stdin);
			}
			i++;
		} else {
			newArgs[k] = (char *)malloc(strlen(oldArgs[i]) * sizeof(char));
			strncpy(newArgs[k], oldArgs[i], strlen(oldArgs[i]));
			newArgs[k][strlen(oldArgs[i])] = '\0';
			i++;
			k++;
		}
	}
	return newArgs;
}

void addToPath () {
	char *cwd;
	char buff[PATH_MAX + 1];
	cwd = getcwd(buff,PATH_MAX + 1);
	char *path = getenv("PATH");
	strcat(path, ":");
	strcat(path, cwd);
	setenv("PATH", path, 1);
}

