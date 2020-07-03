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
#include <time.h>
#include "tyCADv1.h"
#include "tydxfout.h"
#include "tyMems1.h"
#include "SIR.h"
#include "tyZ88.h"
#include "tyMath.h"




#define WHITE_SQUARE "\33[48;5;15m"
#define BLACK_SQUARE "\33[48;5;0m"
#define BLACK_PIECE "\33[38;5;1m"
#define WHITE_PIECE "\33[38;5;2m"
#define ANSI_RESET  "\33[m"



bool printhelp ();
time_t rawtime;
time_t current_time;



int
main (int argc, char **argv)
{


  DEBUG = true;

  struct tm *info;
  char buffer[80];
  time (&rawtime);

  char *time_string;
  info = localtime (&rawtime);




  if (DEBUG)
    debugfile = fopen ("output/debug.txt", "w+");

  current_time = time (NULL);
  time_string = ctime (&current_time);

  if (DEBUG)
    fprintf (debugfile, "%s %s\n", "Debug file opened", time_string);


  strftime (buffer, 80, "%d", info);
  int d = atoi (buffer);

  strftime (buffer, 80, "%m", info);
  int m = atoi (buffer);

  strftime (buffer, 80, "%Y", info);
  int y = atoi (buffer);


  printhelp (d, m, y);

  current_time = time (NULL);
  time_string = ctime (&current_time);

  if (DEBUG)
    {
      fprintf (debugfile, "Close debug file %s", time_string);

      fclose (debugfile);
    }


  return 0;

}				//end main



/*==================Main menu screen and help page==============================================================*/

void
caseFatalityRate ()
{

  int nDeaths = 0;
  int nCases = 0;



  printf ("Enter the total number of deaths ");
  scanf ("%d", &nDeaths);

  printf ("Enter the total Confirmed Cases ");
  scanf ("%d", &nCases);

  printf ("...exported to output directory...\n");

  stdout = freopen ("input/coviddataUK.csv", "a", stdout);
  printf ("%d,%d\n", nDeaths, nCases);

  stdout = freopen ("input/AllcoviddataUK.csv", "a", stdout);
  printf ("%d,%d\n", nDeaths, nCases);


}



bool
printhelp (int d, int m, int y)
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
	("|0)file:WorldMapExample.dxf                   1)file:ukMapExample.dxf          |\n");
      printf
	("|2) DXF Nightinggale/Fibonacci Example stdout 3)file:FibonacciExample.dxf      |\n");
      printf
	("|4) DXF Nightingale Example stdout            5)file:NightingaleExample.dxf    |\n");
      printf
	("|6) Generate html table for covid-19 csv data and Nightingale Pie Chart[dxf]   |\n");
      printf
	("|7) Generate xy graph example files:                                           |\n");
      printf
	("|8) Generate gnuplot script SIR Example       9)file:SIR.dat                   |\n");
      printf
	("|10)Generate Airy1830 Example                 11)Airy1830.dxf                  |\n");
      printf
	("|12)Mems Device Example  : output/mems1e.dxf  output/z88example/z88x.dxf       |\n");
      printf
	("|13)Z88 FE Example bp5: z88 input files in output/z88example/z88i*             |\n");
      printf
	("|14)Z88 FE Example bp6: z88 input files and z88x.dxf                           |\n");

      printf
	("|15)Polyline and mass properties example                                       |\n");


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
	choice = 400;
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
      if (strcmp (strChoice, "13\n") == 0)
	choice = 13;
      if (strcmp (strChoice, "14\n") == 0)
	choice = 14;
      if (strcmp (strChoice, "15\n") == 0)
	choice = 15;



      //non documented features
      if (strcmp (strChoice, "cfr\n") == 0)
	choice = 100;
      if (strcmp (strChoice, "b\n") == 0)
	choice = 200;
      if (strcmp (strChoice, "flange\n") == 0)
	choice = 300;
      if (strcmp (strChoice, "xy\n") == 0)	//non documented
	choice = 400;

      if (strcmp (strChoice, "circle\n") == 0)
	choice = 1000;
      if (strcmp (strChoice, "anulus\n") == 0)
	choice = 1001;
      if (strcmp (strChoice, "square\n") == 0)
	choice = 1002;




    }
  while (choice == -1);


  system ("clear");

  if (choice == 0)
    {
      printf ("WorldMapExample.dxf exported to output directory...\n");
      stdout = freopen ("output/WorldMapExample.dxf", "w", stdout);
      printWorldMapExample ();
    }



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
    generateNightingaleExample (30, 0, 0, 0);

  if (choice == 5)
    {

      int nd = 31;

      if (m == 4 || m == 6 || m == 8 || m == 11)
	nd = 30;
      //leap year cheeck
      if (m == 2)
	nd = 28;
      if (y % 4 == 0 && y % 100 != 100)
	nd = 29;

      printf ("NightingaleExample.dxf exported to output directory...\n");
      stdout = freopen ("output/NightingaleExample.dxf", "w", stdout);
      generateNightingaleExample (nd, d, m, y);
    }




