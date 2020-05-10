/*
DXF reference information found at:
http://paulbourke.net/dataformats/dxf/dxf10.html


SOLID     Four points defining the corners of the solid: (10, 20, 30),
            (11, 21, 31), (12, 22, 32), and (13, 23, 33).  If only three
            points were entered (forming a triangular solid), the third
            and fourth points will be the same.


*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tydxfout.h"
#include "tyMems1.h"

double degToRad = 0.017453292;





void
print_wgs_to_OS ()
{


  struct LATLONG latlong;
  struct EN en;
  struct EN en1;

  latlong.Lat = -90.0;
  latlong.Long = -90.0;

  int cdiv = 18;		//keep it even

  printDXFheader ();

  for (int j = 0; j < cdiv + 1; j++)
    {

      for (int i = 0; i < cdiv + 1; i++)
	{
	  en = latlong_to_EN (latlong);
	  printCirle ("WGSpoints", en.E, en.N, 1e4, 1);
	  latlong.Lat += (double) (180 / cdiv);

	}
      latlong.Lat = -90.0;
      latlong.Long += (double) (180 / cdiv);
    }

  printMap ();





  latlong.Lat = 0.0;
  latlong.Long = 90.0;
  en = latlong_to_EN (latlong);

  latlong.Lat = 0.0;
  latlong.Long = -90.0;
  en1 = latlong_to_EN (latlong);
  printLine ("Equator", en.E, en.N, en1.E, en1.N, 7);



//print circles of lattitude
  latlong.Lat = -90.0;

  for (int j = 0; j < cdiv + 1; j++)
    {

      latlong.Long = -90.0;
      en = latlong_to_EN (latlong);

      for (int i = 0; i < cdiv; i++)
	{
	  latlong.Long += (double) (180 / cdiv);
	  en1 = latlong_to_EN (latlong);
	  if (j == (int) cdiv / 2)
	    printLine ("Equator", en.E, en.N, en1.E, en1.N, 7);
	  else
	    printLine ("Lattitudes", en.E, en.N, en1.E, en1.N, 8);

	  en = latlong_to_EN (latlong);


	}
      latlong.Lat += (double) (180 / cdiv);
    }












//print merdians
  latlong.Long = -90.0;

  for (int j = 0; j < cdiv + 1; j++)
    {

      latlong.Lat = -90.0;
      en = latlong_to_EN (latlong);

      for (int i = 0; i < cdiv; i++)
	{
	  latlong.Lat += (double) (180 / cdiv);
	  en1 = latlong_to_EN (latlong);

	  if (j == (int) cdiv / 2)
	    printLine ("GreenwhichMerdian", en.E, en.N, en1.E, en1.N, 7);
	  else
	    printLine ("Merdians", en.E, en.N, en1.E, en1.N, 8);

	  en = latlong_to_EN (latlong);


	}
      latlong.Long += (double) (180 / cdiv);
    }










/*
The false origin is 400 km west and 100 km north of the ‘true
origin’ on the central meridian at 49°N 2°W.
*/
  latlong.Lat = 49.766809;
  latlong.Long = -7.5571598;
  en = latlong_to_EN (latlong);
  printCirle ("OS_FO", en.E, en.N, 1e4, 3);

  latlong.Lat = 49.0;
  latlong.Long = -2.0;
  en = latlong_to_EN (latlong);
  printCirle ("OS_TO", en.E, en.N, 1e4, 3);




  printDXFfooter ();

}



