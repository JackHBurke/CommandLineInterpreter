/* Jack Burke
   10/26/15
   lcount.c
 
   This program returns the number of every alphabetical character from a text.
   Based on input, this will read from multiple files, or std.in*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "lcount.h"

#define MAX_BUFFER 1024

int charCounts[26] = {0};         //made global as to access in helper function 

int main (int arc, char **argv) {
	char line[MAX_BUFFER];
	int numberOfArgs = 0;
	while (argv[numberOfArgs] != '\0') {
		numberOfArgs++;
	}
	//If the number of arguments given is less than 2, then we know that
	//no file was provided, and we should read from std.in
	if (numberOfArgs < 2) { //may not relate to how many arguments
		while(fgets(line, MAX_BUFFER, stdin)) {
			addCounts(line);
		}     
	} else {
		int argCounter = 1;
		FILE *fp;
		while (argv[argCounter] != '\0') { //may need it to be different
			fp = fopen(argv[argCounter], "r");
			if (fp == NULL) {
				printf("Could not open file %s\n", argv[argCounter]);
			} else {
				while (fgets(line, MAX_BUFFER, fp)) {
					addCounts(line);  
				}
			}
			argCounter++;
		}
	}
	int i = 0;
	printf("\n");
	for(i = 0; i < 26; i++) {
		if (charCounts[i] > 0) {
			printf("%c = %d ", ('a' + i), charCounts[i]);
		}
	}
	printf("\n");
	return 0;
}

//  This calculates where to add in the array by treating the character as a 
//  number and subtracting the lowest value character from it, in order to make
//  sure that we are getting an index within 0-26

void addCounts(char *line) {
	int i = 0;
	while (line[i] != '\0' && line[i] != EOF) {
		if (isalpha(line[i])) {
			int index = (tolower(line[i])- 'a');
			charCounts[index]++;
		} 
		i++;
	}
}