/*====================================================================*/




  if (choice == 6)
    {

      printf ("Generate CFR graphs and tables...\n");

      //ask user for latest data:
      caseFatalityRate ();



      stdout = freopen ("input/COVID19dataUK.csv", "w", stdout);
      convert_coviddataUK_to_COVID19dataUK (m, y);

      stdout = freopen ("output/cfrTableHTML.txt", "w", stdout);
      generateHTMLtablefromCSVtest (d, m, y);



      //the number of days in the month
      int nd = 31;

      if (m == 4 || m == 6 || m == 8 || m == 11)
	nd = 30;
      //leap year cheeck
      if (m == 2)
	nd = 28;
      if (y % 4 == 0 && y % 100 != 100)
	nd = 29;






      char strText[30];

      sprintf (strText, "output/NightingaleExample_%d_%d_%d.dxf", d, m, y);


      stdout = freopen (strText, "w", stdout);
      generateNightingaleExample (nd, d, m, y);


      stdout = freopen ("output/XYdataexampleCovidDeathsUK.dxf", "w", stdout);

      memset (strText, 0, sizeof strText);
      sprintf (strText, "COVID Deaths 1/3/2020 - %d/%d/%d", d, m, y);

      print_xy_graph ("input/AllcoviddataUK.csv", strText, "Days",
		      "Total COVID19 UK Deaths", "XYdata", 7, 0);

      memset (strText, 0, sizeof strText);
      sprintf (strText, "COVID Cases 1/3/2020 - %d/%d/%d", d, m, y);

      stdout = freopen ("output/XYdataexampleCovidCasesUK.dxf", "w", stdout);
      print_xy_graph ("input/AllcoviddataUK.csv", strText, "Days",
		      "Total COVID19 UK cases", "XYdata", 7, 1);




    }