struct EN
latlong_to_EN (struct LATLONG LatLong)
{


  double EarthSemiAxis_a = 6377563.396;
  double EarthSemiAxis_b = 6356256.909;
  double E0 = 400000.0;
  double N0 = -100000;
  double F0 = 0.9996012717;
  double lambda0 = -2.0 * degToRad;
  double phi0 = 49.0 * degToRad;	// Projection

  double phi = LatLong.Lat * degToRad;
  double lambda = LatLong.Long * degToRad;

  double n, e2, v, rho, neta2, M, I, II, III, IIIA, IV, V, VI, N, E;

  n =
    (EarthSemiAxis_a - EarthSemiAxis_b) / (EarthSemiAxis_a + EarthSemiAxis_b);

  e2 =
    (EarthSemiAxis_a * EarthSemiAxis_a -
     EarthSemiAxis_b * EarthSemiAxis_b) / (EarthSemiAxis_a * EarthSemiAxis_a);

  v = (EarthSemiAxis_a * F0) / sqrt (1.0 - e2 * sin (phi) * sin (phi));

  rho =
    ((EarthSemiAxis_a * F0) * (1 - e2)) / pow (1 - e2 * sin (phi) * sin (phi),
					       1.5);

  neta2 = (v / rho) - 1.0;



  double n2 = n * n;
  double n3 = n * n * n;
  double cos3phi = cos (phi) * cos (phi) * cos (phi);


  M = EarthSemiAxis_b * F0 * ((1.0 + n + (5.0 / 4.0) * n2 +
			       (5.0 / 4.0) * n3) * (phi - phi0) - ((3.0 * n +
								    3.0 * n2 +
								    (21.0 /
								     8.0) *
								    n3)) *
			      sin (phi - phi0) * cos (phi + phi0) +
			      ((15.0 / 8.0) * n2 +
			       (15.0 / 8.0) * n * n * n) * sin (2.0 * (phi -
								       phi0))
			      * cos (2.0 * (phi + phi0)) -
			      ((35.0 / 24.0) * n3) * sin (3.0 *
							  (phi -
							   phi0)) * cos (3.0 *
									 (phi
									  +
									  phi0)));

  I = M + N0;
  II = (v / 2.0) * sin (phi) * cos (phi);

  double tan2phi = tan (phi) * tan (phi);

  III = (v / 24.0) * sin (phi) * cos3phi * (5.0 - tan2phi + 9.0 * neta2);

  double cos5phi = cos (phi) * cos (phi) * cos (phi) * cos (phi) * cos (phi);
  double tan4phi = tan (phi) * tan (phi) * tan (phi) * tan (phi);

  IIIA =
    (v / 720.0) * sin (phi) * cos5phi * (61.0 - 58.0 * tan2phi + tan4phi);

  IV = v * cos (phi);


  V = (v / 6.0) * cos3phi * ((v / rho) - tan2phi);

  VI = (v / 120.0) * cos5phi *
    (5.0 - 18.0 * tan2phi + tan4phi + 14.0 * neta2 - 58.0 * neta2 * tan2phi);


  double lambdaLessLambda02 = (lambda - lambda0) * (lambda - lambda0);
  double lambdaLessLambda03 = lambdaLessLambda02 * (lambda - lambda0);
//double lambdaLessLambda04 = lambdaLessLambda02*lambdaLessLambda02;
  double lambdaLessLambda05 =
    lambdaLessLambda02 * lambdaLessLambda02 * (lambda - lambda0);

  N = I +
    II * lambdaLessLambda02 +
    III * lambdaLessLambda03 + IIIA * lambdaLessLambda05;



  E = E0 +
    IV * (lambda - lambda0) +
    V * lambdaLessLambda03 + VI * lambdaLessLambda05;

/*
printf(" Lat=%f (%f)\n Lon=%f (%f)\n n=%f \n e2=%f\n v=%f\n rho=%f\n neta2=%f\n M=%f\n I=%f\n II=%f\n III=%f\n IIIA=%f\n IV=%f\n V=%f\n VI=%f\n N=%f\n E=%f\n", 
LatLong.Lat,phi, LatLong.Long,lambda, n,e2,v,rho,neta2,M,I,II,III,IIIA,IV,V,VI,N,E);
*/


  struct EN en;
  en.N = N;
  en.E = E;
  return en;



}



void
printblankDXF ()
{
  printDXFheader ();
  printDXFfooter ();
}




void
printMapwithDXFheadAndFoot ()
{
//write out dxf data
  printDXFheader ();
  printMap ();
  printDXFfooter ();
}


void
printMapLatLong (char *countryname)
{

  readinLatLong (countryname);

}

