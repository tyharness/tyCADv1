/*
 * 
A great DXF reference source can be found at:
http://paulbourke.net/dataformats/dxf/dxf10.html


//havent got my head around hatching yet
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
#include "tyMath.h"
#include "tyCADv1.h"


double degToRad = 0.017453292;
double Pi = 3.141592664;



void
printWorldMapExample ()
{
  printDXFheader ();
  print_lat_long_projection ();
  printDXFfooter ();
}


/*I use this as quick way of testing functions*/
void
printblankDXF ()
{
  printDXFheader ();

/*testing functions*/

//void printDXFtext(char *text, char *layername, int colour, double x0, double y0, double h, double rot, int justify);
//printDXFtext ("testing", "textL", 1, 100.0, 100.0, 25.0, 45.0, 0);
//print_xyAxes("axes",7,-200.0,300.0,-100.0,500.0,10,12);

  // print_lat_long_cylindrical_projection();
  print_lat_long_projection ();

  printDXFfooter ();
}


void
print_xy_graph (char *filename, char *Title, char *legendX, char *legendY,
		char *layername, int colour, int mode)
{


  printDXFheader ();


/*go thorough csv data get max mins 
	
	* mode) plot either xdata,ydata or
	* 0) x independent, x data
	* 1) y independent, y data
	* 
	* would be better to take a more flexible approach so you can plot any or multiple columns of data
	* against each other.  	

*/

  FILE *file;

  file = fopen (filename, "r");


  char strText[30];



  char c;
  char c1;

  char sNum[30];
  memset (sNum, 0, sizeof sNum);

  int q = 0;

  double x = 0.0;
  double y = 0.0;

  double xold = 0.0;
  double yold = 0.0;


  int nData = 0;




  if (file)
    {



      //get the first x and y coords

      //get the max and min xy data

      double xmax = 0.0;
      double ymax = 0.0;

      double xmin = 0.0;
      double ymin = 0.0;







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
	      x = atof (sNum);
	      if (mode == 0)
		y = atof (sNum);
	      q = 0;
	      memset (sNum, 0, sizeof sNum);




	    }

	  if (c == '\n')
	    {
	      nData++;
	      if (mode == 1 || mode == 2)
		y = atof (sNum);



	      q = 0;
	      memset (sNum, 0, sizeof sNum);


	      // mode 2 plotting col1,col2
	      if (x < xmin)
		xmin = x;
	      if (x > xmax)
		xmax = x;
	      if (y < ymin)
		ymin = y;
	      if (y > ymax)
		ymax = y;

	    }

	}

      printf ("999\n");
      printf
	("nData %d,  mode %d, xmin %f, ymin %f, ymin %f, ymax %f\n",
	 nData, mode, xmin, ymin, xmax, ymax);


      //part2



      //option linear independent x axis not related to  
      if (mode < 0 || mode > 2)
	mode = 2;

      if (mode == 0 || mode == 1)
	{
	  xmin = 0.0;
	  xmax = (double) nData;
	}





      if (mode == 2)
	{
	  xmin = ymin;
	  xmax = ymax;
	}





      //Based roughly on fitting on A4 210mm, 297mm  paper at 1:1 for printing
      double SX = 180.0;
      double scaleX = SX / (xmax - xmin);
      double SY = 250.0;
      double scaleY = SY / (ymax - ymin);




      double Pr = 0.5;		//point radius   
      double scaleP;

      //point radius scaled to graph
      if (scaleX >= scaleY)
	scaleP = Pr * scaleX;
      else
	scaleP = Pr * scaleY;


      double th;

      if (scaleX >= scaleY)
	th = 3.0 * scaleX;
      else
	th = 3.0 * scaleY;

      printf ("999\n");
      printf
	("SX %f ,SY %f, scaleX %f, scaleY %f, scaleP %f,th %f\n ", SX, SY,
	 scaleX, scaleY, scaleP, th);

      printf ("999\n");
      printf
	("Scaled xmin %f, Scaled xmax %f, Scaled ymin %f, Scaled ymax %f\n",
	 xmin * scaleX, xmax * scaleX, ymin * scaleY, ymax * scaleY);


      //draw the axes
      print_xyAxes (layername, 1, xmin * scaleX, xmax * scaleX, ymin * scaleY,
		    ymax * scaleY, 7, 7, scaleX, scaleY);

      printDXFtext (legendX, "LegendXY", 1, (xmax + xmin) * 0.5 * scaleX, -5.0 * th, th, 0.0, 0);	//mid just
      printDXFtext (legendY, "LegendXY", 1, -6.0 * th, (ymax + ymin) * 0.5 * scaleY, th, 90.0, 0);	//mid just
      printDXFtext (Title, "LegendXY", 1, 2.0 * th, ymax * scaleY - 2.0 * th,
		    th, 0.0, 0);


      sprintf (strText, "Max: %.0f", ymax);
      printDXFtext (strText, "LegendXY", 3, 2.0 * th,
		    ymax * scaleY - 4.0 * th, th, 0.0, 0);
      memset (strText, 0, sizeof strText);


