
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
