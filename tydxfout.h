


struct tyLin {
	int key; 
 	double x0;
	double y0;
	double x1;
	double y1;
	int colour;
};



struct tyLine {
 struct tyLin  *m;     
 struct tyLine *next; //linked list pointer
};





//global pointers for LL
struct tyLine *head; //link pointer to head node
struct tyLine *z;    //link pointer to tail node
struct tyLine *t;    //link pointer to current node



//forward declarations
void   createLineList();
void   freeLinesList();
struct tyLine *insertLineafter(struct tyLin tm, struct tyLine *t);
int    NLines();



void printDXFheader();
void printDXFfooter();
void printLine(char *layername, double x0, double y0, double x1, double y1,int colour);

void printCirle(char *layername, double cx, double cy, double R, int colour);
void printPolyLineFooter();


void printPolyLineHeader(char *layername, int colour, int closed);

void printPloyLineVertex(char *layername, double x, double y, double z);
void printPloyLineVertexBulge(char *layername, double x, double y, double z, double bulge);

void printArc(char *layername,int colour, double x, double y,double r, double StartAng, double incAng);
void printWedgey(char *layername,int colour, double x, double y,double r, double StartAng, double incAng);

void printMap();
void drawOSGrid();
void drawTest();
void generateHTMLtablefromCSVtest();
void generateFibonacciExample();
void generateNightingaleExample();



