void
readinLatLong (char *filename)
{

//printf("%s\n",filename);



  FILE *file;
  char c;

  file = fopen (filename, "r");
//double x;
//double y;

  char sNum[30];
  int q = 0;

  double dx = 0;
  double dy = 0;
//int flip = 0;

  if (file)
    {

 
      while (c != EOF || (int)c == 255) 
	{
	  c = getc (file);
	  if (c == EOF || (int)c == 255)
	    break;
	  if (c != ',' && c != ' ')
	    {
	      sNum[q] = c;
	      q++;
	    }

	  if (c == ',')
	    {
	      dy = atof (sNum);
	      q = 0;
	      sNum[q] = '\0';
	    }

	  if (c == '\n')
	    {
	      dx = atof (sNum);
	      //printf("%f ---- %f\n",dx,dy);
	      struct EN en;
	      struct LATLONG LLo;
	      LLo.Lat = dy;
	      LLo.Long = dx;
	      en = latlong_to_EN (LLo);

	      printPloyLineVertex ("tyinput", en.E, en.N, 0.0);
	      q = 0;
	      sNum[q] = '\0';
	    }



	}


      fclose (file);
    }




}


void
printMap ()
{


//read in data
  createLineList ();

  drawOSGrid ();







  printCirle ("London", 538966.0, 177334.0, 8.9e6 / 100, 7);
  printCirle ("Lincoln", 497796.0, 371810.0, 756000 / 100.0, 2);
  printCirle ("Louth", 532643.0, 387383.0, 20000.0 / 100.0, 2);
  printCirle ("Birmingham", 406689, 286822, 1.086e6 / 100.0, 3);
  printCirle ("Cardiff", 318371.0, 176329, 335000.0 / 100.0, 4);
  printCirle ("Glasgow", 260107.0, 665646, 600000.0 / 100.0, 5);
  printCirle ("Aberdeen", 425678.0, 554270.0, 208000.0 / 100.0, 6);


//Print the line list/////////////////////////////////////////////////
  t = head->next;		//assume head is a dummy node

  while (t != z)
    {

      struct tyLin mPtr;
      mPtr = *(*t).m;
      printLine ("tyLinesList", mPtr.x0, mPtr.y0, mPtr.x1, mPtr.y1,
		 mPtr.colour);
      t = t->next;
    }


//Print the Polylines

  printPolyLineHeader ("UKcoast", 2, 1);



//read in map coords

  char *filename;
  filename = "input/UKmapData.csv";

  FILE *file;
  char c;

  file = fopen (filename, "r");


  char sNum[30];
  int q = 0;

  double dx = 0;
  double dy = 0;


  if (file)
    {


      while (c != EOF || (int)c != 255)
	{
	  
	  c = getc (file);
	 // printf("%c %d\n",c,(int)c);
	  
	  if(c == EOF || (int)c == 255)
	    break;
	  
	  if (c != ',' && c != ' ')
	    {
	      sNum[q] = c;
	      q++;
	    }

	  if (c == ',')
	    {
	      dx = atof (sNum);
	      q = 0;
	      //sNum[q] = '\0';
	       memset (sNum, 0, sizeof sNum);
	    }

	  if (c == '\n')
	    {
	      dy = atof (sNum);
	      //printf("%f ---- %f\n",dx,dy);
	      printPloyLineVertex ("tyPoly", dx, dy, 0.0);
	      q = 0;
	      //sNum[q] = '\0';
	       memset (sNum, 0, sizeof sNum);
	    }



	}

        
      fclose (file);
    }

  // printf(" debug here1\n");


  printPolyLineFooter ();



  printPolyLineHeader ("EireNI", 3, 1);;
  printMapLatLong ("input/eire.csv");
  printPolyLineFooter ();


  freeLinesList ();

}



void
printWedgey (char *layername, int colour, double x, double y, double r,
	     double StartAng, double incAng)
{

  printPolyLineHeader ("Nightingale", colour, 1);
  double degtorad = 0.017453292;
  double B;
  B = tan (incAng * degtorad * 0.25);
  printPloyLineVertex ("Nightingale", 0, 0, 0.0);
  printPloyLineVertexBulge ("Nightingale", r * cos (StartAng * degtorad),
			    r * sin (StartAng * degtorad), 0.0, B);
  printPloyLineVertex ("Nightingale",
		       r * cos ((StartAng + incAng) * degtorad),
		       r * sin ((StartAng + incAng) * degtorad), 0.0);
  printPolyLineFooter ();
}