//go back through data and plot points

      fseek (file, 0, SEEK_SET);




      int i = 0;

      while (c1 != EOF || (int) c1 != 255)
	{

	  c1 = getc (file);
	  if (c1 == EOF || (int) c1 == 255)
	    break;
	  if (c1 != ',' && c1 != ' ')
	    {
	      sNum[q] = c1;
	      q++;
	    }

	  if (c1 == ',')
	    {

	      if (mode == 0 || mode == 1)
		x = (double) i;	// independent           

	      if (mode == 0)
		y = atof (sNum);	//pick the first col to plot on the y axis

	      if (mode == 2)
		x = atof (sNum);	//conventional x axis

	      q = 0;
	      memset (sNum, 0, sizeof sNum);
	    }

	  if (c1 == '\n')
	    {

	      if (mode == 1 || mode == 2)
		y = atof (sNum);





	      printf ("999\n");
	      printf ("--------%f, %f ----- %f, %f, %f, %f\n",
		      xold, yold, x, y, scaleX, scaleY);

	      if (i > 0)
		printLine (layername, xold * scaleX, yold * scaleY,
			   x * scaleX, y * scaleY, 3);

	      printCirle (layername, x * scaleX, y * scaleY, Pr * scaleP, 2);

	      xold = x;
	      yold = y;
	      i++;


	      q = 0;
	      memset (sNum, 0, sizeof sNum);


	    }
	  printf ("999\n");
	  printf ("File not read\n");


	}


      fclose (file);
    }
  else
    {



    }


  printDXFfooter ();

}



void
print_xyAxes (char *layername, int colour, double xmin, double xmax,
	      double ymin, double ymax, int divX, int divY, double oscaleX,
	      double oscaleY)
{


  printLine (layername, xmin, 0.0, xmax, 0.0, colour);	// x axis


  printLine (layername, 0.0, ymin, 0.0, ymax, colour);	// y yaxis



  char strText[30];
  double dx = (xmax - xmin) / (double) divX;
  double dy = (ymax - ymin) / (double) divY;

  double x = xmin;
  double y = ymin;

  for (int i = 0; i <= divX; i++)
    {
      printLine (layername, x, 0.0, x, -10.0, colour);

      sprintf (strText, "%.0f", x / oscaleX);
      printDXFtext (strText, layername, 1, x, y - 20.0, 3.0, 90.0, 0);
      memset (strText, 0, sizeof strText);

      x += dx;
    }



  for (int i = 0; i <= divY; i++)
    {
      printLine (layername, 0.0, y, -10.0, y, colour);

      sprintf (strText, "%.0f", y / oscaleY);
      printDXFtext (strText, layername, 1, -30.0, y + 2, 3.0, 0.0, 0);
      memset (strText, 0, sizeof strText);

      y += dy;
    }




}


