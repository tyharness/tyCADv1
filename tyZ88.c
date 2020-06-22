#include "tyZ88.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


  /*beam 13 element
     > Cross-sectional area QPARA
     > insert 0 for second moment of inertia I yy (bending around y-y axis)
     > insert 0 for max. distance e yy from neutral axis y-y
     > Second moment of inertia I zz (bending around z-z axis)
     > Max. distance e zz from neutral axis z-z
     > insert 0 for second moment of area (torsion)
   */




  printf ("1\n1 %d %e %e %e %e %e %e %e\n", nElements, A, 0.0, 0.0, Izz,
	  maxdistz, 0.0, 0.0);


}



void
z88v15z88int ()
{
  printf ("\n");
}


void
z88v15z88i5 ()
{
  printf ("0\n");
}


void
z88v15z88i2 ()
{


  /*
   * Cantilever Fixed - Free
   * 
   * printf ("%d Number of BC\n", 4);
   printf ("1 1 2 0.00000E+000  Node1  disp x\n");
   printf ("1 2 2 0.00000E+000  Node1  disp y\n");
   printf ("1 3 2 0.00000E+000  Node1  rotation\n");
   printf ("10 2 1 -1.0000E-006 Node 10 Load y\n");
   */



  /*
   * Encastre - DisplacedEncastre 
   */

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

void
z88xHeader ()
{

  printf ("0\n");
  printf ("SECTION\n");
  printf ("2\n");
  printf ("HEADER\n");
  printf ("9\n");
  printf ("$ACADVER\n");
  printf ("1\n");
  printf ("AC1009\n");
  printf ("9\n");
  printf ("$INSBASE\n");
  printf ("10\n");
  printf ("0.0\n");
  printf ("20\n");
  printf ("0.0\n");
  printf ("30\n");
  printf ("0.0\n");
  printf ("9\n");
  printf ("$EXTMIN\n");
  printf ("10\n");
  printf ("0\n");
  printf ("20\n");
  printf ("0\n");
  printf ("30\n");
  printf ("0\n");
  printf ("9\n");
  printf ("$EXTMAX\n");
  printf ("10\n");
  printf ("0.001\n");
  printf ("20\n");
  printf ("0\n");
  printf ("30\n");
  printf ("0\n");
  printf ("9\n");
  printf ("$LIMMIN\n");
  printf ("10\n");
  printf ("-0.00025\n");
  printf ("20\n");
  printf ("0\n");
  printf ("9\n");
  printf ("$LIMMAX\n");
  printf ("10\n");
  printf ("0.00125\n");
  printf ("20\n");
  printf ("0\n");
  printf ("9\n");
  printf ("$CLAYER\n");
  printf ("8\n");
  printf ("Z88NET\n");
  printf ("9\n");
  printf ("$PDMODE\n");
  printf ("70\n");
  printf ("3\n");
  printf ("9\n");
  printf ("$PDSIZE\n");
  printf ("40\n");
  printf ("-3.0\n");
  printf ("0\n");
  printf ("ENDSEC\n");
  printf ("0\n");
  printf ("SECTION\n");
  printf ("2\n");
  printf ("TABLES\n");
  printf ("0\n");
  printf ("TABLE\n");
  printf ("2\n");
  printf ("LTYPE\n");
  printf ("70\n");
  printf ("1\n");
  printf ("0\n");
  printf ("LTYPE\n");
  printf ("2\n");
  printf ("CONTINUOUS\n");
  printf ("70\n");
  printf ("64\n");
  printf ("3\n");
  printf ("Solid line\n");
  printf ("72\n");
  printf ("65\n");
  printf ("73\n");
  printf ("0\n");
  printf ("40\n");
  printf ("0.0\n");
  printf ("0\n");
  printf ("ENDTAB\n");
  printf ("0\n");
  printf ("TABLE\n");
  printf ("2\n");
  printf ("LAYER\n");
  printf ("70\n");
  printf ("6\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("0\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("-7\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("Z88NET\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("5\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("Z88EIO\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("5\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("Z88KNR\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("1\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("Z88RBD\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("6\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("Z88FLA\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("6\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("Z88GEN\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("6\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("LAYER\n");
  printf ("2\n");
  printf ("Z88PKT\n");
  printf ("70\n");
  printf ("64\n");
  printf ("62\n");
  printf ("1\n");
  printf ("6\n");
  printf ("CONTINUOUS\n");
  printf ("0\n");
  printf ("ENDTAB\n");
  printf ("0\n");
  printf ("ENDSEC\n");
  printf ("0\n");
  printf ("SECTION\n");
  printf ("2\n");
  printf ("ENTITIES\n");
  printf ("0\n");

}


void
Z88printDXFtext (char *text, double x0, double y0, double h)
{

  printf ("TEXT\n");
  printf ("8\n");
  printf ("Z88KNR\n");


  printf (" 10\n");
  printf ("%f\n", x0);
  printf (" 20\n");
  printf ("%f\n", y0);
  printf (" 30\n");
  printf ("0.0\n");
  printf (" 40\n");
  printf ("%f\n", h);


  printf (" 1\n");
  printf ("%s\n", text);

  printf (" 0\n");

}



void
Z88printLine (double x0, double y0, double x1, double y1)
{
  printf ("LINE\n");
  printf ("8\n");
  printf ("Z88NET\n");

  printf (" 10\n");
  printf ("%f\n", x0);

  printf (" 20\n");
  printf ("%f\n", y0);

  printf (" 30\n");
  printf ("0.0\n");

  printf (" 11\n");
  printf ("%f\n", x1);

  printf (" 21\n");
  printf ("%f\n", y1);

  printf (" 31\n");
  printf ("0.0\n");



  printf (" 0\n");

}

void
Z88printPoint (double x0, double y0)
{
  printf ("POINT\n");
  printf ("8\n");
  printf ("Z88PKT\n");

  printf ("10\n");
  printf ("%f\n", x0);

  printf ("20\n");
  printf ("%f\n", y0);

  printf ("30\n");
  printf ("0.0\n");

  printf ("0\n");

}
