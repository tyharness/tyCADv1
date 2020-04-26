/*
http://www.tyharness.dx.am/tctutorial/index.htm

This is a work in progress  rough and ready programming


tyCAD - a simple set of c functions for use with computer aided design CAD.  I often find with various CAD applications
that they are limted and I know 5 lines of code will probably solve the problem.  Therefore here's some functions you may
find useful.  Please It's not a graphical editor/application but produces data in a DXF format so you can import into the CAD application of your choice.


From CLI

To compile:
make

To run
./tyCADv1


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <math.h>
#include "tydxfout.h"
//#include "SIR1.c"


#define WHITE_SQUARE "\33[48;5;15m"
#define BLACK_SQUARE "\33[48;5;0m"
#define BLACK_PIECE "\33[38;5;1m"
#define WHITE_PIECE "\33[38;5;2m"
#define ANSI_RESET  "\33[m"
typedef enum { false, true } bool;


bool printhelp();


int main(int argc, char **argv)
{

printhelp();

return 0;

}//end main



/*==================Main menu screen and help page==============================================================*/





bool printhelp(){


char strChoice[30];



int choice = -1;

do{
 system("clear"); 
 
 printf(WHITE_SQUARE BLACK_PIECE "|==============================================================================|\n"); 
 printf(                         "| Welcome to TyCAD Ty Harness Computer Aided Design V1                         |\n");
 printf(                         "|==============================================================================|\n" ANSI_RESET); 
 printf("| Command Line parameters------------------------------------------------------|\n");
 printf("|==============================================================================|\n"); 
 printf("|==============================================================================|\n"); 
 printf("| Menu driven options:                                                         |\n");
 printf("|==============================================================================|\n");
 printf("|0) DXF map and grid stdout 			1)to file:ukMapExample.dxf      |\n");
 printf("|2) DXF Nightinggale/Fibonacci Example stdout 	3)to file:FibonacciExample.dxf  |\n");
 printf("|4) DXF Nightingale Example stdout 	        5)to file:NightingaleExample.dxf|\n");
 printf("|6) Generate html table for covid-19 csv data  7)to file:cfrTableHTML.txt      |\n");
// printf("|8) Generate gnuplot script SIR Example        9)to file                       |\n");
 printf("|==============================================================================|\n"); 
 printf("| Press Ctrl c to quit at any time                                            |\n");
 printf("|==============================================================================|\n"); 

 fgets(strChoice,30,stdin);

 if (strcmp(strChoice,"0\n")  == 0)   choice = 0;   
 if (strcmp(strChoice,"1\n")  == 0)   choice = 1;   	

 if (strcmp(strChoice,"2\n")  == 0)   choice = 2;   
 if (strcmp(strChoice,"3\n")  == 0)   choice = 3;   	
 if (strcmp(strChoice,"4\n")  == 0)   choice = 4;   	
 if (strcmp(strChoice,"5\n")  == 0)   choice = 5;   
 if (strcmp(strChoice,"6\n")  == 0)   choice = 6;   
 if (strcmp(strChoice,"7\n")  == 0)   choice = 7;   
 //if (strcmp(strChoice,"8\n")  == 0)   choice = 8;   
 //if (strcmp(strChoice,"9\n")  == 0)   choice = 9;   
 
  
 
 
} while (choice == -1);


system("clear"); 

if (choice == 0)printMap(); 
if (choice == 1){
	stdout = freopen("ukMapExample.dxf", "w", stdout);
	printMap();
}

if (choice == 2)generateFibonacciExample();
if (choice == 3){
	stdout = freopen("FibonacciExample.dxf", "w", stdout);
	generateFibonacciExample();
}

if (choice == 4)generateNightingaleExample();
if (choice == 5){
	stdout = freopen("NightingaleExample.dxf", "w", stdout);
	generateNightingaleExample();
}


if (choice == 6)generateHTMLtablefromCSVtest();
if (choice == 7){
	stdout = freopen("cfrTableHTML.txt", "w", stdout);
	generateHTMLtablefromCSVtest();
}

/*
if (choice == 8)generateSIRdata();
if (choice == 9){
	stdout = freopen("SIR.dat", "w", stdout);
	generateSIRdata();
}
*/

return true;
}