void
print_lat_long_projection ()
{

/* DUmb Projection but easy to program and check*/

  int cdiv = 36;		//keep it even

  double dx = 360.0 / (double) cdiv;
  double dy = 360.0 / (double) cdiv;

  double x0 = -180.0;
  double y0 = -90.0;
  double x1 = x0 + dx;
  double y1 = -90.0;

  for (int i = 0; i <= (int) cdiv * 0.5; i++)
    {
      for (int j = 0; j < cdiv; j++)
	{
	  printLine ("Lattitudes", x0, y0, x1, y1, 8);
	  x0 = x1;
	  x1 += dx;
	}
      x0 = -180;
      x1 = x0 + dx;
      y0 += dy;
      y1 = y0;

    }


  x0 = -180.0;
  y0 = -90.0;
  x1 = x0;
  y1 = y0 + dy;

  for (int i = 0; i <= cdiv; i++)
    {
      for (int j = 0; j < (int) cdiv * 0.5; j++)
	{
	  printLine ("Longitudes", x0, y0, x1, y1, 8);
	  y0 = y1;
	  y1 += dy;
	}
      y0 = -90;
      y1 = y0 + dy;
      x0 += dx;
      x1 = x0;

    }


  printPolyLineHeader ("Africa", 4, 1);;
  readinLatLong1 ("input/Africa.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("England", 1, 1);;
  readinLatLong1 ("input/England.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Scotland", 5, 1);;
  readinLatLong1 ("input/Scotland.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Wales", 7, 1);;
  readinLatLong1 ("input/Wales.csv");
  printPolyLineFooter ();



  printPolyLineHeader ("Eire", 76, 1);;
  readinLatLong1 ("input/EireSI.csv");
  printPolyLineFooter ();



  printPolyLineHeader ("NI", 3, 1);;
  readinLatLong1 ("input/NI.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("IsleOfWight", 4, 1);
  readinLatLong1 ("input/IsleOfWight.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("IsleOfMan", 4, 1);
  readinLatLong1 ("input/IsleOfMan.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("OrkneyIsles", 4, 1);
  readinLatLong1 ("input/Orkney.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("ShetlandIsles", 4, 1);
  readinLatLong1 ("input/Shetland.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("USA", 4, 1);;
  readinLatLong1 ("input/usa.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("SouthAmerica", 4, 1);;
  readinLatLong1 ("input/SouthAmerica.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Mexico", 4, 1);;
  readinLatLong1 ("input/Mexico.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Australia", 4, 1);;
  readinLatLong1 ("input/Australia.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Greenland", 4, 1);;
  readinLatLong1 ("input/Greenland.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("Portugal", 4, 1);;
  readinLatLong1 ("input/Portugal.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("Spain", 4, 1);;
  readinLatLong1 ("input/Spain.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("France", 4, 1);;
  readinLatLong1 ("input/France.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("Antartica", 4, 1);
  readinLatLong1 ("input/Antartica.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("Artic", 4, 1);
  readinLatLong1 ("input/Artic.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Japan", 4, 1);
  readinLatLong1 ("input/Japan.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("Iceland", 4, 1);
  readinLatLong1 ("input/Iceland.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("SriLanka", 4, 1);
  readinLatLong1 ("input/SriLanka.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Canada", 4, 1);
  readinLatLong1 ("input/Canada.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("NewZealand", 4, 1);
  readinLatLong1 ("input/NewZealand.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("PapaNewGuinea", 4, 1);
  readinLatLong1 ("input/PapaNewGuinea.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("riw", 4, 1);
  readinLatLong1 ("input/riw.csv");
  printPolyLineFooter ();



}



void
print_lat_long_cylindrical_projection ()
{



  struct LATLONG latlong;
  struct XYZ xyz0;
  struct XYZ xyz1;





  int cdiv = 36;		//keep it even

  double dlong = 360.0 / (double) cdiv;
  double dlat = 360.0 / (double) cdiv;





  double startLong = -180.0;

  latlong.Long = startLong;
  latlong.Lat = 0.0;
  xyz0 = latlong_to_XYZ (latlong);

  for (int j = 0; j < cdiv; j++)
    {

      for (int i = 0; i < cdiv; i++)
	{
	  latlong.Long += dlong;
	  xyz1 = latlong_to_XYZ (latlong);

	  printLine ("LattitudesA", xyz0.x, xyz0.y, xyz1.x, xyz1.y, 1);
	  xyz0 = xyz1;
	}

      latlong.Lat += dlat;
      latlong.Long = startLong;
      xyz0 = latlong_to_XYZ (latlong);
    }



}




void
print_wgs_to_OS ()
{


  struct LATLONG latlong;
  struct EN en;
  struct EN en1;
  char strText[30];

  // double startLong = -180.0;
  double startLong = -14.0;
  double endLong = 10.0;

  //double startLat = -90.0;
  double startLat = 49.0;
  double endLat = 73.0;

  latlong.Lat = startLat;
  latlong.Long = startLong;


  //int cdiv = 36;              //keep it even
  int cdiv = 96;

  double dlong = (endLong - startLong) / (double) cdiv;
  double dlat = (endLat - startLat) / (double) cdiv;



  printDXFheader ();



/*

  for (int j = 0; j <= cdiv; j++)
    {

      for (int i = 0; i <= 2*cdiv; i++)
	{
	  en = latlong_to_EN (latlong);
	  printCirle ("WGSpoints", en.E, en.N, 1e4, 1);
	  latlong.Lat += (double) (360/ cdiv);

	}
      latlong.Lat = -90.0;
      latlong.Long += (double) (360 / cdiv);
    }


*/




/*
 * print out the individual country maps
 */


  printMap ();			//uk map in OSGB coordinates

/*
  printPolyLineHeader ("EireNI", 4, 1);;
  printMapLatLong ("input/eire.csv");
  printPolyLineFooter ();





  printPolyLineHeader ("Africa", 4, 1);;
  printMapLatLong ("input/Africa.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("USA", 4, 1);;
  printMapLatLong ("input/usa.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("SouthAmerica", 4, 1);;
  printMapLatLong ("input/SouthAmerica.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Mexico", 4, 1);;
  printMapLatLong ("input/Mexico.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Australia", 1, 1);;
  printMapLatLong ("input/Austraila.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Greenland", 4, 1);;
  printMapLatLong ("input/Greenland.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Antartica", 4, 1);
  printMapLatLong ("input/Antartica.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Thailand", 4, 1);;
  printMapLatLong ("input/Thailand.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Spain", 8, 1);;
  printMapLatLong ("input/Spain.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("Portugal", 8, 1);;
  printMapLatLong ("input/Portugal.csv");
  printPolyLineFooter ();

  latlong.Lat = startLat;
  latlong.Long = endLong;
  en = latlong_to_EN (latlong);

  latlong.Lat = startLat;
  latlong.Long = startLong;
  en1 = latlong_to_EN (latlong);
  printLine ("Equator", en.E, en.N, en1.E, en1.N, 1);

*/

/*
 * 
 * print circles of lattitude
  */


  latlong.Lat = startLat;
  latlong.Long = startLong;
  en = latlong_to_EN (latlong);


  for (int i = 0; i <= (int) cdiv * 0.5; i++)
    {

      for (int j = 0; j < cdiv; j++)
	{
	  latlong.Long += dlong;
	  en1 = latlong_to_EN (latlong);

	  /*if (i == (int) cdiv * 0.25)
	     printLine ("Equator", en.E, en.N, en1.E, en1.N, 11);
	     else
	     printLine ("Lattitudes", en.E, en.N, en1.E, en1.N, 11);

	   */
	  if (i % 4 == 0)
	    printLine ("Lattitudes", en.E, en.N, en1.E, en1.N, 11);
	  else
	    printLine ("Lattitudes", en.E, en.N, en1.E, en1.N, 8);




	  en = latlong_to_EN (latlong);

	}




      sprintf (strText, "  %.2f deg", latlong.Lat);
      printDXFtext (strText, "AiryText", 11, en1.E, en1.N, 0.8e4, 0.0, 0);





      latlong.Lat += dlat;
      latlong.Long = startLong;
      en = latlong_to_EN (latlong);
    }



  memset (strText, 0, sizeof strText);





//print merdians-----------------------------------------------------------------------
  latlong.Long = startLong;
  latlong.Lat = startLat;


  for (int i = 0; i <= cdiv; i++)
    {
      latlong.Lat = startLat;
      en = latlong_to_EN (latlong);

      for (int j = 0; j < (int) cdiv * 0.5; j++)
	{
	  latlong.Lat += dlat;
	  en1 = latlong_to_EN (latlong);

	  /*  if (i == (int) cdiv / 2)
	     printLine ("GreenwhichMerdian", en.E, en.N, en1.E, en1.N, 11);
	     else
	     printLine ("Merdians", en.E, en.N, en1.E, en1.N, 11);
	   */
	  if (i % 4 == 0)
	    printLine ("Merdians", en.E, en.N, en1.E, en1.N, 11);
	  else
	    printLine ("Merdians", en.E, en.N, en1.E, en1.N, 8);


	  en = latlong_to_EN (latlong);
	}




      sprintf (strText, "  %.2f deg", latlong.Long);
      printDXFtext (strText, "AiryText", 11, en1.E, en1.N, 0.8e4, 90.0, 0);


      latlong.Lat = startLat;
      latlong.Long += dlong;
    }

  memset (strText, 0, sizeof strText);

//---------------------------------------------------------------------------








/*
The false origin is 400 km west and 100 km north of the ‘true
origin’ on the central meridian at 49°N 2°W.
*/
  latlong.Lat = 49.766809;
  latlong.Long = -7.5571598;
  en = latlong_to_EN (latlong);
  printCirle ("OS_FO", en.E, en.N, 1e4, 8);

  latlong.Lat = 49.0;
  latlong.Long = -2.0;
  en = latlong_to_EN (latlong);
  printCirle ("OS_TO", en.E, en.N, 1e4, 8);




  printDXFfooter ();

}


struct XYZ
latlong_to_XYZ (struct LATLONG LatLong)
{


  double r = 6377563.396 / 2.0;
  struct XYZ xyz;
  double piOver2 = 1.570796;

  xyz.x = r * sin (piOver2 - LatLong.Long) * cos (LatLong.Lat);
  xyz.y = r * sin (piOver2 - LatLong.Long) * sin (LatLong.Lat);
  xyz.z = r * cos (piOver2 - LatLong.Long);

  return xyz;
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
printDXFtext (char *text, char *layername, int colour, double x0, double y0,
	      double h, double rot, int justify)
{
  printf ("TEXT\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("62\n");
  printf ("%d\n", colour);
  printf ("10\n");
  printf ("%f\n", x0);
  printf ("20\n");
  printf ("%f\n", y0);
  printf ("30\n");
  printf ("0.0\n");
  printf ("40\n");
  printf ("%f\n", h);
  printf ("50\n");
  printf ("%f\n", rot);
  printf ("72\n");
  printf ("%d\n", justify);
  printf ("7\nSIMPLEX\n");
  printf ("1\n");
  printf ("%s\n", text);
  printf ("0\n");
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
readinLatLong1 (char *filename)
{

//printf("%s\n",filename);



  FILE *file;
  char c;

  file = fopen (filename, "r");

  char sNum[30];
  int q = 0;

  double dx = 0;
  double dy = 0;

  if (file)
    {


      while (c != EOF || (int) c == 255)
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
	      dy = atof (sNum);
	      q = 0;
	      sNum[q] = '\0';
	    }

	  if (c == '\n')
	    {
	      dx = atof (sNum);
	      //printf("%f ---- %f\n",dx,dy);

	      // struct LATLONG LLo;
	      //LLo.Lat = dy;
	      //LLo.Long = dx;
	      // en = latlong_to_EN (LLo);

	      printPloyLineVertex ("LatLong", dx, dy, 0.0);
	      q = 0;
	      sNum[q] = '\0';
	    }



	}


      fclose (file);
    }




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


      while (c != EOF || (int) c == 255)
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


  //read in data in to a linked list
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

  printPolyLineHeader ("UKcoast", 7, 1);


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


      while (c != EOF || (int) c != 255)
	{

	  c = getc (file);
	  // printf("%c %d\n",c,(int)c);

	  if (c == EOF || (int) c == 255)
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






  printPolyLineHeader ("Eire", 76, 1);;
  printMapLatLong ("input/EireSI.csv");
  printPolyLineFooter ();



  printPolyLineHeader ("NI", 3, 1);;
  printMapLatLong ("input/NI.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("IsleOfWight", 4, 1);
  printMapLatLong ("input/IsleOfWight.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("IsleOfMan", 4, 1);
  readinLatLong ("input/IsleOfMan.csv");
  printPolyLineFooter ();


  printPolyLineHeader ("OrkneyIsles", 4, 1);
  printMapLatLong ("input/Orkney.csv");
  printPolyLineFooter ();

  printPolyLineHeader ("ShetlandIsles", 4, 1);
  printMapLatLong ("input/Shetland.csv");
  printPolyLineFooter ();



  freeLinesList ();

}



void
printWedgey (char *layername, int colour, double x, double y, double r,
	     double StartAng, double incAng)
{

  printPolyLineHeader (layername, colour, 1);
  double degtorad = 0.017453292;
  double B;
  if (r < 0)
    r = -r;
  B = tan (incAng * degtorad * 0.25);
  printPloyLineVertex (layername, 0, 0, 0.0);
  printPloyLineVertexBulge (layername, r * cos (StartAng * degtorad),
			    r * sin (StartAng * degtorad), 0.0, B);
  printPloyLineVertex (layername,
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
printPoint (char *layername, double x0, double y0, int colour)
{

  /*POINT     10, 20, 30 (point).

     Point entities have an optional 50 group that determines 
     the orientation of PDMODE images. The group value is the 
     negative of the Entity Coordinate Systems (ECS) angle of 
     the UCS X axis in effect when the point was drawn. The X 
     axis of the UCS in effect when the point was drawn is 
     always parallel to the XY plane for the point's ECS, and 
     the angle between the UCS X axis and the ECS X axis is a 
     single 2D angle. The value in group 50 is the angle from 
     horizontal (the effective X axis) to the ECS X axis. 
     Entity Coordinate Systems (ECS) are described later in 
     this section.
   */
  printf ("POINT\n");
  printf ("8\n");
  printf ("%s\n", layername);
  printf ("62\n");
  printf ("%d\n", colour);
  printf ("10\n");
  printf ("%f\n", x0);
  printf ("20\n");
  printf ("%f\n", y0);
  printf ("30\n");
  printf ("0.0\n");
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
printPolarLine (char *layername, double x0, double y0, double r, double theta,
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
  printf ("%f\n", x0 + r * cos (theta));
  printf ("21\n");
  printf ("%f\n", y0 + r * sin (theta));
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
	      xm1.colour = 5;
	      t = insertLineafter (xm1, t);
	    }
	  if (i < 7)
	    {
	      xm1.key = i + 1;
	      xm1.x0 = dx;
	      xm1.y0 = dy;
	      xm1.x1 = dx + 1e5;
	      xm1.y1 = dy;
	      xm1.colour = 5;
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
generateNightingaleExample (int N, int day, int mon, int year)
{

  printDXFheader ();


  FILE *file;
  char c, c1;
  double cdiv = 0.0;		//number of divisions in a whole circle 360 deg
  int nData = 0;
  double ang = 0.0;


  file = fopen ("input/coviddataUK.csv", "r");

  char strText[30];
  double th = 0.5;
  double dr = 0.0;

  char sNum[30];
  memset (sNum, 0, sizeof sNum);

  int q = 0;

  int dx = 0.0;
  int dy = 0.0;

  double dxold = 0.0;
  double dyold = 0.0;

  if (file)
    {



      while (c != EOF || (int) c != 255)
	{
	  c = getc (file);
	  if (c == EOF || (int) c == 255)
	    break;
	  if (c == '\n')
	    nData++;
	}




      fseek (file, 0, SEEK_SET);


      if (N > 0)
	cdiv = 360.0 / (double) N;
      else
	cdiv = 360.0 / (double) nData;



      printf ("999\n");
      printf
	("------------------------------------------------------------------  %d - %f\n",
	 nData, cdiv);


      int i = 0;

      while (c1 != EOF || (int) c1 != 255)
	{

	  c1 = getc (file);
	  if (c1 == EOF || (int) c1 == 255)
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
	      memset (sNum, 0, sizeof sNum);
	    }

	  if (c1 == '\n')
	    {
	      dy = atoi (sNum);
	      printf ("999\n");
	      printf ("----------------------------------%d ---- %d---- %f\n",
		      dx, dy, (double) dx / (double) dy);
	      dr = ((double) dx / (double) dy) * 100.0;
	      printWedgey ("NightingaleT=0", 1, 0, 0, dr, ang, cdiv);

	      sprintf (strText, "%.1f%%", dr);
	      printDXFtext (strText, "NightingaleTextT=0", 1,
			    10.0 * cos (ang * degToRad),
			    10.0 * sin (ang * degToRad), th, ang, 0);

//to do remove hard coded /7/20 

	      sprintf (strText, "%d/%d/%d", i + 1, mon, year);
	      printDXFtext (strText, "NightingaleTextDate", 3,
			    20.0 * cos (ang * degToRad),
			    20.0 * sin (ang * degToRad), th, ang, 0);



	      if (i != 0)
		{
		  dr =
		    (((double) dx - dxold) / ((double) dy - dyold -
					      7.0)) * 100.0;
		  printWedgey ("NightingaleT=7", 5, 0, 0, dr, ang, cdiv);


		  sprintf (strText, "%.1f%%", dr);
		  printDXFtext (strText, "NightingaleTextT=7", 5,
				17.0 * cos (ang * degToRad),
				17.0 * sin (ang * degToRad), th, ang, 0);

		}

	      dxold = (double) dx;
	      dyold = (double) dy;
	      i++;

	      ang += cdiv;

	      q = 0;

	      memset (sNum, 0, sizeof sNum);

	      memset (strText, 0, sizeof strText);
	    }



	}


      fclose (file);
    }








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
  double th = dr1 * 0.5;
  double dr10 = dr0 + dr1;
  char strText[10];

  for (int i = 0; i < cdiv; i++)
    {

      printWedgey ("Fibbonacci", i, 0, 0, dr10, ang, 360 / cdiv);

      sprintf (strText, " %.0f", dr1 / 1e4);
      printDXFtext (strText, "FibbonacciText", i, dr10 * cos (ang * degToRad),
		    dr10 * sin (ang * degToRad), th, ang, 0);

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

      while (c1 != EOF || (int) c1 != 255)
	{

	  c1 = getc (file);
	  if (c1 == EOF || (int) c1 == 255)
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
		  printf (",%.0f/[%.0f-7]=%.3f[%.1f%%],?",
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
generateHTMLtablefromCSVtest (int day, int month, int year)
{



  printf ("<img src='NightingaleExample_%d_%d_%d.png'>", day, month, year);



  printf ("<br><br><table border='2'>\n");
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
      while (c != EOF || (int) c != 255)
	{
	  c = getc (file);
	  if (c == EOF || (int) c == 255)
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
print2Dflange ()
{

  printDXFheader ();

  double q = 100.0;		//straight line distance between holes



  double R = 150.0;		//out flange radius
  double Ri = 50.0;		//inner flange radius
  double r = 7.0;		//hole radius
  int nholes = 8;		//number of holes


  double dtheta = (double) (360 / nholes) * degToRad;
  double p = q / (2 * sin (dtheta * 0.5));	//Pitch circle radius


  double theta = 0.0;

  printCirle ("flange2D", 0.0, 0.0, R, 7);	//Outer dia
  printCirle ("flange2D", 0.0, 0.0, Ri, 7);	//Inner dia
  printCirle ("flange2D", 0.0, 0.0, p, 1);	//Pitch Circle Diameter

//print holes
  for (int i = 0; i < nholes; i++)
    {
      printCirle ("flange2D", p * cos (theta), p * sin (theta), r, 7);
      theta += dtheta;
    }


  printDXFfooter ();

}


void
exportPolylineCircle ()
{





  double R = 100.0;
  double x = R;
  double y = 0.0;
  double theta = 0;
  double dt = 3.142 / 180;

  for (int i = 0; i < 359; i++)
    {
      x = R * cos (theta);
      y = R * sin (theta);
      printf ("%f,%f\n", x, y);
      theta += dt;
    }

}








void
testMassProperties (char *filename)
{

  //write out dxf data
  printDXFheader ();

  printPolyLineHeader ("Pline", 3, 1);
  //read in the polyline


  FILE *file;
  char c;

  file = fopen (filename, "r");


  char sNum[30];
  int q = 0;

  double x1 = 0.0;
  double y1 = 0.0;


  int i = 0;
  //char strText[32];


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
	      memset (sNum, 0, sizeof sNum);
	    }

	  if (c == '\n')
	    {
	      y1 = atof (sNum);

	      printPloyLineVertex ("mems1e", x1, y1, 0.0);
	      q = 0;
	      sNum[q] = '\0';
	      memset (sNum, 0, sizeof sNum);
	      i++;

	    }

	}

      fclose (file);

    }

  printPolyLineFooter ();



  struct tyPlineProperties pline = get_tyPline_from_file (filename);


  double sf, Del, I;		//scale this to the max min of obejct

  double DelX = pline.xmax - pline.xmin;
  double DelY = pline.ymax - pline.ymin;


  if (DelX > DelY)
    Del = DelX;
  else
    Del = DelY;

  if (pline.I1 > pline.I2)
    I = pline.I1;
  else
    I = pline.I2;

  sf = Del / I;

  if (DEBUG)
    fprintf (debugfile,
	     "Function testMassProperties Del/I = sf %f / %f = %f \n", Del, I,
	     sf);



  print_xyAxes ("Axes", 2, pline.xmin, pline.xmax, pline.ymin, pline.ymax, 12,
		12, 1.0, 1.0);


  printCirle ("Centroid", pline.xbar, pline.ybar, 0.01 * Del, 3);	//plot the centroid of pline
  printCirle ("Origin", 0.0, 0.0, 0.02 * Del, 2);	//plot origin, point of roatation

  char strText[40];


  sprintf (strText, "Area [unit]^2 = %8.3e", pline.A);
  printDXFtext (strText, "MassProperties", 1, 10.0, 0.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "xbar [unit], ybar [unit]  = %8.3e, %8.3e", pline.xbar,
	   pline.ybar);
  printDXFtext (strText, "MassProperties", 1, 10.0, 8.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Mxx [unit]^3 =  %8.3e", pline.Mx);
  printDXFtext (strText, "MassProperties", 1, 10.0, 16.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Myy[unit]^3 =  %8.3e", pline.My);
  printDXFtext (strText, "MassProperties", 1, 10.0, 24.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Ixx [unit]^4              = %8.3e", pline.Ixx);
  printDXFtext (strText, "MassProperties", 1, 10.0, 32.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Iyy [unit]^4 = %8.3e", pline.Iyy);
  printDXFtext (strText, "MassProperties", 1, 10.0, 40.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Izz [unit]^4 = %8.3e", pline.Izz);
  printDXFtext (strText, "MassProperties", 1, 10.0, 48.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);


  sprintf (strText, "Ixy [unit]^4 = %8.3e", pline.Ixy);
  printDXFtext (strText, "MassProperties", 1, 10.0, 56.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);


  sprintf (strText, "Inaxx[unit]^4 = %8.3e", pline.Inaxx);
  printDXFtext (strText, "MassProperties", 1, 10.0, 64.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Inayy [unit]^4 = %8.3e", pline.Inayy);
  printDXFtext (strText, "MassProperties", 1, 10.0, 72.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "J [unit]^4 = %8.3e", pline.J);
  printDXFtext (strText, "MassProperties", 1, 10.0, 80.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "rx [unit] = %8.3e", pline.rx);
  printDXFtext (strText, "MassProperties", 1, 10.0, 88.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "ry [unit] = %8.3e", pline.ry);
  printDXFtext (strText, "MassProperties", 1, 10.0, 96.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "Theta1 [radian] = %8.3e", pline.theta1);
  printDXFtext (strText, "MassProperties", 1, 10.0, 104.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);


  sprintf (strText, "Theta2 [radian] = %8.3e", pline.theta2);
  printDXFtext (strText, "MassProperties", 1, 10.0, 112.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "I1[unit]^4 = %8.3e", pline.I1);
  printDXFtext (strText, "MassProperties", 1, 10.0, 120.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);

  sprintf (strText, "I2[unit]^4 = %8.3e", pline.I2);
  printDXFtext (strText, "MassProperties", 1, 10.0, 128.0, 3.0, 0.0, 0);
  memset (strText, 0, sizeof strText);




  /* change to vector arrows */
  printPolarLine ("I1", pline.xbar, pline.ybar, pline.I1 * sf, pline.theta1,
		  4);
  printPolarLine ("I2", pline.xbar, pline.ybar, pline.I2 * sf, pline.theta2,
		  4);




  printDXFfooter ();



}




void
exportPolylineAnnulus ()
{
/*
Generate annulus polyline data and write the polyline
* to stdout or file
*/

  double startAng = 30.0;
  double endAng = 300.0;
  double DelAng = endAng - startAng;
  double R = 100.0;
  double r = 50.0;


  double theta = startAng * degToRad;
  double x = R * cos (theta);
  double y = R * sin (theta);


  double dt = 2 * Pi / 360.0;

  for (int i = 0; i <= DelAng; i++)
    {
      x = R * cos (theta);
      y = R * sin (theta);
      printf ("%f,%f\n", x, y);
      theta += dt;
    }


  theta = endAng * degToRad;

  x = r * cos (theta);
  y = r * sin (theta);

  printf ("%f,%f\n", x, y);


  for (int i = DelAng; i >= 0; i--)
    {
      x = r * cos (theta);
      y = r * sin (theta);
      printf ("%f,%f\n", x, y);
      theta -= dt;
    }





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
