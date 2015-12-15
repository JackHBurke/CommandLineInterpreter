/*   Jack Burke
     10/16/15
     calc.c

     This program works as a basic calculater, evalutating two numbers with a 
     opperator (+, -, *, and /)   */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "calc.h"
#include "tokenizer.h"

#define MAX_BUFFER 1024

int main (int argc, char **argv) {
   while(!feof(stdin)) {      
      int x,y,z;
	 char opp;
         scanf("%d %c %d", &x, &opp, &y);    //Only takes in 2 ints, with a char in between
         if (feof(stdin)) {
	    return 0;
	 }   
	 int wrongExp = 0;   
         //Since our operator is read in as a character, a switch statement
         //is used to evalutate z.        
	 switch(opp) {
	    case '+':
	       z = x+y;
	       break;
	    case '-':
	       z = x-y;
	       break;
	    case '*':
	       z = x*y;
	       break;
	    case '/': 
	       z = x/y;
	       break;
	    default:
	       wrongExp = 1;        //To check for a valid operator
	       break;
	 }
	 if (wrongExp) {
	    printf("Usage : int opp int\n");
	 } else {
	    printf("%d %c %d = %d\n", x, opp, y, z);
	 }
   }
   return 0;
}
