/*

DXF reference information found at:
http://paulbourke.net/dataformats/dxf/dxf10.html
*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tydxfout.h"



void printMap(){


//read in data
createLineList();

drawOSGrid();


//write out dxf data
printDXFheader();




printCirle("London",538966.0,177334.0,8.9e6/100,7);
printCirle("Lincoln",497796,371810,756000/100,2);
printCirle("Birmingham",406689, 286822,1.086e6/100,3);
printCirle("Cardiff",318371, 176329,335000/100,4);
printCirle("Glasgow",260107, 665646,600000/100,5);
printCirle("Aberdeen",425678, 554270,208000/100,6);


//Print the line list/////////////////////////////////////////////////
	t = head->next;		//assume head is a dummy node

	while (t != z) {

	    struct tyLin mPtr;
	    mPtr = *(*t).m; 
	    printLine("tyLinesList",mPtr.x0, mPtr.y0, mPtr.x1, mPtr.y1, mPtr.colour);
            t = t->next;
	}



printLine("Meridian",538966.0, -100000, 538966, 1100000,7);


//Print the Polylines

printPolyLineHeader("UKcoast",2,1);

/*test triangle closed
printPloyLineVertex("tyPoly",0.0,0.0,0.0);
printPloyLineVertex("tyPoly",200.0,0.0,0.0);
printPloyLineVertex("tyPoly",200.0,200.0,0.0);
*/

//read in map coords

char *filename;
filename = "UKmapData.csv";

FILE *file;
char c;

file = fopen(filename, "r");
//double x;
//double y;

char sNum[30];
int q = 0;

double dx=0;
double dy=0;
//int flip = 0;

if (file) 
    { 


     while (c != EOF) 
        { 
         c = getc(file);
         if (c==EOF) break;
         if (c!=',' && c!= ' ') 
		{
		  sNum[q] = c;
		  q++;		  
		}	
 
         if (c == ','){
	   dx = atof(sNum);
	   q = 0;  sNum[q] = '\0';}
  
	 if (c == '\n'){
	    dy = atof(sNum);
	    //printf("%f ---- %f\n",dx,dy);
	    printPloyLineVertex("tyPoly",dx,dy,0.0);
	    q = 0; sNum[q] = '\0';}
	 

        
	}
 

fclose(file);
}

printPolyLineFooter();

printDXFfooter();




freeLinesList();

}



void printWedgey(char *layername,int colour, double x, double y,double r, double StartAng, double incAng){

printPolyLineHeader("Nightingale",colour,1);
double degtorad = 0.017453292;
double B;
B = tan(incAng*degtorad*0.25);        
printPloyLineVertex("Nightingale",0,0,0.0);
printPloyLineVertexBulge("Nightingale",r*cos(StartAng*degtorad ),r*sin(StartAng*degtorad ),0.0,B);
printPloyLineVertex("Nightingale",r*cos((StartAng+incAng)*degtorad),r*sin((StartAng+incAng)*degtorad),0.0);
printPolyLineFooter();
}



void printArc(char *layername,int colour, double x, double y,double r, double StartAng, double incAng){
//ARC       10, 20, 30 (center), 40 (radius), 50 (start angle), 51 (endangle).

printf("ARC\n");
printf("8\n");
printf("%s\n",layername);
printf("62\n");
printf("%d\n",colour);
printf("10\n");
printf("%f\n",x);
printf("20\n");
printf("%f\n",y);
printf("30\n");
printf("0\n");
printf("40\n");
printf("%f\n",r);
printf("50\n");
printf("%f\n",StartAng);
printf("51\n");
printf("%f\n",StartAng+incAng);
printf("0\n");
}






/*
void printCirleList(char *layername){
}
*/

void printCirle(char *layername, double cx, double cy, double R, int colour){
printf("CIRCLE\n");
printf("8\n");
printf("%s\n",layername);
printf("62\n");
printf("%d\n",colour);
printf("10\n");
printf("%f\n",cx);
printf("20\n");
printf("%f\n",cy);
printf("30\n");
printf("0\n");
printf("40\n");
printf("%f\n",R);
printf("0\n");
}

void printLine(char *layername, double x0, double y0, double x1, double y1,int colour){
printf("LINE\n");
printf("8\n");
printf("%s\n",layername);
printf("62\n");
printf("%d\n",colour);
printf("10\n");
printf("%f\n",x0);
printf("20\n");
printf("%f\n",y0);
printf("30\n");
printf("0\n");
printf("11\n");
printf("%f\n",x1);
printf("21\n");
printf("%f\n",y1);
printf("31\n");
printf("0\n");
printf("0\n");
}



