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

 float calcAlpha(float MLp, float MRp, float Dm){
  float theta = acos((MLp*MLp+Dm*Dm-MRp*MRp)/(2.0*MLp*Dm));
  return theta;
 }
 float calcPx(float MLp, float theta){
   float Px=cos(theta)*MLp;
   return Px;
 }
 float calcPy(float MLp, float theta){
   float Py=sin(theta)*MLp;
   return Py;
 }

 float calcMLp(float x, float y){
   float ML;
   ML=sqrt(x*x+y*y);
   return ML;
 }
 float calcMRp(float x, float y, float Dm){
   float MR;
   MR=sqrt((Dm-x)*(Dm-x)+y*y);
   return MR;
 }

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
//moveToX=atoi(argv[1]);
//moveToY=atoi(argv[2]);
long X =atol(argv[1]);
long Y =atol(argv[2]);
printf("X=%d\nY=%d\n",X,Y);

//printf("X=%d\n",X);

// wiringPI StepUp
wiringPiSetup();

pinMode (STEP_RIGHT, OUTPUT);
pinMode (DIR_RIGHT, OUTPUT);
pinMode (STEP_LEFT, OUTPUT);
pinMode (DIR_LEFT, OUTPUT);

// Pos actual p
float MLp;
float MRp;
float Xp;
float Yp;


float lpp= ((8*3.141592)/400);
// lpp long per step = 0,06283184

// Punt Inicial X0,Y0,ML0,MR0

float ML0 = 220.0;
float MR0 = 667.0;
// Motors distance
float Dm  = 845.0;

//Inicial point
float alpha=calcAlpha(ML0, MR0, Dm);
printf("alpha=%6.6f\n",alpha);

float X0 = calcPx(ML0, alpha);
float Y0 = calcPy(ML0, alpha);

printf("X0=%6.6f\n",X0);
printf("Y0=%6.6f\n",Y0);
//final point X,Y
// line equation

float m = ((Y-Y0)/(X-X0));
float b = (Y-(m*X));

printf("m=%6.6f\n",m);
printf("b=%6.6f\n",b);

//XY repect Origin
float fX=(float)X+X0;
float fY=(float)Y+Y0;
printf("fX=%6.6f\nfY=%6.6f\n",fX,fY);

int inc_dir=0;

if ( (fabs(X0-fX)) > (fabs(Y0-fY)) ){//dir Horizontal
 if (X0<fX) inc_dir=1;             //line Horizontal Y0-fY=0
 if (X0>fX) inc_dir=2;
} else {                           //dir Vertical
 if (Y0<fY) inc_dir=3;
 if (Y0>fY) inc_dir=4;
}
printf("inc_dir=%i\n",inc_dir);

float newX=X0;
float newY=Y0;
float newAlpha=alpha;

while ( (fabs(newX-fX)>0.1) || (fabs(newY-fY)>0.1) ){

printf ("fabs  newX-fX  >0.1  %5.5f",fabs(newX-fX));
printf ("fabs  newY-fY  >0.1  %5.5f",fabs(newY-fY));

//Valor to find, aprox
 if (inc_dir==1){
	newX+=0.02;
	newY=(m*newX)+b;
}
 if (inc_dir==2){
	newX-=0.02;
	newY=(m*newX)+b;
}
 if (inc_dir==3){
	newY+=0.02;
	newX=(newY-b)/m;
}
 if (inc_dir==4){
	newY-=0.02;
	newX=(newY-b)/m;
}
// in leght

float newML= calcMLp(newX, newY);
float newMR= calcMRp(newX, newY, Dm);
//printf("  ML0====%5.5f     MR0=====%5.5f\n",ML0,MR0);
//printf("newML====%5.5f    mewMR====%5.5f\n",newML,newMR);
//printf("fabs(newML-(ML0+lpp)=%5.5f+ fabs(newMR-MR0)=%5.5f\n",fabs(newML-(ML0+lpp)), fabs(newMR-MR0));

float error_a = fabs(newML-(ML0+lpp)) + fabs(newMR-MR0);
float error_b = fabs(newML-(ML0-lpp)) + fabs(newMR-MR0);
float error_c = fabs(newML-(ML0)) + fabs(newMR-(MR0+lpp));
float error_d = fabs(newML-(ML0)) + fabs(newMR-(MR0-lpp));

//printf("error_a=%3.3f-error_b=%3.3f-error_c=%3.3f-error_d=%3.3f\n"
//        ,error_a,error_b,error_c,error_d);



if ((error_a < error_b) &&
    (error_a < error_c) &&
    (error_a < error_d)){
  //MakeStepLeft(1);
  printf("error_a\n");
  ML0+=lpp;
  float newAlpha=calcAlpha(ML0,MR0,Dm);
  newX=calcPx(ML0, newAlpha);
  newY=calcPy(ML0, newAlpha);
}
if ((error_b < error_a) &&
    (error_b < error_c) &&
    (error_b < error_d)){
  //MakeStepLeft(0);
  printf("error_b\n");
  ML0-=lpp;
  float newAlpha=calcAlpha(ML0,MR0,Dm);
  newX=calcPx(ML0, newAlpha);
  newY=calcPy(ML0, newAlpha);
}
if ((error_c < error_a) &&
    (error_c < error_b) &&
    (error_c < error_d)){
  //MakeStepRight(1);
  printf("error_c\n");
  MR0+=lpp;
  float newAlpha=calcAlpha(ML0,MR0,Dm);
  newX=calcPx(ML0, newAlpha);
  newY=calcPy(ML0, newAlpha);
}
if ((error_d < error_a) &&
    (error_d < error_b) &&
    (error_d < error_c)){
  //MakeStepRight(0);
  printf("error_d\n");
  MR0-=lpp;
  float newAlpha=calcAlpha(ML0,MR0,Dm);
  newX=calcPx(ML0, newAlpha);
  newY=calcPy(ML0, newAlpha);
}
printf("ML0=%6.6f-MR0=%6.6f-Dm=%6.6f\n",ML0,MR0,Dm);
printf("newAlpha=%6.6f     ",newAlpha);
printf("newX=%6.6f    ",newX);
printf("newY=%6.6f\n",newY);
delay(1000);

}//while

return 0;
}