/*====================================================================*/

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

      // printf ("circle.dat exported to output directory...\n");     
      //stdout = freopen ("input/circle.dat", "w", stdout); 
      //  exportPolylineCircle();



      //then generate the dxf
      stdout = freopen ("output/mems1e.dxf", "w", stdout);
      memsDeviceDXF ();



      //some additional files that generate by z88x





    }



  if (choice == 13)
    {

      /* bp5 problem
       * 
       * 
       * copy all the output/z88example direcotry to the root directory of z88r binary file.
       * 
       * you can use the gui commander interface
       * ./z88com
       * 
       * then run the solver
       * ./z88r -c -choly &
       * if succesful
       * view the displacements
       * cat z88o2.txt &
       * view the stress
       * cat z880o.txt &
       * to use Prof. FR opengl output program
       * ./z88o &
       * 
       * I think it's as easy to generate the input files direcotry as per this example
       * but you can convert input files to z88x.dxf using
       * 
       * ./z88x -iatx
       * 
       */


      printf ("z88 input files exported to output/z88example directory...\n");


      /*Using a z88 Beam13 element to describe the cantilever beam problem bp5 */

      int nMaterials = 1;
      int nElements = 9;
      int nNodes = nElements + 1;
      int matNo = 1;
      double E = 1.08e11;
      double Poisson = 0.3;


      double d = 10e-6;

      double A = d * d;
      double maxdistz = d * 0.5;
      double Izz = (d * d * d * d) / 12;	//db^3/12  

      double L = 1e-3;		//length of cantilever 1mm


      stdout = freopen ("output/z88example/z88i1.txt", "w", stdout);
      z88v15z88i1 (L, nElements, nNodes);

      stdout = freopen ("output/z88example/z88i2.txt", "w", stdout);
      z88v15z88i2 ();


      stdout = freopen ("output/z88example/z88int.txt", "w", stdout);
      z88v15z88int ();

      stdout = freopen ("output/z88example/z88man.txt", "w", stdout);
      z88v15z88man ();


      stdout = freopen ("output/z88example/z88mat.txt", "w", stdout);
      z88v15z88mat (nMaterials, nElements);

      stdout = freopen ("output/z88example/51.txt", "w", stdout);
      z88v15z88_51 (matNo, E, Poisson);

      stdout = freopen ("output/z88example/z88elp.txt", "w", stdout);
      z88v15z88elp (nElements, A, Izz, maxdistz);

      stdout = freopen ("output/z88example/z88i5.txt", "w", stdout);
      z88v15z88i5 ();


    }



  if (choice == 14)
    {


      /*Example bp6
       * convert the z88x to z88 imput files
       *./z88x -iafx
       */

      int nMaterials = 1;
      int nElements = 9;
      //int nNodes = 3;
      int matNo = 1;
      double E = 1.08e11;
      double Poisson = 0.3;


      double d = 10e-6;

      double A = d * d;
      double maxdistz = d * 0.5;
      double Izz = (d * d * d * d) / 12;	//db^3/12  

      double L = 1e-3;		//length of cantilever          


      printf
	("z88x.dxf,z88int,z88mat,z88man,51,z88elp exported to output directory...\n");

      stdout = freopen ("output/z88example/z88int.txt", "w", stdout);
      z88v15z88int ();

      stdout = freopen ("output/z88example/z88man.txt", "w", stdout);
      z88v15z88man ();


      stdout = freopen ("output/z88example/z88mat.txt", "w", stdout);
      z88v15z88mat (nMaterials, nElements);

      stdout = freopen ("output/z88example/51.txt", "w", stdout);
      z88v15z88_51 (matNo, E, Poisson);

      stdout = freopen ("output/z88example/z88elp.txt", "w", stdout);
      z88v15z88elp (nElements, A, Izz, maxdistz);




      stdout = freopen ("output/z88example/z88x.dxf", "w", stdout);
      z88ExampleDXF (L);
    }


  if (choice == 15)
    {



      char *filename[10];
      char strChoice[10];





      system ("clear");


      int ret = 0;		//default
      filename[0] = "input/square.dat";

      filename[1] = "input/square.dat";
      filename[2] = "input/square1.dat";
      filename[3] = "input/square2.dat";
      filename[4] = "input/circle.dat";
      filename[5] = "input/zed.dat";
      filename[6] = "input/zed1.dat";
      filename[7] = "input/mems1e.csv";
      filename[8] = "input/anulus.dat";




      for (int i = 1; i <= 8; i++)
	{
	  printf ("%d) %s\n", i, filename[i]);
	}


      fgets (strChoice, 30, stdin);


      if (strcmp (strChoice, "1\n") == 0)
	ret = 1;
      if (strcmp (strChoice, "2\n") == 0)
	ret = 2;
      if (strcmp (strChoice, "3\n") == 0)
	ret = 3;
      if (strcmp (strChoice, "4\n") == 0)
	ret = 4;
      if (strcmp (strChoice, "5\n") == 0)
	ret = 5;
      if (strcmp (strChoice, "6\n") == 0)
	ret = 6;
      if (strcmp (strChoice, "7\n") == 0)
	ret = 7;


      if (strcmp (strChoice, "8\n") == 0)
	ret = 8;



      printf
	("%s input/MassPropertiesExample.dxf exported to output directory...\n",
	 filename[ret]);

      stdout = freopen ("output/MassPropertiesExample.dxf", "w", stdout);
      testMassProperties (filename[ret]);


    }







/*non documented functions*/

  if (choice == 100)
    {
      //test the user input function
      caseFatalityRate ();
    }


/*non documented functions*/


  if (choice == 200)
    {
      /*generate  a blank dxf file */
      printf ("blank.dxf exported to output directory...\n");
      stdout = freopen ("output/blank.dxf", "w", stdout);
      printblankDXF ();
    }



  /*non documneted features
   * 
   */

  if (choice == 300)
    {

      printf ("flange2d.dxf exported to output directory...\n");
      stdout = freopen ("output/flange2d.dxf", "w", stdout);
      print2Dflange ();
    }


  if (choice == 400)
    {
      //just some sample xy graphs for debugging

      printf ("XYexample.dxf exported to output directory...\n");

      stdout = freopen ("output/XYdatatest1mode0.dxf", "w", stdout);
      print_xy_graph ("input/xytestdata.dat", "test mode 0", "xind", "col1",
		      "test1", 7, 0);

      stdout = freopen ("output/XYdatatest1mode1.dxf", "w", stdout);
      print_xy_graph ("input/xytestdata.dat", "test mode 1", "xind", "col2",
		      "test2", 7, 1);

      stdout = freopen ("output/XYdatatest1mode2.dxf", "w", stdout);
      print_xy_graph ("input/xytestdata.dat", "test mode 2", "col1", "col2",
		      "test3", 7, 2);


    }


  if (choice == 1000)
    {
      printf ("circle.dat exported to output directory...\n");
      stdout = freopen ("input/circle.dat", "w", stdout);
      exportPolylineAnnulus ();
    }

  if (choice == 1001)
    {
      printf ("anulus.dat exported to output directory...\n");
      stdout = freopen ("input/anulus.dat", "w", stdout);
      exportPolylineAnnulus ();
    }

  if (choice == 1002)
    {
      printf ("square.dat exported to output directory...\n");
      stdout = freopen ("input/square.dat", "w", stdout);
      exportPolylineAnnulus ();
    }



  return true;
}