void printPolyLineHeader(char *layername, int colour, int closed){
printf("POLYLINE\n");
printf("8\n");
printf("%s\n",layername);
printf("70\n");
printf("%d\n",closed);  //closed 1 open 0
printf("66\n");
printf("1\n");
printf("62\n");
printf("%d\n",colour);
printf("0\n");
}



void printPolyLineFooter(){
printf("SEQEND\n");
printf("0\n");
}

void printPloyLineVertex(char *layername, double x, double y, double z){
printf("VERTEX\n");
printf("8\n");
printf("%s\n",layername);
printf("10\n");
printf("%f\n",x);
printf("20\n");
printf("%f\n",y);
printf("30\n");
printf("%f\n",z);
printf("0\n");
}

void printPloyLineVertexBulge(char *layername, double x, double y, double z, double bulge){
/*50 (curve fit tangent direction
            -optional).  The bulge is the tangent of 1/4 the included
            angle for an arc segment, made negative if the arc goes
            clockwise from the start point to the end point; a bulge of 0
            indicates a straight segment, and a bulge of 1 is a semicir-
            cle.
*/

printf("VERTEX\n");
printf("8\n");
printf("%s\n",layername);
printf("10\n");
printf("%f\n",x);
printf("20\n");
printf("%f\n",y);
printf("30\n");
printf("%f\n",z);
printf("42\n");
printf("%f\n",bulge);
printf("0\n");



}

void printDXFfooter(){
printf("ENDSEC\n");
printf("0\n");
printf("EOF\n");
}


void printDXFheader(){

printf("999\n");
printf("Simple DXF by T Harness\n");
printf("0\n");
printf("SECTION\n");
printf("2\n");
printf("HEADER\n");
printf("9\n");
printf("$ACADVER\n");
printf("1\n");
printf("AC1009\n");
printf("9\n");
printf("$INSBASE\n");
printf("10\n");
printf("0.0\n");
printf("20\n");
printf("0.0\n");
printf("30\n");
printf("0.0\n");
printf("9\n");
printf("$EXTMIN\n");
printf("10\n");
printf("-1000.0\n");
printf("20\n");
printf("-1000.0\n");
printf("9\n");
printf("$EXTMAX\n");
printf("10\n");
printf("1000.0\n");
printf("20\n");
printf("1000.0\n");
printf("0\n");
printf("ENDSEC\n");
printf("0\n");
printf("SECTION\n");
printf("2\n");
printf("TABLES\n");
printf("0\n");
printf("ENDSEC\n");
printf("0\n");
printf("SECTION\n");
printf("2\n");
printf("BLOCKS\n");
printf("0\n");
printf("ENDSEC\n");
printf("0\n");
printf("SECTION\n");
printf("2\n");
printf("ENTITIES\n");
printf("0\n");


}








/*                                                DRAW OSGB grid six fig reference */
void drawOSGrid(){

	t = head;
        int dx = 0;int dy = 0;

        struct tyLin xm1;
        for (int j = 0; j < 11; j++) {
        for (int i = 0; i < 8; i++) {
	if (j< 10){
	xm1.key = i + 1;
        xm1.x0 = dx; 		xm1.y0 = dy;
	xm1.x1 = dx;	xm1.y1 = dy+1e5;
 	xm1.colour = 8;
	t = insertLineafter(xm1, t);
	}
	if (i< 7){
	xm1.key = i + 1;
        xm1.x0 = dx; 		xm1.y0 = dy;
	xm1.x1 = dx+1e5;	xm1.y1 = dy;
	xm1.colour = 8;
	t = insertLineafter(xm1, t);
	}
	dx += 1e5;
	}
	dy += 1e5;dx = 0;
 	}

}



/* 								Experimental/Ancillary functions*/
void drawTest(){

t = head;


    struct tyLin xm1;

    for (int i = 0; i < 9; i++) {

	xm1.key = i + 1;
	xm1.x0 = (double)rand()/RAND_MAX*2.0-1.0;
	xm1.y0 = (double)rand()/RAND_MAX*2.0-1.0;
	xm1.x1 = (double)rand()/RAND_MAX*2.0-1.0;
	xm1.y1 = (double)rand()/RAND_MAX*2.0-1.0;

	t = insertLineafter(xm1, t);
    }
//int n = NLines();
//printf("Number of lines = %d \n\n", n );

}

