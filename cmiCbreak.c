/* Jack Burke
   cmiCbreak.c

   This file contains all of the functions that deal with processing 
   input from the user through cbreak mode */

#include "cmiCbreak.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024

char line[MAX_BUFFER];
int currentIndex = 0;   //current index of current line
char **history;         //array of strings, containing each element of the history
int historyIndex;       //placement of next history
int sizeOfHistory = 0;  
int historyPtr = 0;     //how many up/down arrows have been pressed
int viewedHistory = 0;  //index for history when using up/down arrows

/* translateChar
 *
 *    This function translates the characters entered into the keyboard from
 * cbreak mode. It also handles up/down arrows and backspace
 * 
 *    output: 0 on enter key, else 1
 */
int translateChar() {
	char c = getchar();
	if ((c == 8) || (c == 127)) {
                if (currentIndex > 0) {
			printf("\b \b");
			currentIndex--;
		}
	} else if (c == 4) {
		if (currentIndex == 0) {
			printf("\n");
			exit(5);
			//return 0;
		} else {
			deleteCurrentLine();
		}
	} else if (c == 13 || c == 10) {
		return 0;
	} else if (c == 27) {
		char c2 = getchar();
		if (c2 == 91) {
			char c3 = getchar();
			if (c3 == 65) {
				//deleteCurrentLine();
				printUpHistory();
			} else if (c3 == 66) {
				//deleteCurrentLine();
				printDownHistory();
			}
		}
        } else {
		printf("%c", c);
		line[currentIndex] = c;
		currentIndex++;	
        }
	return 1;
}

/* getHistoryIndex
 *
 *    output: current stored index of last history string
 */
int getHistoryIndex() {
	return historyIndex;
}

/* deleteCurrentLine
 *
 *    This function clears the current line and current index, with calling clearBuffer
 */
void deleteCurrentLine() {
	clearBuffer();
	while (currentIndex > 0) {
		printf("\b \b");
		currentIndex--;
	}
}

void clearBuffer() {
	int index = currentIndex;
	while (index > 0) {
		line[index - 1] = '\0';
		index--;
	}
}

/* getLine
 *
 *    This function returns the current line, and resets the buffer
 */
char *getLine() {
	char *currentLine = (char *)malloc(currentIndex * sizeof(char));
	strncpy(currentLine, line, currentIndex);
	currentLine[currentIndex] = '\0';
	clearBuffer();
	currentIndex = 0;
	//historyPtr = 0;
	return currentLine;
}

void initializeHistory() {
	history = (char **)malloc(10 * sizeof(char *));
	historyIndex = 0;
}


/* getLine
 *
 *    This function adds the current line to the history, and changes the index for the next
 * inputted history command
 */
void addToHistory() {
	char *newHistoryLine = (char *)malloc((currentIndex) * sizeof(char));
	strncpy(newHistoryLine, line, currentIndex);
	newHistoryLine[currentIndex] = '\0';
	history[historyIndex] = newHistoryLine;	
	if (historyIndex == 10) {
		historyIndex = 0;
	} else {
		historyIndex++;
	}
	if (sizeOfHistory < 10) {
		sizeOfHistory++;
	}
	viewedHistory = historyIndex;
	historyPtr = 0;
}

void addToCurrentLine(char *newLine) {
	strncpy(line, newLine, currentIndex);
}

void printUpHistory() {
	if (!(historyPtr == sizeOfHistory)) {
		deleteCurrentLine();
		if (viewedHistory == 0) {
			viewedHistory = 10;
		} else {
			viewedHistory--;
		}
		printHistory();
		historyPtr++;
	}
}

void printDownHistory() {
	if (historyPtr < 1) {
		deleteCurrentLine();
	} else if (historyPtr < 2) { //This ensures our pointer is set to the front of the history array
		deleteCurrentLine();
		if (viewedHistory == 10) {
			viewedHistory = 0;
		} else {
			viewedHistory++;
		}
		historyPtr--;
	} else {
		deleteCurrentLine();
		if (viewedHistory == 10) {
			viewedHistory = 0;
		} else {
			viewedHistory++;
		}
		printHistory();
		historyPtr--;
	}
}

/* printHistory
 *
 *    This function prints the command at the current index in history, and loads it into the buffer
 */
void printHistory() {	
	currentIndex = strlen(history[viewedHistory]);
	printf("%s", history[viewedHistory]);
	addToCurrentLine(history[viewedHistory]);
}

void resetCurrentIndex() {
	currentIndex = 0;
}
