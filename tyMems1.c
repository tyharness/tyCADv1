
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
Generate the mems1e design


*/



void
mems1e ()
{


  int n = 80;

  double x, y, L, t, g, sh, susg,suse,susL,sust;


  L = 100.0;//Length of electrode
  t = 10.0; //thickness of electrode
  g = 4.0;  //eletrode gapping
  sh = 100; //spar height  
  susg = 120.0; //suspension gapping
  suse = 30.0;
  susL = 400.0;
  sust = 10.0;
  double linkt = 20.0;
  double linkL = 500.0;
 
  double pad = 300.0;

  x = 0.0;
  y = 0.0;
  

  printf ("%f,%f\n", -sh-susg-sust, 0.0);


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

  x -= sh+susg+sust;   	
  printf ("%f,%f\n", x, y);
  
  double ysparL = y;
  
  y -= susL;
  printf ("%f,%f\n", x, y);

  x += sust;   	
  printf ("%f,%f\n", x, y);

  y+= susL - suse;
  printf ("%f,%f\n", x, y);
  
  x += susg;   	
  printf ("%f,%f\n", x, y);

  y -= 0.5*(ysparL-linkt )- suse ;
  printf ("%f,%f\n", x, y);
  
   x -= linkL;   	
  printf ("%f,%f\n", x, y);

   y += 0.5*(pad - linkt) ;
   printf ("%f,%f\n", x, y);

   x -= pad;   	
  printf ("%f,%f\n", x, y);

   y -= pad; ;
   printf ("%f,%f\n", x, y);

   x += pad;   	
  printf ("%f,%f\n", x, y);

   y += 0.5*(pad - linkt) ;
   printf ("%f,%f\n", x, y);
   
x += linkL;   	
  printf ("%f,%f\n", x, y);

  y -= 0.5*(ysparL-linkt) - suse ;
  printf ("%f,%f\n", x, y);
  
  x -= susg;   	
  printf ("%f,%f\n", x, y);
 
  y+= susL;
  printf ("%f,%f\n", x, y); 


  x -= sust;   	
  printf ("%f,%f\n", x, y);
 

}
