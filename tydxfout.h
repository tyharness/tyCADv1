

extern double EarthSemiAxis_a;
extern double EarthSemiAxis_b;
extern double E0;
extern double N0;
extern double F0;
extern double lambda0;
extern double phi0;		// Projection UTM zone 30 

extern double degToRad;

struct EN
{
  double N;
  double E;
};

struct LATLONG
{
  double Lat;
  double Long;
};

struct XYZ
{
  double x;
  double y;
  double z;
};





struct tyLin
{
  int key;
  double x0;
  double y0;
  double x1;
  double y1;
  int colour;
};



struct tyLine
{
  struct tyLin *m;
  struct tyLine *next;		//linked list pointer
};





//global pointers for LL
struct tyLine *head;		//link pointer to head node
struct tyLine *z;		//link pointer to tail node
struct tyLine *t;		//link pointer to current node



//forward declarations
void createLineList ();
void freeLinesList ();
struct tyLine *insertLineafter (struct tyLin tm, struct tyLine *t);
int NLines ();

struct EN latlong_to_EN (struct LATLONG LatLong);

struct XYZ latlong_to_XYZ (struct LATLONG LatLong);



void printblankDXF ();
void printDXFheader ();
void printDXFfooter ();




void
print_xy_graph (char *filename, char *Title, char *legendX, char *legendY,
		char *layername, int colour, int mode);



void print_xyAxes (char *layername, int colour, double xmin, double xmax,
		   double ymin, double ymax, int divX, int divY,
		   double oscaleX, double oscaleY);


void printPoint (char *layername, double x0, double y0, int colour);



void printLine (char *layername, double x0, double y0, double x1, double y1,
		int colour);

void printCirle (char *layername, double cx, double cy, double R, int colour);

void printDXFtext (char *text, char *layername, int colour, double x0,
		   double y0, double h, double rot, int justify);


void printPolyLineFooter ();


void printPolyLineHeader (char *layername, int colour, int closed);

void printPloyLineVertex (char *layername, double x, double y, double z);
void printPloyLineVertexBulge (char *layername, double x, double y, double z,
			       double bulge);

void printArc (char *layername, int colour, double x, double y, double r,
	       double StartAng, double incAng);
void printWedgey (char *layername, int colour, double x, double y, double r,
		  double StartAng, double incAng);

void printWorldMapExample ();
void print_lat_long_projection ();
void print_lat_long_cylindrical_projection ();
void print_wgs_to_OS ();
void printMap ();
void drawOSGrid ();
void drawTest ();

void convert_coviddataUK_to_COVID19dataUK (int mon, int year);
void generateHTMLtablefromCSVtest ();
void generateFibonacciExample ();
void generateNightingaleExample (int N);
void printMapwithDXFheadAndFoot ();

void readinLatLong (char *filename);
void readinLatLong1 (char *filename);
void printMapLatLong (char *countryname);

void z88ExampleDXF ();
void memsDeviceDXF ();
void memsDevice ();
