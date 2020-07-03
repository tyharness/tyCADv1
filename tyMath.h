



struct tyPoint
{
  double x;
  double y;
};


struct tyPlineProperties
{

  double xmin;
  double xmax;
  double ymin;
  double ymax;

  double A;			//Area
  double xbar;			//distance to centroid from yaxis x=0
  double ybar;			//distance to centroid from xaxis y=0
  double Mx;			//First moment of Area
  double My;			//First moment of Area
  double Ixx;			//Second moment of Area
  double Iyy;			//Second moment of Area
  double Izz;			//Second moment of Area
  double Ixy;
  double Inaxx;			//Second moment of Area
  double Inayy;			//Second moment of Area


  double J;

  double rx;			//radius of gyration
  double ry;			//radius of gyration 
  double theta1;		//principal axis
  double theta2;		//principal axis
  double I1;			//principal max
  double I2;			//principal min


};

//FUnction Prototypes

struct tyPoint mirrorPointAboutLine (double Ax, double Ay, double Bx,
				     double By, double Cx, double Cy);


struct tyPlineProperties get_tyPline_from_file (char *filename);
