/* ****************************************************************


Sorry - I've heavly lifted and adapted this code from the original site:

http://epirecip.es/epicookbook/chapters/kr08/2_1/c_original

Default Set up US Population Jeff Henning Example
https://community.ptc.com/t5/PTC-Mathcad/coronavirus-math/m-p/657813?attachment-id=75809


You can use gnuplot to graph the data if you prefer with this script


Plot the data using gnuplot script
#gnuplot whole data set
#set output 'SIR.png'
set key autotitle columnhead
set datafile separator ","
set xrange [0:340]
set yrange [0:350e6]
set title "SIR"
set xlabel "S(t), I(t), R(t)"
set ylabel "S,I,R,I+R"
plot 'SIR.dat' using 0:2 with lines ls 3 ti "S Data" , '' using 0:3 with lines ls 1 ti "I Data" , '' using 0:4 with lines ls 2 ti "R Data" , '' using 0:5 with lines lt rgb "violet" ti "I+R Data" 

///////////////////////////// 

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SIR.h"



double ti, S, I, R, Pop[3];
double dPop[3];
void Runge_Kutta (double step);
void Perform_Checks ();
void Output_Data ();
void Diff (double Pop[3]);



void
generateSIRdata ()
{

/*
 * UK Population Model N=60e6(above): 
 * (1 infected, do nothing no intervention R=4.7 [./SIR 60e6 0.0714286 1 4.7 500 >> SIR.dat]),
(1 infected, Moderate Social Distancing R=1.5 [./SIR 60e6 0.0714286 10000 1.5 500 >> SIR.dat]),
(1 infected, Aggressive Social Distancing R=0.5[./SIR 60e6 0.0714286 10000 0.5 500 >> SIR.dat])
*/


  N = 60e6;
  gamm = 0.0714286; //1.0 / 14.0;
  I0 = 1;
  R0 = 4.7;
  MaxTime = 500.0;


  S0 = N - 1;
  beta = R0 * gamm;




  S = S0;
  I = I0;
  R = R0;

  double step = 1.0;

  double Every = pow (10, floor (log10 ((1.0 / ((beta + gamm) * S0)))));

  while (MaxTime / Every > 10000)
    {
      Every *= 10.0;
    }
  // printf("Using a time step of %g and outputing data every %g\n\n",step,Every);



  /* The main iteration routine */

  ti = 0;

  Output_Data ();



  do
    {
      Runge_Kutta (step);
      ti += step;

      /* If time has moved on sufficiently, output the current data */
      if (floor (ti / Every) > floor ((ti - step) / Every))
	{

	  Output_Data ();
	}

    }
  while (ti < MaxTime);


  Output_Data ();


}



void
Output_Data ()
{
  printf ("%g,%g,%g,%g,%g,\n", ti, S, I, R, I + R);
}


void
Runge_Kutta (double step)
{
  int i;
  double dPop1[3], dPop2[3], dPop3[3], dPop4[3];
  double tmpPop[3], initialPop[3];

  /* Integrates the equations one step, using Runge-Kutta 4
     Note: we work with arrays rather than variables to make the
     coding easier */

  initialPop[0] = S;
  initialPop[1] = I;
  initialPop[2] = R;

  Diff (initialPop);
  for (i = 0; i < 3; i++)
    {
      dPop1[i] = dPop[i];
      tmpPop[i] = initialPop[i] + step * dPop1[i] / 2;
    }

  Diff (tmpPop);
  for (i = 0; i < 3; i++)
    {
      dPop2[i] = dPop[i];
      tmpPop[i] = initialPop[i] + step * dPop2[i] / 2;
    }

  Diff (tmpPop);
  for (i = 0; i < 3; i++)
    {
      dPop3[i] = dPop[i];
      tmpPop[i] = initialPop[i] + step * dPop3[i];
    }

  Diff (tmpPop);

  for (i = 0; i < 3; i++)
    {
      dPop4[i] = dPop[i];

      tmpPop[i] =
	initialPop[i] + (dPop1[i] / 6 + dPop2[i] / 3 + dPop3[i] / 3 +
			 dPop4[i] / 6) * step;
    }


  S = tmpPop[0];
  I = tmpPop[1];
  R = tmpPop[2];

  return;
}

// Initialise the equations and Runge-Kutta integration
void
Diff (double Pop[3])
{
  // Set up temporary variables to make the equations look neater

  double tmpS, tmpI;		//, tmpR;


  tmpS = Pop[0];
  tmpI = Pop[1];		//tmpR=Pop[2];

  /* The differential equations */

  dPop[0] = -(beta * tmpS * tmpI) / N;	// dS/dt
  dPop[1] = ((beta * tmpS * tmpI) / N) - gamm * tmpI;	// dI/dt
  dPop[2] = gamm * tmpI;	// dR/dt

  return;
}
