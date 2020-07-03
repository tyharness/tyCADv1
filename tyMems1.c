
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tyCADv1.h"
#include "tydxfout.h"
#include "tyMath.h"
#include "tyMems1.h"
#include "tyZ88.h"





void
mems1e ()
{
/*
Generate the mems1e design using polyline data and write the polyline
* to stdout or file
*/


  int n = 80;

  double x, y, L, t, g, sh, susg, suse, susL, sust;


  L = 100.0;			//Length of electrode
  t = 10.0;			//thickness of electrode
  g = 4.0;			//eletrode gapping
  sh = 100.0;			//spar height  
  susg = 120.0;			//suspension gapping
  suse = 30.0;			//suspenspen thickness at the ends
  susL = 400.0;			//cantilever Length
  sust = 10.0;			//cantilever thickness (or depth in terms of bending moments) 

  double linkt = 20.0;		//pad link thickness
  double linkL = 500.0;		//pad link arm length 
  double pad = 300.0;		//suspensed pad sq. dim

  x = 0.0;
  y = 0.0;


  printf ("%f,%f\n", -sh - susg - sust, 0.0);


  for (int i = 0; i < n; i++)
    {


      if (i % 2 == 0)
	{
	  x = 0.0;
	  if (i == 0)
	    printf ("%f,%f\n", x, y);
	  x = L;
	  printf ("%f,%f\n", x, y);
	  y += t;
	  printf ("%f,%f\n", x, y);
	}
      else
	{
	  x = 0;
	  printf ("%f,%f\n", x, y);

	  if (i != n - 1)
	    {
	      y += t + g;
	      printf ("%f,%f\n", x, y);

	    }
	}


    }

  x -= sh + susg + sust;
  printf ("%f,%f\n", x, y);

  double ysparL = y;

  y -= susL;
  printf ("%f,%f\n", x, y);

  x += sust;
  printf ("%f,%f\n", x, y);

  y += susL - suse;
  printf ("%f,%f\n", x, y);

  x += susg;
  printf ("%f,%f\n", x, y);

  y -= 0.5 * (ysparL - linkt) - suse;
  printf ("%f,%f\n", x, y);

  x -= linkL;
  printf ("%f,%f\n", x, y);

  y += 0.5 * (pad - linkt);
  printf ("%f,%f\n", x, y);

  x -= pad;
  printf ("%f,%f\n", x, y);

  y -= pad;;
  printf ("%f,%f\n", x, y);

  x += pad;
  printf ("%f,%f\n", x, y);

  y += 0.5 * (pad - linkt);
  printf ("%f,%f\n", x, y);

  x += linkL;
  printf ("%f,%f\n", x, y);

  y -= 0.5 * (ysparL - linkt) - suse;
  printf ("%f,%f\n", x, y);

  x -= susg;
  printf ("%f,%f\n", x, y);

  y += susL;
  printf ("%f,%f\n", x, y);


  x -= sust;
  printf ("%f,%f\n", x, y);

}