void generateNightingaleExample(){

printDXFheader();

double cdiv = 30;

double r[2][26];

r[0][0] = 2353; r[1][0] = 29474; 
r[0][1] = 2921; r[1][1] = 33718; 
r[0][2] = 3605; r[1][2] = 38168; 
r[0][3] = 4313; r[1][3] = 41903; 
r[0][4] = 4934; r[1][4] = 47806;
r[0][5] = 5373; r[1][5] = 51608;
r[0][6] = 6159; r[1][6] = 55242; 
r[0][7] = 7097; r[1][7] = 60733;
r[0][8] = 7978; r[1][8] = 65077; 
r[0][9] = 8958; r[1][9] = 73758; 
r[0][10] = 9875; r[1][10] = 78991; 
r[0][11] = 10612; r[1][11] = 84279; 
r[0][12] = 11329; r[1][12] = 88621;
r[0][13] = 12107; r[1][13] = 93873; 
r[0][14] = 12868; r[1][14] = 98476;
r[0][15] = 13729; r[1][15] = 103093; 
r[0][16] = 14576; r[1][16] = 108692; 
r[0][17] = 15464; r[1][17] = 114217; 
r[0][18] = 16060; r[1][18] = 120067; 
r[0][19] = 16509; r[1][19] = 124743;
r[0][20] = 17337; r[1][20] = 129044; 
r[0][21] = 18100; r[1][21] = 133495; 
r[0][22] = 18738; r[1][22] = 138078; 
r[0][23] = 19506; r[1][23] = 143464; 
r[0][24] = 20319; r[1][24] = 148377;
r[0][25] = 20732; r[1][25] = 152840;

double ang = 0;
for (int i=0; i< 26; i++){
 printWedgey("Nightingale",i+1,0,0,(r[0][i] / r[1][i])*100 ,ang,360/cdiv);
 //printf("%f   %f   %f\n",r[0][i], r[1][i],  (r[0][i] / r[1][i])*100);
if (i>0) printWedgey("Nightingale1",i+1,0,0, (r[0][i]-r[0][i-1])/(r[0][i]-r[0][i-1]-7)*100      ,ang,360/cdiv);
  ang += 360/cdiv;
}


printDXFfooter();


}





void generateFibonacciExample(){


printDXFheader();

double cdiv = 8;


double ang = 0;
double dr0 = 0;
double dr1 = 1e4;
double dr10 = dr0+dr1;

for (int i=0; i< cdiv; i++){

 printWedgey("Nightingale",i,0,0,dr10,ang,360/cdiv);
 ang += 360/cdiv;
 dr10 = dr0+dr1;
 dr0 = dr1;
 dr1 = dr10;

}


printDXFfooter();

}


void generateHTMLtablefromCSVtest(){
printf("<table border='2'>\n");
printf("<tr><td>UK</td> <td> CFR(T=0)</td> <td> CFR(T=7)</td> <td>ONS registered deaths<br>(increase on 5 year avg.)</td> </tr>");

char *filename;
filename = "COVID19dataUK.csv";

FILE *file;
char c;

file = fopen(filename, "r");

int q = 0;
char col[4][30];

int cinc = 0;


if (file) 
    { 
     while (c != EOF) 
        { 
         c = getc(file);
         if (c==EOF) break;

         if (c!=',') 
		{
		  col[cinc][q] = c;
	          q++;				  
		}
	

 	 if (c==','){
	    col[cinc][q] = '\0';
	    cinc++;	  
	    q=0;
	}
         
  
	 if (c == '\n'){
            col[cinc][q] = '\0';    			    
	    //printf("%s  ----  %s  ----  %s  ----  %s",col[0],col[1],col[2],col[3]);
	   	
	    printf("<tr><td>%s</td><td>  %s </p> </td> <td> %s </td><td> %s </td></tr> ",col[0],col[1],col[2],col[3]); 
	   	
	    q = 0; 
	    cinc = 0;
            }
	}
 fclose(file);
}

printf("</table>\n"); 


}


void createLineList()
{

    head = (struct tyLine *) malloc(sizeof(struct tyLine));	//will need freeing eventually
    z = NULL;

    head->m = NULL;		//perhaps wont need to free m
    head->next = z;


}


void freeLinesList()
{

    struct tyLine *x;
    struct tyLin  *xPtr;


    t = head->next;

    while (t != z) {
	x = t;
	xPtr = (*x).m;

	t = t->next;

	free(xPtr);
	free(x);

    }

    free(t);
    free(head);

}



struct tyLine *insertLineafter(struct tyLin tm, struct tyLine *t)
{


    struct tyLine *x;
    x = (struct tyLine *) malloc(sizeof(struct tyLine));	//create a new move x

    struct tyLin *xm;
    xm = (struct tyLin *) malloc(sizeof(struct tyLin));

    xm->key = tm.key;
    xm->x0 =  tm.x0;
    xm->y0 =  tm.y0;
    xm->x1 =  tm.x1;
    xm->y1 =  tm.y1;
    xm->colour = tm.colour;

    x->m = xm;
    x->next = t->next;		//copy t's pointer into new move
    t->next = x;		//set t next pointer to new move x



    return x;


}



int NLines()
{
    int n = 0;
    t = head->next;		//assume head is a dummy node
    while (t != z) {
	n++;
	t = t->next;
    }

    return n;
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

