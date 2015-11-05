/*   Jack Burke
     10/26/15
     tokenizer.c

     This program contains the solo function for the tokenizer of cmi.c
     It takes in a string, obtained by fgets in cmi.c, and returns a null 
     terminated array of null terminated strings, based on input.    */

#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

char **getTokens(char *line) {
   int i = 0;
   char **tokens;
   //Read through once to determine how many strings to malloc for 
   while(line[i] != EOF && line[i] != '\n' && line[i] != '\0') {
      int start = 0;
      int wordCount = 0;
      int status1;         //used to wait on the child
      if (line[i] == ' ') {
         if (i == 0) {
            i++;
         } else if (line[i-1] == ' ') {
            i++;
         } else {
            wordCount++;
         }
      } else {
         i++;
      }

      if (i != 0) {
         if (line[i-1] != ' ') {
            wordCount++;
         }
      }
      tokens = (char **)malloc((wordCount + 1) * sizeof(char*));
      i = 0;
      int currentWord = 0;
      while (line[i] != EOF && line[i] != '\0' && line[i] != '\n') {
         while (line[i] == ' ') {
            i++;
         }
         start = i;
         i++;
         while (line[i] != ' ' && line[i] != '\0' 
                && line[i] != '\n' && line[i] != EOF) {
            i++;
         }
         tokens[currentWord] = (char *)malloc((i + 1 - start) * sizeof(char));
         strncpy(tokens[currentWord], &line[start], (i - start));
         tokens[currentWord][i - start] = '\0';
         currentWord++;
      }
      tokens[currentWord] = '\0';       //null terminating the array
   }
   
   return tokens;
}
