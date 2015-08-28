//gcc moveXY.c -lwiringPi -lm -o moveXY.c

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <math.h>
#include <string.h>

//wiringPI
#define STEP_LEFT 4
#define DIR_LEFT 17
#define STEP_RIGHT 23
#define DIR_RIGHT 22

#define STEP_PAUSE 1500

#define BASELENGTH              845
#define CORDLENGTH_LEFT         220
#define CORDLENGTH_RIGHT        667


int MakeStepLeft(dir)
{
digitalWrite(DIR_LEFT, dir);

digitalWrite(STEP_LEFT, 1);
usleep(STEP_PAUSE);
digitalWrite(STEP_LEFT,0);
usleep(STEP_PAUSE);
printf("left-%d\n",dir);
return 0;
}

int MakeStepRight(dir)
{
digitalWrite(DIR_RIGHT, dir);

digitalWrite(STEP_RIGHT, 1);
usleep(STEP_PAUSE);
digitalWrite(STEP_RIGHT,0);
usleep(STEP_PAUSE);
printf("RIGTH-%d\n",dir);
return 0;
}

int main (int argc, char **argv)
{
wiringPiSetup();
pinMode (STEP_RIGHT, OUTPUT);
pinMode (DIR_RIGHT, OUTPUT);
pinMode (STEP_LEFT, OUTPUT);
pinMode (DIR_LEFT, OUTPUT);

long X =atol(argv[1]);
long Y =atol(argv[2]);

printf("X=%i\nY=%i\n",X,Y);


//Steps per mm (with gear)
//double StepsPermm = 2000.0 / 98.0;== 20,4
//Steps per mm (without gear)
//double StepsPermm = 2000.0 / 199.0;== 10,05
double StepsPermm = (200.0/(8.0*3.141592654)); //7,95
//double StepsPermm = 7.957747155;
long CordLengthLeft = 0;
long CordLengthRight = 0;
long BaseLength = 0;
long X0, Y0;
long newCordLengthLeft;
long newCordLengthRight;

BaseLength = BASELENGTH * StepsPermm;
CordLengthLeft = CORDLENGTH_LEFT * StepsPermm;
CordLengthRight = CORDLENGTH_RIGHT * StepsPermm;

X0 = (CordLengthLeft * CordLengthLeft - CordLengthRight * CordLengthRight + BaseLength * BaseLength) / (2.0 * BaseLength);
Y0 = sqrt(CordLengthRight * CordLengthRight - (BaseLength - X0) * (BaseLength - X0));

printf("X0=%ld, Y0=%ld, CL=%ld, CR=%ld StepsPermm=%lf\n", X0, Y0, CordLengthLeft, CordLengthRight, StepsPermm);

X+=X0;
Y+=Y0;

newCordLengthLeft = sqrt((double)(X * X) + (double)(Y * Y));
newCordLengthRight = sqrt((double)((BaseLength-X) * (BaseLength-X)) + (double)(Y * Y));

  while(newCordLengthLeft > CordLengthLeft){
    printf("MakeStepLeft(1)\n");
    CordLengthLeft++;
  }
  while(newCordLengthLeft < CordLengthLeft){
    printf("MakeStepLeft(-1)\n");
    CordLengthLeft--;
  }

  while(newCordLengthRight > CordLengthRight){
    printf("MakeStepRight(1)\n");
    CordLengthRight++;
  }
  while(newCordLengthRight < CordLengthRight){
    printf("MakeStepRight(-1)\n");
    CordLengthRight--;
  }

return 0;
}


