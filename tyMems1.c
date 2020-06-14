
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tyCADv1.h"
#include "tydxfout.h"
#include "tyMath.h"
#include "tyMems1.h"



/*
Generate the mems1e design using polyline data and write the polyline
* to stdout or file
*/



void
mems1e ()
{


  int n = 80;

  double x, y, L, t, g, sh, susg, suse, susL, sust;


  L = 100.0;			//Length of electrode
  t = 10.0;			//thickness of electrode
  g = 4.0;			//eletrode gapping
  sh = 100;			//spar height  
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

  sprintf (strText, "Planar thickness of = %4.3e [um]", pt * 1e-6);	//just afor printing
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





void
z88v15z88mat (int nMaterials, int nElements)
{

/*this needs to handle mulitple materials but to get going I'm just using 1*/

  printf ("%d\n", nMaterials);
  printf ("1 %d 51.txt\n", nElements);

}

void
z88v15z88_51 (int matNo, double E, double Poisson)
{
//printf("+1.08000E+011 +3.00000E-001"); 
  printf ("%e %e\n", E, Poisson);
}






void
z88v15z88man ()
{

  printf ("DYNAMIC START\n\n");

  printf ("GLOBAL START\n");
  printf ("IBFLAG 1\n");
  printf ("IPFLAG 0\n");
  printf ("IHFLAG 0\n");
  printf ("SIMCASE 1\n");
  printf ("GLOBAL END \n\n");

  printf ("SOLVER START\n");
  printf ("MAXIT 5000\n");
  printf ("EPS 0.0000001\n");
  printf ("RALPHA\n");
  printf ("ROMEGA 0.9\n");
  printf ("SOLVER END\n\n");

  printf ("STRESS START\n");
  printf ("KDFLAG 1\n");
  printf ("ISFLAG 0\n");
  printf ("STRESS END\n\n");

  printf ("DYNAMIC END\n");


}



void
z88v15z88elp (int nElements, double A, double Izz, double maxdistz)
{


  /*beam 13
     > Cross-sectional area QPARA
     > insert 0 for second moment of inertia I yy (bending around y-y axis)
     > insert 0 for max. distance e yy from neutral axis y-y
     > Second moment of inertia I zz (bending around z-z axis)
     > Max. distance e zz from neutral axis z-z
     > insert 0 for second moment of area (torsion)
   */



  printf ("1\n");
  //printf("1 9 +1.00000E-010 0 0 +8.33333E-022 +5.00000E-006 0 0\n");        
  printf ("1 %d %e %e %e %e %e %e %e\n", nElements, A, 0.0, 0.0, Izz,
	  maxdistz, 0.0, 0.0);


}



void
z88v15z88int ()
{
  printf ("\n");
}


void
z88v15z88i2 ()
{

  printf ("%d Number of BC\n", 5);
  printf ("1 1 2 0.00000E+000  Node1  disp x\n");
  printf ("1 2 2 0.00000E+000  Node1  disp y\n");
  printf ("1 3 2 0.00000E+000  Node1  rotation\n");
  printf ("10 3 2 0.00000E+000 Node10 rotation\n");
  printf ("10 2 1 -1.0000E-006 Node 10 Load y\n");


}





void
z88v15z88i1 (double L, int nElements, int nNodes)
{



  //double L = 1e-3; //length of cantilever 1mm

  int nModel = 2;		// 2D Model
  //int nElements = 9;    //Number of elements
  //int nNodes = 10;      //Number of nodes

  int etype = 13;		//Element type Beam 13
  int etypeDOF = 3;		// 3DOF x,y, theta

  int tDOF = etypeDOF * nNodes;
  int coords = 0;		//0 for cartesian and 1 for polar

  //int nMaterials = 1;

  double nodex = 0.0;
  double nodey = 0.0;
  double nodez = 0.0;

/*1 st number: Dimension of the structure (2 or 3) [Long]
2 nd number: Number of nodes of the FEA structure [Long]
3 rd number: Number of elements [Long]
4 th number: Number of degrees of freedom [Long]
5 th number: Coordinate flag KFLAG (0 or 1) [Long]. Attention: This position was in former
Z88 versions reserved for the number of materials NEG.
*/
  printf
    ("%d %d %d %d %d  modelDimension, Number of Nodes, Number of Elements, Total DOF, KFLAG(cartesian) \n",
     nModel, nNodes, nElements, tDOF, coords);

  for (int i = 1; i <= nNodes; i++)
    {
      printf ("%d %d %e %e %e node %d\n", i, etypeDOF, nodex, nodey, nodez,
	      i);
      nodex += (L / (double) nElements);
    }


  for (int i = 1; i <= nElements; i++)
    {
      printf ("%d %d element %d\n", i, etype, i);
      printf ("%d %d\n", i, i + 1);
    }






}