void
memsDeviceDXF ()
{

  if (DEBUG)
    fprintf (debugfile, "Function memsDeviceDXF \n");


  //write out dxf data
  printDXFheader ();

  printPolyLineHeader ("mems1e", 3, 1);
  //read in the polyline

  char *filename;
  filename = "input/mems1e.csv";

  //test my area algorithm with some simple polylines
  //filename = "input/square.dat";
  //filename = "input/circle.dat";

  FILE *file;
  char c;

  file = fopen (filename, "r");


  char sNum[30];
  int q = 0;

  double x0 = 0.0;
  double y0 = 0.0;

  double x00 = 0.0;
  double y00 = 0.0;

  double x1 = 0.0;
  double y1 = 0.0;

  double area = 0.0;
  double dA;

  int i = 0;
  char strText[32];


  if (file)
    {


      while (c != EOF || (int) c != 255)
	{
	  c = getc (file);
	  if (c == EOF || (int) c == 255)
	    break;
	  if (c != ',' && c != ' ')
	    {
	      sNum[q] = c;
	      q++;
	    }

	  if (c == ',')
	    {
	      x1 = atof (sNum);
	      q = 0;
	      sNum[q] = '\0';
	    }

	  if (c == '\n')
	    {
	      y1 = atof (sNum);
	      //printf("%f ---- %f\n",dx,dy);
	      printPloyLineVertex ("mems1e", x1, y1, 0.0);
	      q = 0;
	      sNum[q] = '\0';


	      if (i == 0)
		{
		  x00 = x1;
		  y00 = y1;
		}

	      if (i > 0)
		{
		  dA = x0 * y1 - x1 * y0;
		  area += dA;
		}

	      x0 = x1;
	      y0 = y1;
	      i++;

	    }

	}

      fclose (file);
    }

  printPolyLineFooter ();


  dA = x0 * y00 - x00 * y0;
  area += dA;
  area *= 0.5;

  sprintf (strText, "Area = %4.3e [um]^2", area);
  printDXFtext (strText, "mems1eProperties", 3, -1000.0, -50.0, 25.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  //generate the lightening holes they also serve to help under etch the suspended geometry
  //give it the ol' drilliam 

  double dx = -70.0;
  double dy = 30.0;
  int n = 4;
  double g = 10.0;




  double SparL = 946.0;
  double halfSparL = SparL * 0.5;

  double halfSparLength = 886.0 * 0.5 - 0.5 * g;

  double sy = ((halfSparLength - (n - 1) * g) / n);


  //trianglular holes
  double hx = 47.5;		//side length of tri hole
  double hy = sy - 1.5 * g;	//side length of tri hole

  double ha = 0.5 * hx * hy;	//area of hole


  double t = tan (hy / hx);	//angle of diaganol link  
  double xshift = g / cos (t);	//keep the diaganal g thick
  double pt = 7.0;		//planar thickness um need for vol and mass


  struct tyPoint P, P1, P2;

  printLine ("construction lines", -1000.0, halfSparL, 200.0, halfSparL, 2);	//just a centre line to mirror the holes about

  for (int i = 0; i < n; i++)
    {


      printPolyLineHeader ("mems1e_holes", 1, 1);
      printPloyLineVertex ("mems1e_holes", dx, dy, 0.0);
      printPloyLineVertex ("mems1e_holes", dx + hx, dy, 0.0);
      printPloyLineVertex ("mems1e_holes", dx + hx, dy + sy, 0.0);
      printPolyLineFooter ();

      P = mirrorPointAboutLine (-100.0, halfSparL, 100.0, halfSparL, dx, dy);
      P1 =
	mirrorPointAboutLine (-100.0, halfSparL, 100.0, halfSparL, dx + hx,
			      dy);
      P2 =
	mirrorPointAboutLine (-100.0, halfSparL, 100.0, halfSparL, dx + hx,
			      dy + sy);

      printPolyLineHeader ("mems1e_holesm", 1, 1);
      printPloyLineVertex ("mems1e_holesm", P.x, P.y, 0.0);
      printPloyLineVertex ("mems1e_holesm", P1.x, P1.y, 0.0);
      printPloyLineVertex ("mems1e_holesm", P2.x, P2.y, 0.0);
      printPolyLineFooter ();




      printPolyLineHeader ("mems1e_holes", 1, 1);
      printPloyLineVertex ("mems1e_holes", dx + hx + xshift, dy + sy, 0.0);
      printPloyLineVertex ("mems1e_holes", dx + xshift, dy, 0.0);
      printPloyLineVertex ("mems1e_holes", dx + xshift, dy + sy, 0.0);
      printPolyLineFooter ();

      P =
	mirrorPointAboutLine (-100.0, halfSparL, 100.0, halfSparL,
			      dx + hx + xshift, dy + sy);
      P1 =
	mirrorPointAboutLine (-100.0, halfSparL, 100.0, halfSparL,
			      dx + xshift, dy);
      P2 =
	mirrorPointAboutLine (-100.0, halfSparL, 100.0, halfSparL,
			      dx + xshift, dy + sy);

      printPolyLineHeader ("mems1e_holesm", 1, 1);
      printPloyLineVertex ("mems1e_holesm", P.x, P.y, 0.0);
      printPloyLineVertex ("mems1e_holesm", P1.x, P1.y, 0.0);
      printPloyLineVertex ("mems1e_holesm", P2.x, P2.y, 0.0);
      printPolyLineFooter ();

      dy += sy + g;


    }


  /*print out the mems1e mass properties
     //Please note I use um as the units in my polyline file.  Although LibreCAD uses mm for
     its dim tool by default DXF is really unitless/dimensionless.
   */


  sprintf (strText, "Area of %d holes = %4.3e [um]^2", 4 * n, 4 * ha);	//4 sets per n iteration
  printDXFtext (strText, "mems1eProperties", 1, -1000.0, -100.0, 25.0, 0.0,
		0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Area of Suspended Structure = %4.3e [um]^2", area - 4 * ha);	//um^2
  printDXFtext (strText, "mems1eProperties", 7, -1000.0, -150.0, 25.0, 0.0,
		0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Planar thickness of = %4.3e [um]", pt);	//just afor printing
  printDXFtext (strText, "mems1eProperties", 7, -1000.0, -200.0, 25.0, 0.0,
		0);
  memset (strText, 0, sizeof strText);


  sprintf (strText, "Volume of Suspended Structure = %4.3e [um]^3", (area - 4 * ha) * pt);	//um^3
  printDXFtext (strText, "mems1eProperties", 7, -1000.0, -250.0, 25.0, 0.0,
		0);
  memset (strText, 0, sizeof strText);


  //density of silicon = 2300 kg/m^3
  // 1 kg/m^3 = 1e9 ug / 1e18 um^3 = 1e-9 ug/um^3
  // mass = rho*vol 


  sprintf (strText, "Mass = %4.3e [ug]", (area - 4 * ha) * pt * 2300e-9);
  printDXFtext (strText, "mems1eProperties", 7, -1000.0, -300.0, 25.0, 0.0,
		0);
  memset (strText, 0, sizeof strText);


  if (DEBUG)
    {
      //  printLine("kjklj",-100.0,500.00, 100.0, 500.0,2); //reflection line
      //  P = mirrorPointAboutLine (-100.0, 500.00, 100.0, 500.0, 100.0, -1000.0);        
      //  fprintf(debugfile,"x=%f, y=%f \n", P.x, P.y );
      //  printLine("test",100.0, -1000.0,   P.x, P.y,  4);

    }


  printDXFfooter ();



}


/* Create FE geometry to run through Frank Reig's z88 software
 */

void
z88ExampleDXF (double Length)
{

/*
 * Z88GEN: Layer for general information (1st input group in the mesh generator input file
Z88NI.TXT and general structure data file Z88I1.TXT).
* 
* 
Z88KNR: Layer including the node numbers.
* 
* 
* 
* 
Z88EIO: Layer including the element information like element type and in the case of mesh
generator input file Z88NI.TXT control information for the mesh generator.
* 
* 
* 
Z88NET: Layer containing the mesh which was drawn or outlined in defined order.
* 
* 
Z88RBD: Layer containing the contents of the boundary conditions file Z88I2.TXT.
Z88FLA: Layer containing the surface and pressure loads as defined for Z88I5.TXT


A further layer, Z88PKT, is produced by Z88X if you convert from Z88 to CAD. It shows all
nodes with a point marker in order to better recognize the nodes. For the reverse step, from
CAD to Z88, it is completely insignificant.
*/







  double x = 0.0;

  int n = 9;			//number of elements
  double dx = Length / (double) n;


  double th = 0.1 * dx;		//text height
  double tt = 0.5 * dx;

  char strText[32];

  //write out dxf data
  //printDXFheader ();
  z88xHeader ();

  /*Z88GEN: Layer for general information (1st input group in the mesh generator input file
     Z88NI.TXT and general structure data file Z88I1.TXT).
     * 
     * 1 st number: Dimension of the structure (2 or 3) [Long]
     2 nd number: Number of nodes of the FEA structure [Long]
     3 rd number: Number of elements [Long]
     4 th number: Number of degrees of freedom [Long]
     5 th number: Coordinate flag KFLAG (0 or 1) [Long]. Attention: This position was in former
     Z88 versions reserved for the number of materials NEG.
     * 
     * Z88I1.TXT 2 3 2 9 0
   */

  sprintf (strText, "Z88I1.TXT 2 %d %d %d 0", n + 1, n, 3 * (n + 1));
  printDXFtext (strText, "Z88GEN", 256, tt, tt, th, 0.0, 0);
  memset (strText, 0, sizeof strText);




  int m = 4;			//No of loads/boundary conditions


  sprintf (strText, "Z88I2.TXT %d", m);
  printDXFtext (strText, "Z88RBD", 256, tt, -2 * tt, th, 0.0, 0);
  memset (strText, 0, sizeof strText);


  /*2 nd input group:
     The boundary conditions and loads are defined. For every boundary condition and for every
     load one line, respectively.
     1st number: node number with boundary condition: load or constraint [Long]
     2nd number: Respective degree of freedom (1,2,3,4,5,6) [Long]
     3rd number: Condition flag: 1 = force [Long] or 2 = displacement [Long]
     4th number: Value of the load or displacement [Double]
     * */



  printDXFtext ("RBD 1 1 1 2   0.00000E+000", "Z88RBD", 256, tt, -3 * tt, th,
		0.0, 0);
  printDXFtext ("RBD 2 1 2 2   0.00000E+000", "Z88RBD", 256, tt, -4 * tt, th,
		0.0, 0);
  printDXFtext ("RBD 3 1 3 2   0.00000E+000", "Z88RBD", 256, tt, -5 * tt, th,
		0.0, 0);
  printDXFtext ("RBD 4 10 3 1  -1.00000E-006", "Z88RBD", 256, tt, -6 * tt, th,
		0.0, 0);



  printDXFtext ("Z88I5.TXT 0", "Z88FLA", 256, tt, -8 * tt, th, 0.0, 0);



  for (int i = 1; i <= n + 1; i++)
    {

      sprintf (strText, "P %d", i);

      printf ("999\n");
      printf ("----------Point and Test %s  \n", strText);

      //Z88KNR: Layer including the node numbers.
      Z88printPoint (x, 0.0);

      Z88printDXFtext (strText, x, 0.0, th);
      memset (strText, 0, sizeof strText);
      x += dx;
    }


  x = 0;

  for (int i = 1; i <= n; i++)
    {
      sprintf (strText, "FE %d 13", i);
      printDXFtext (strText, "Z88EIO", 256, x + 0.3 * dx, 0.1 * tt, th, 0.0,
		    0);
      memset (strText, 0, sizeof strText);
      x += dx;
    }

  x = 0;

  for (int i = 1; i <= n; i++)
    {
      Z88printLine (x, 0.000, x + dx, 0.000);
      x += dx;
    }


  printDXFfooter ();


}
