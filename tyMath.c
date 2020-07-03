#include <string.h>
#include <math.h>
#include "tyCADv1.h"
#include "tyMath.h"




struct tyPoint
mirrorPointAboutLine (double Ax, double Ay, double Bx, double By, double Cx,
		      double Cy)
{

  if (DEBUG == true)
    fprintf (debugfile, "mirrorPointAboutLine\n");

  struct tyPoint pM;
  double denom = abs (((Bx - Ax) * (Bx - Ax)) + ((By - Ay) * (By - Ay)));
  double u = (((Cx - Ax) * (Bx - Ax)) + ((Cy - Ay) * (By - Ay))) / denom;

  if (DEBUG == true)
    fprintf (debugfile, "u = %f\n", u);

  //D is min dist from an point to a line or where the normal strikes the line very useful point
  double Dx = (Ax + u * (Bx - Ax));
  double Dy = (Ay + u * (By - Ay));

  //the reflective symetry point 
  pM.x = 2 * Dx - Cx;		//Cx + 2*(Dx-Cx);
  pM.y = 2 * Dy - Cy;		//Cy + 2*(Dy-Cy);

  return pM;



}



struct tyPlineProperties
get_tyPline_from_file (char *filename)
{

  struct tyPlineProperties pp;



//Area = 1/2 intergral around the curve (xdy - ydx)



  if (DEBUG)
    fprintf (debugfile, "Function memsDeviceDXF \n");



  FILE *file;
  char c;

  file = fopen (filename, "r");


  char sNum[30];
  int q = 0;

  double xmin = 0.0;
  double xmax = 0.0;
  double ymin = 0.0;
  double ymax = 0.0;

  double x = 0.0;
  double y = 0.0;

  double dx0 = 0.0;
  double dy0 = 0.0;

  double dx = 0.0;
  double dy = 0.0;

  double A = 0.0;
  double dA;

  double Mx = 0.0;
  double My = 0.0;

  double xbar = 0.0;
  double ybar = 0.0;


  double Ixx = 0.0;
  double Iyy = 0.0;
  double Ixy = 0.0;


  int i = 0;



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
	      dx = atof (sNum);
	      if (dx < xmin)
		xmin = dx;
	      if (dx > xmax)
		xmax = dx;
	      q = 0;
	      sNum[q] = '\0';
	      memset (sNum, 0, sizeof sNum);
	    }

	  if (c == '\n')
	    {
	      dy = atof (sNum);
	      //printf("%f ---- %f\n",dx,dy);

	      if (dy < ymin)
		ymin = dy;
	      if (dy > ymax)
		ymax = dy;
	      q = 0;
	      sNum[q] = '\0';
	      memset (sNum, 0, sizeof sNum);


	      if (i == 0)
		{
		  dx0 = dx;
		  dy0 = dy;
		}

	      if (i > 0)
		{
		  dA = 0.5 * (x * dy - y * dx);
		  A += dA;

		  Mx += 0.5 * (x * x * dy - y * dx * dx);
		  My += 0.5 * (x * dy * dy - y * y * dx);


		  Ixx += 0.5 * (x * dy * dy * dy - y * y * y * dx);

		  Iyy += 0.5 * (x * x * x * dy - y * dx * dx * dx);


		  Ixy += 0.5 * (x * dy * x * dy - y * dx * y * dx);





		}

	      x = dx;
	      y = dy;
	      i++;

	    }

	}

      fclose (file);
    }


  dA = 0.5 * (x * dy0 - y * dx0);
  A += dA;			//Closure Point 
  Mx += 0.5 * (x * x * dy0 - y * dx0 * dx0);
  Mx *= 0.5;
  My += 0.5 * (x * dy0 * dy0 - y * y * dx);
  My *= 0.5;

  xbar = Mx / A;
  ybar = My / A;


  Ixx += 0.5 * (x * dy0 * dy0 * dy0 - y * y * y * dx0);
  Ixx *= (1 / 3.0);

  Iyy += 0.5 * (x * x * x * dy0 - y * dx0 * dx0 * dx0);
  Iyy *= (1 / 3.0);





  Ixy += 0.5 * (x * dy0 * x * dy0 - y * dx0 * y * dx0);
  Ixy *= (1 / 4.0);

  double Izz = Ixx + Iyy;	//Polar moment of Inertia about the origin z axis

  double rx = sqrt (Ixx / A);
  double ry = sqrt (Iyy / A);

  double Inaxx = Ixx - A * xbar * xbar;
  double Inayy = Iyy - A * ybar * ybar;



  double J = Inaxx + Inayy;	//Polar moment of Inertia about the origin z axis


  double z1 = (2.0 * Ixy) / (Iyy - Ixx);
  if (z1 < 0)
    z1 *= -1.0;

  double theta1 = 0.5 * atan (z1);
  double theta2 = (3.141592654 * 0.5 + theta1);


  double zz = (Iyy - Ixx) * 0.5;

  double I1 = (Ixx + Iyy) * 0.5 + sqrt (zz * zz + Ixy * Ixy);
  double I2 = (Ixx + Iyy) * 0.5 - sqrt (zz * zz + Ixy * Ixy);

  if (DEBUG)
    {
      fprintf (debugfile, "Area =  %f\n", A);
      fprintf (debugfile, "%f, %f   %f, %f\n", xmin, xmax, ymin, ymax);
    }


  pp.A = A;
  pp.Mx = Mx;
  pp.My = My;
  pp.Ixx = Ixx;
  pp.Iyy = Iyy;
  pp.Inaxx = Inaxx;
  pp.Inayy = Inayy;
  pp.Izz = Izz;
  pp.Ixy = Ixy;
  pp.rx = rx;
  pp.ry = ry;
  pp.theta1 = theta1;
  pp.theta2 = theta2;
  pp.J = J;
  pp.xbar = xbar;
  pp.ybar = ybar;
  pp.I1 = I1;
  pp.I2 = I2;

  pp.xmin = xmin;
  pp.xmax = xmax;
  pp.ymin = ymin;
  pp.ymax = ymax;


  return pp;
}
