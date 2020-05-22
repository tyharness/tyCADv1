/*
http://www.tyharness.dx.am/tctutorial/index.htm

This is a work in progress  rough and ready brutish programming


tyCAD - a simple set of c functions for use with computer aided design CAD.  I often find with various CAD applications
that they are limted and I know 5 lines of code will probably solve the problem.  Therefore here's some functions you may
find useful.  Please note it's not a graphical editor/application but produces data in a DXF format so you can import into the CAD application of your choice.


From CLI

To compile:
make

To tidy up the files and force all files to be compile use: 
make clean
and then make


To run
./tyCADv1


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "tydxfout.h"
#include "tyMems1.h"
#include "SIR.h"


#define WHITE_SQUARE "\33[48;5;15m"
#define BLACK_SQUARE "\33[48;5;0m"
#define BLACK_PIECE "\33[38;5;1m"
#define WHITE_PIECE "\33[38;5;2m"
#define ANSI_RESET  "\33[m"
typedef enum
{ false, true } bool;


bool printhelp ();


int
main (int argc, char **argv)
{

  printhelp ();

  return 0;

}				//end main



/*==================Main menu screen and help page==============================================================*/





bool
printhelp ()
{


  char strChoice[30];



  int choice = -1;

  do
    {
      system ("clear");

      printf (WHITE_SQUARE BLACK_PIECE
	      "|==============================================================================|\n");
      printf
	("| Welcome to TyCAD Ty Harness Computer Aided Design V1                         |\n");
      printf
	("|==============================================================================|\n"
	 ANSI_RESET);
      printf
	("| Command Line parameters-----to do--------------------------------------------|\n");
      printf
	("|==============================================================================|\n");
      printf
	("| Menu driven options:                                                         |\n");
      printf
	("|==============================================================================|\n");
      printf
	("|0) DXF map and grid stdout                   1)file:ukMapExample.dxf          |\n");
      printf
	("|2) DXF Nightinggale/Fibonacci Example stdout 3)file:FibonacciExample.dxf      |\n");
      printf
	("|4) DXF Nightingale Example stdout            5)file:NightingaleExample.dxf    |\n");
      printf
	("|6) Generate html table for covid-19 csv data and Nightingale Pie Chart[dxf]   |\n");
      printf
	("|8) Generate gnuplot script SIR Example       9)file:SIR.dat                   |\n");
      printf
	("|10)Generate Airy1830 Example                 11)Airy1830.dxf                  |\n");
      printf
	("|12)Mems Device Example: mems1e.dxf                                            |\n");
      printf
	("|==============================================================================|\n");
      printf
	("| Press Ctrl c to quit at any time                                             |\n");
      printf
	("|==============================================================================|\n");

      fgets (strChoice, 30, stdin);

      if (strcmp (strChoice, "0\n") == 0)
	choice = 0;
      if (strcmp (strChoice, "1\n") == 0)
	choice = 1;

      if (strcmp (strChoice, "2\n") == 0)
	choice = 2;
      if (strcmp (strChoice, "3\n") == 0)
	choice = 3;
      if (strcmp (strChoice, "4\n") == 0)
	choice = 4;
      if (strcmp (strChoice, "5\n") == 0)
	choice = 5;
      if (strcmp (strChoice, "6\n") == 0)
	choice = 6;
      if (strcmp (strChoice, "7\n") == 0)
	choice = 7;
      if (strcmp (strChoice, "8\n") == 0)
	choice = 8;
      if (strcmp (strChoice, "9\n") == 0)
	choice = 9;
      if (strcmp (strChoice, "10\n") == 0)
	choice = 10;
      if (strcmp (strChoice, "11\n") == 0)
	choice = 11;
      if (strcmp (strChoice, "12\n") == 0)
	choice = 12;





      if (strcmp (strChoice, "b\n") == 0)
	choice = 200;



    }
  while (choice == -1);


  system ("clear");

  if (choice == 0)
    printMap ();
  if (choice == 1)
    {
      printf ("ukMapExample.dxf exported to output directory...\n");
      stdout = freopen ("output/ukMapExample.dxf", "w", stdout);
      printMapwithDXFheadAndFoot ();
    }

  if (choice == 2)
    generateFibonacciExample ();
  if (choice == 3)
    {
      printf ("FibonacciExample.dxf exported to output directory...\n");
      stdout = freopen ("output/FibonacciExample.dxf", "w", stdout);
      generateFibonacciExample ();
    }

  if (choice == 4)
    generateNightingaleExample (30);
  if (choice == 5)
    {
      printf ("NightingaleExample.dxf exported to output directory...\n");
      stdout = freopen ("output/NightingaleExample.dxf", "w", stdout);
      generateNightingaleExample (31);
    }


  if (choice == 6)
    {
      stdout = freopen ("input/COVID19dataUK.csv", "w", stdout);
      convert_coviddataUK_to_COVID19dataUK (5, 20);

      stdout = freopen ("output/cfrTableHTML.txt", "w", stdout);
      generateHTMLtablefromCSVtest ();

      stdout = freopen ("output/NightingaleExample.dxf", "w", stdout);
      generateNightingaleExample (31);

    }


  if (choice == 8)
    generateSIRdata ();
  if (choice == 9)
    {
      printf ("SIR.dat exported to output directory...\n");
      stdout = freopen ("output/SIR.dat", "w", stdout);
      generateSIRdata ();
    }

  if (choice == 10)
    print_wgs_to_OS ();
  if (choice == 11)
    {
      printf ("Airy1830.dxf exported to output directory...\n");
      stdout = freopen ("output/Airy1830.dxf", "w", stdout);
      print_wgs_to_OS ();
    }


  if (choice == 12)
    {
      printf ("mems1e.dxf exported to output directory...\n");

      //generate the polyline data first
      stdout = freopen ("input/mems1e.csv", "w", stdout);
      mems1e ();
      //then generate the dxf
      stdout = freopen ("output/mems1e.dxf", "w", stdout);
      memsDeviceDXF ();
    }







  if (choice == 100)
    {
      printMapLatLong ("input/eire.csv");
    }


  if (choice == 200)
    {
      printf ("blank.dxf exported to output directory...\n");
      stdout = freopen ("output/blank.dxf", "w", stdout);
      printblankDXF ();
    }







  return true;
}