void
printArc (char *layername, int colour, double x, double y, double r,
	  double StartAng, double incAng)
{
//ARC       10, 20, 30 (center), 40 (radius), 50 (start angle), 51 (endangle).

  printf ("ARC\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("62\n");
  printf ("%d\n", colour);
  printf ("10\n");
  printf ("%f\n", x);
  printf ("20\n");
  printf ("%f\n", y);
  printf ("30\n");
  printf ("0\n");
  printf ("40\n");
  printf ("%f\n", r);
  printf ("50\n");
  printf ("%f\n", StartAng);
  printf ("51\n");
  printf ("%f\n", StartAng + incAng);
  printf ("0\n");
}






/*
void printCirleList(char *layername){
}
*/

void
printCirle (char *layername, double cx, double cy, double R, int colour)
{
  printf ("CIRCLE\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("62\n");
  printf ("%d\n", colour);
  printf ("10\n");
  printf ("%f\n", cx);
  printf ("20\n");
  printf ("%f\n", cy);
  printf ("30\n");
  printf ("0\n");
  printf ("40\n");
  printf ("%f\n", R);
  printf ("0\n");
}

void
printLine (char *layername, double x0, double y0, double x1, double y1,
	   int colour)
{
  printf ("LINE\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("62\n");
  printf ("%d\n", colour);
  printf ("10\n");
  printf ("%f\n", x0);
  printf ("20\n");
  printf ("%f\n", y0);
  printf ("30\n");
  printf ("0\n");
  printf ("11\n");
  printf ("%f\n", x1);
  printf ("21\n");
  printf ("%f\n", y1);
  printf ("31\n");
  printf ("0\n");
  printf ("0\n");
}



void
printPolyLineHeader (char *layername, int colour, int closed)
{
  printf ("POLYLINE\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("70\n");
  printf ("%d\n", closed);	//closed 1 open 0
  printf ("66\n");
  printf ("1\n");
  printf ("62\n");
  printf ("%d\n", colour);
  printf ("0\n");
}



void
printPolyLineFooter ()
{
  printf ("SEQEND\n");
  printf ("0\n");
}

void
printPloyLineVertex (char *layername, double x, double y, double z)
{
  printf ("VERTEX\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("10\n");
  printf ("%f\n", x);
  printf ("20\n");
  printf ("%f\n", y);
  printf ("30\n");
  printf ("%f\n", z);
  printf ("0\n");
}

void
printPloyLineVertexBulge (char *layername, double x, double y, double z,
			  double bulge)
{
/*50 (curve fit tangent direction
            -optional).  The bulge is the tangent of 1/4 the included
            angle for an arc segment, made negative if the arc goes
            clockwise from the start point to the end point; a bulge of 0
            indicates a straight segment, and a bulge of 1 is a semicir-
            cle.
*/

  printf ("VERTEX\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("10\n");
  printf ("%f\n", x);
  printf ("20\n");
  printf ("%f\n", y);
  printf ("30\n");
  printf ("%f\n", z);
  printf ("42\n");
  printf ("%f\n", bulge);
  printf ("0\n");



}
















void
printDXFfooter ()
{
  printf ("ENDSEC\n");
  printf ("0\n");
  printf ("EOF\n");
}


void
printDXFheader ()
{

  printf ("999\n");
  printf ("Simple DXF by T Harness\n");
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
  printf ("-1000.0\n");
  printf ("20\n");
  printf ("-1000.0\n");
  printf ("9\n");
  printf ("$EXTMAX\n");
  printf ("10\n");
  printf ("1000.0\n");
  printf ("20\n");
  printf ("1000.0\n");
  printf ("0\n");
  printf ("ENDSEC\n");
  printf ("0\n");
  printf ("SECTION\n");
  printf ("2\n");
  printf ("TABLES\n");
  printf ("0\n");
  printf ("ENDSEC\n");
  printf ("0\n");
  printf ("SECTION\n");
  printf ("2\n");
  printf ("BLOCKS\n");
  printf ("0\n");
  printf ("ENDSEC\n");
  printf ("0\n");
  printf ("SECTION\n");
  printf ("2\n");
  printf ("ENTITIES\n");
  printf ("0\n");


}








/*                                                DRAW OSGB grid six fig reference */
void
drawOSGrid ()
{

  t = head;
  int dx = 0;
  int dy = 0;

  struct tyLin xm1;
  for (int j = 0; j < 11; j++)
    {
      for (int i = 0; i < 8; i++)
	{
	  if (j < 10)
	    {
	      xm1.key = i + 1;
	      xm1.x0 = dx;
	      xm1.y0 = dy;
	      xm1.x1 = dx;
	      xm1.y1 = dy + 1e5;
	      xm1.colour = 8;
	      t = insertLineafter (xm1, t);
	    }
	  if (i < 7)
	    {
	      xm1.key = i + 1;
	      xm1.x0 = dx;
	      xm1.y0 = dy;
	      xm1.x1 = dx + 1e5;
	      xm1.y1 = dy;
	      xm1.colour = 8;
	      t = insertLineafter (xm1, t);
	    }
	  dx += 1e5;
	}
      dy += 1e5;
      dx = 0;
    }

}



/* 								Experimental/Ancillary functions*/
void
drawTest ()
{

  t = head;


  struct tyLin xm1;

  for (int i = 0; i < 9; i++)
    {

      xm1.key = i + 1;
      xm1.x0 = (double) rand () / RAND_MAX * 2.0 - 1.0;
      xm1.y0 = (double) rand () / RAND_MAX * 2.0 - 1.0;
      xm1.x1 = (double) rand () / RAND_MAX * 2.0 - 1.0;
      xm1.y1 = (double) rand () / RAND_MAX * 2.0 - 1.0;

      t = insertLineafter (xm1, t);
    }
//int n = NLines();
//printf("Number of lines = %d \n\n", n );

}

void
generateNightingaleExample (int N)
{

  printDXFheader ();


  FILE *file;
  char c, c1;
  double cdiv = 0;		//number of divisions in a whole circle 360 deg
  int nData = 0;
  double ang = 0;


  file = fopen ("input/coviddataUK.csv", "r");


  char sNum[30];
  int q = 0;

  int dx = 0;
  int dy = 0;

  double dxold = 0.0;
  double dyold = 0.0;

  if (file)
    {



      while (c != EOF || (int)c != 255)
	{
	  c = getc (file);
	  if (c == EOF || (int)c == 255)
	    break;
	  if (c == '\n')
	    nData++;
	}




      fseek (file, 0, SEEK_SET);


      if (N > 0)
	cdiv = 360.0 / (double) N;
      else
	cdiv = 360.0 / (double) nData;




//printf("%d %f\n",nData,cdiv);


      int i = 0;

      while (c1 != EOF || (int)c1 != 255)
	{

	  c1 = getc (file);
	  if (c1 == EOF || (int)c1 == 255)
	    break;
	  if (c1 != ',' && c1 != ' ')
	    {
	      sNum[q] = c1;
	      q++;
	    }

	  if (c1 == ',')
	    {
	      dx = atoi (sNum);
	      q = 0;
	      //sNum[0] = '\0';
	      memset (sNum, 0, sizeof sNum);
	    }

	  if (c1 == '\n')
	    {
	      dy = atoi (sNum);
	      //  printf("%d ---- %d---- %f\n",dx,dy,(double)dx/(double)dy );

	      printWedgey ("Nightingale", 1, 0, 0,
			   ((double) dx / (double) dy) * 100.0, ang, cdiv);



	      if (i != 0)
		{
		  printWedgey ("Nightingale1", 5, 0, 0,
			       ((double) dx - dxold) / ((double) dy - dyold -
							7.0) * 100.0, ang,
			       cdiv);
		}
	      dxold = (double) dx;
	      dyold = (double) dy;
	      i++;



	      ang += cdiv;




	      q = 0;
	      //sNum[0] = '\0';
	      memset (sNum, 0, sizeof sNum);
	    }



	}


      fclose (file);
    }





/*
  double ang = 0;
  for (int i = 0; i < nData; i++)
    {
      printWedgey ("Nightingale", i + 1, 0, 0, (r[0][i] / r[1][i]) * 100, ang,
		   360 / cdiv);
      //printf("%f   %f   %f\n",r[0][i], r[1][i],  (r[0][i] / r[1][i])*100);
      if (i > 0)
	printWedgey ("Nightingale1", i + 1, 0, 0,
		     (r[0][i] - r[0][i - 1]) / (r[0][i] - r[0][i - 1] -
						7) * 100, ang, 360 / cdiv);
      ang += 360 / cdiv;
    }




*/


  printDXFfooter ();


}





void
generateFibonacciExample ()
{


  printDXFheader ();

  double cdiv = 8;


  double ang = 0;
  double dr0 = 0;
  double dr1 = 1e4;
  double dr10 = dr0 + dr1;

  for (int i = 0; i < cdiv; i++)
    {

      printWedgey ("Nightingale", i, 0, 0, dr10, ang, 360 / cdiv);
      ang += 360 / cdiv;
      dr10 = dr0 + dr1;
      dr0 = dr1;
      dr1 = dr10;

    }


  printDXFfooter ();

}






void
convert_coviddataUK_to_COVID19dataUK (int mon, int year)
{

/*30 04 20,26771/171253 = 0.156 (15.6%),674/(6032-7) = 0.112(11.2%),?
date,deaths/  , deaths/ -7 , ?
*/



  FILE *file;
  char c1;





  file = fopen ("input/coviddataUK.csv", "r");


  char sNum[30];
  int q = 0;

  int dx = 0;
  int dy = 0;

  double dxold = 0.0;
  double dyold = 0.0;

  if (file)
    {

      int i = 0;

      while (c1 != EOF || (int)c1 != 255)
	{

	  c1 = getc (file);
	  if (c1 == EOF || (int)c1 == 255)
	    break;
	  if (c1 != ',' && c1 != ' ')
	    {
	      sNum[q] = c1;
	      q++;
	    }

	  if (c1 == ',')
	    {
	      dx = atoi (sNum);
	      q = 0;
	      //sNum[0] = '\0';
	      memset (sNum, 0, sizeof sNum);
	    }

	  if (c1 == '\n')
	    {
	      dy = atoi (sNum);
	      double cfr = (double) dx / (double) dy;
	      printf ("%02d/%02d/%0d,%d/%d=%.3f[%.1f%%]", i + 1, mon, year,
		      dx, dy, cfr, cfr * 100.0);

	      if (i != 0)
		{
		  double cfr1 =
		    ((double) dx - dxold) / ((double) dy - dyold - 7.0);
		  printf (",%.0f/[%.0f-7]=%.3f[%.1f],?",
			  ((double) dx - dxold), ((double) dy - dyold), cfr1,
			  (cfr1 * 100.0));
		}
	      else
		printf (",?,?");

	      printf ("\n");
	      dxold = (double) dx;
	      dyold = (double) dy;
	      i++;








	      q = 0;
	      //sNum[0] = '\0';
	      memset (sNum, 0, sizeof sNum);
	    }



	}


      fclose (file);
    }



}





void
generateHTMLtablefromCSVtest ()
{
  printf ("<table border='2'>\n");
  printf
    ("<tr><td>UK</td> <td> CFR(T=0)</td> <td> CFR(T=7)</td> <td>ONS registered deaths<br>(increase on 5 year avg.)</td> </tr>");

  char *filename;
  filename = "input/COVID19dataUK.csv";

  FILE *file;
  char c;

  file = fopen (filename, "r");

  int q = 0;
  char col[4][30];

  int cinc = 0;


  if (file)
    {
      while (c != EOF || (int)c != 255)
	{
	  c = getc (file);
	  if (c == EOF || (int)c == 255)
	    break;

	  if (c != ',')
	    {
	      col[cinc][q] = c;
	      q++;
	    }


	  if (c == ',')
	    {
	      col[cinc][q] = '\0';
	      cinc++;
	      q = 0;
	    }


	  if (c == '\n')
	    {
	      col[cinc][q] = '\0';
	      //printf("%s  ----  %s  ----  %s  ----  %s",col[0],col[1],col[2],col[3]);

	      printf
		("<tr><td>%s</td><td>  %s </p> </td> <td> %s </td><td> %s </td></tr> ",
		 col[0], col[1], col[2], col[3]);

	      q = 0;
	      cinc = 0;
	    }
	}
      fclose (file);
    }

  printf ("</table>\n");


  printf ("</table>\n");

}


void
createLineList ()
{

  head = (struct tyLine *) malloc (sizeof (struct tyLine));	//will need freeing eventually
  z = NULL;

  head->m = NULL;		//perhaps wont need to free m
  head->next = z;


}


void
freeLinesList ()
{

  struct tyLine *x;
  struct tyLin *xPtr;


  t = head->next;

  while (t != z)
    {
      x = t;
      xPtr = (*x).m;

      t = t->next;

      free (xPtr);
      free (x);

    }

  free (t);
  free (head);

}



struct tyLine *
insertLineafter (struct tyLin tm, struct tyLine *t)
{


  struct tyLine *x;
  x = (struct tyLine *) malloc (sizeof (struct tyLine));	//create a new move x

  struct tyLin *xm;
  xm = (struct tyLin *) malloc (sizeof (struct tyLin));

  xm->key = tm.key;
  xm->x0 = tm.x0;
  xm->y0 = tm.y0;
  xm->x1 = tm.x1;
  xm->y1 = tm.y1;
  xm->colour = tm.colour;

  x->m = xm;
  x->next = t->next;		//copy t's pointer into new move
  t->next = x;			//set t next pointer to new move x



  return x;


}



int
NLines ()
{
  int n = 0;
  t = head->next;		//assume head is a dummy node
  while (t != z)
    {
      n++;
      t = t->next;
    }

  return n;
}





void
memsDeviceDXF ()
{



  //write out dxf data
  printDXFheader ();



  printPolyLineHeader ("mems1e", 3, 1);


//read in the polyline

  char *filename;
  filename = "input/mems1e.csv";

  FILE *file;
  char c;

  file = fopen (filename, "r");


  char sNum[30];
  int q = 0;

  double dx = 0.0;
  double dy = 0.0;
  //double area = 0.0;

  if (file)
    {


      while (c != EOF || (int)c != 255)
	{
	  c = getc (file);
	  if (c == EOF || (int)c == 255)
	    break;
	  if (c != ',' && c != ' ')
	    {
	      sNum[q] = c;
	      q++;
	    }

	  if (c == ',')
	    {
	      dx = atof (sNum);
	      q = 0;
	      sNum[q] = '\0';
	    }

	  if (c == '\n')
	    {
	      dy = atof (sNum);
	      //printf("%f ---- %f\n",dx,dy);
	      printPloyLineVertex ("mems1e", dx, dy, 0.0);
	      q = 0;
	      sNum[q] = '\0';
	    }



	}


      fclose (file);
    }

  printPolyLineFooter ();


  /*
  dx = -110.0; dy=10.0;


  for (int i = 0;i <20;i++){  
  printPolyLineHeader ("mems1e_holes", 2, 1);
 
   
  printPloyLineVertex ("mems1e_holes", dx, dy, 0.0);
  printPloyLineVertex ("mems1e_holes", dx+40.0, dy, 0.0);
  printPloyLineVertex ("mems1e_holes", dx+40.0, dy+100.0, 0.0);

  printPloyLineVertex ("mems1e_holes", dx, dy, 0.0);
  printPloyLineVertex ("mems1e_holes", dx+40.0, dy, 0.0);
  printPloyLineVertex ("mems1e_holes", dx+40.0, dy+100.0, 0.0);


  printPolyLineFooter ();
  dy += 110.0;
}
*/

  printDXFfooter ();



}





/*
//copy textdata from another file

char *filename;

filename = "ukmapdat.dxf";

FILE *file;
char c;

file = fopen(filename, "r");

if (file) 
    { 
     while (c != EOF) 
        { 
        c = getc(file);
         if (c==EOF) break;
        putchar(c); 
        } 
//printf("\n");
fclose(file);
}

*/
