//gcc CalculateLine.c -lwiringPi -lm -o CalculateLine.c

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

#define STEP_PAUSE 15000

#define BASELENGTH              830
#define CORDLENGTH_LEFT         380
#define CORDLENGTH_RIGHT        600



//Steps per mm (with gear)
//double StepsPermm = 2000.0 / 98.0;== 20,4
//Steps per mm (without gear)
//double StepsPermm = 2000.0 / 199.0;== 10,05
double StepsPermm = (200.0/(8.0*3.141592654)); //7,95
//double StepsPermm = (400.0/(8.0*3.141592654)); //15,91
//double StepsPermm = (800.0/(8.0*3.141592654)); //31,83

// goblal var
long CordLengthLeft = 0;
long CordLengthRight = 0;
long BaseLength = 0;
long X0, Y0;
long currentX = 0, currentY = 0;
long countStepsLeft=0;
long countStepsRight=0;

// ****************** FUNCIONS   ******************
int MakeStepLeft(int dir)
{
//pinMode (STEP_LEFT, OUTPUT);
//pinMode (DIR_LEFT, OUTPUT);

digitalWrite(DIR_LEFT, dir);

digitalWrite(STEP_LEFT, 1);
usleep(STEP_PAUSE);
digitalWrite(STEP_LEFT, 0);
usleep(STEP_PAUSE);
//printf("MakeStepLeft(%i)\n",dir);
countStepsLeft++;
return 0;
}

int MakeStepRight(int dir)
{
//pinMode (STEP_RIGHT, OUTPUT);
//pinMode (DIR_RIGHT, OUTPUT);

digitalWrite(DIR_RIGHT, dir);

digitalWrite(STEP_RIGHT, 1);
usleep(STEP_PAUSE);
digitalWrite(STEP_RIGHT,0);
usleep(STEP_PAUSE);
//printf("MakeStepRight(%i)\n",dir);
countStepsRight++;
return 0;
}

//++++++++++++++++++++++++++++++++++ move XY ******************
void moveXY(long X, long Y){
long newCordLengthLeft;
long newCordLengthRight;

BaseLength = BASELENGTH * StepsPermm;
CordLengthLeft = CORDLENGTH_LEFT * StepsPermm;
CordLengthRight = CORDLENGTH_RIGHT * StepsPermm;

X0 = (CordLengthLeft * CordLengthLeft - CordLengthRight * CordLengthRight + BaseLength * BaseLength) / (2.0 * BaseLength);
Y0 = sqrt(CordLengthRight * CordLengthRight - (BaseLength - X0) * (BaseLength - X0));

printf("X0=%ld, Y0=%ld, CL=%ld, CR=%ld StepsPermm=%lf\n", X0, Y0, CordLengthLeft, CordLengthRight, StepsPermm);
printf("MOVEXY  X=%ld, Y=%ld\n", X,Y);

X+=X0;
Y+=Y0;


newCordLengthLeft = sqrt((double)(X * X) + (double)(Y * Y));
newCordLengthRight = sqrt((double)((BaseLength-X) * (BaseLength-X)) + (double)(Y * Y));

  while(newCordLengthLeft > CordLengthLeft){
    //printf("MakeStepLeft(0)\n");
    MakeStepLeft(0);
    CordLengthLeft++;
  }
  while(newCordLengthLeft < CordLengthLeft){
    //printf("MakeStepLeft(1)\n");
    MakeStepLeft(1);
    CordLengthLeft--;
  }

  while(newCordLengthRight > CordLengthRight){
    //printf("MakeStepRight(0)\n");
    MakeStepRight(0);
    CordLengthRight++;
  }
  while(newCordLengthRight < CordLengthRight){
    //printf("MakeStepRight(1)\n");
    MakeStepRight(1);
    CordLengthRight--;
  }
}
// ++++++++++++++++++++++++main  CalculateLine +++++++++++++++++++++++++++
int main (int argc, char **argv)
{
//wiringPiSetup(); POTSER ESTE EL QUE MARCA EL NOM DE LES GPIO O WiringPi
wiringPiSetupGpio();

pinMode (STEP_RIGHT, OUTPUT);
pinMode (DIR_RIGHT, OUTPUT);
pinMode (STEP_LEFT, OUTPUT);
pinMode (DIR_LEFT, OUTPUT);

long moveToX = atol(argv[1]);//*StepsPermm;
long moveToY = atol(argv[2]);//*StepsPermm;

long  tempX = 0, tempY = 0;
int i = 0;


printf("moveToX=%i     moveToY=%i\n",moveToX,moveToY);
printf("moveToX*StepsPermm=%i  moveToY*StepsPermm=%i\n",moveToX*(long)StepsPermm,moveToY*(long)StepsPermm);

  if(moveToX - currentX != 0 && moveToY - currentY != 0){
    tempX = currentX;
    tempY = currentY;

    if(abs(moveToX - currentX) > abs(moveToY - currentY)){
      while(currentX < moveToX){
        currentX++;
        moveXY(currentX, currentY);
        currentY = tempY + (currentX - tempX) * (moveToY - tempY) / (moveToX - tempX);
        moveXY(currentX, currentY);
        printf("X++\n");
      }
      while(currentX > moveToX){
        currentX--;
        moveXY(currentX, currentY);
        currentY = tempY + (currentX - tempX) * (moveToY - tempY) / (moveToX - tempX);
        moveXY(currentX, currentY);
        printf("X--\n");
      }
    }
    else{
      while(currentY < moveToY){
        currentY++;
        moveXY(currentX, currentY);
        currentX = tempX + (currentY - tempY) * (moveToX - tempX) / (moveToY - tempY);
        moveXY(currentX, currentY);
       printf("Y++\n");
       }
      while(currentY > moveToY){
        currentY--;
        moveXY(currentX, currentY);
        currentX = tempX + (currentY - tempY) * (moveToX - tempX) / (moveToY - tempY);
        moveXY(currentX, currentY);
        printf("Y--\n");
      }
    }
  }


  while(moveToY > currentY){
    currentY++;
    moveXY(currentX, currentY);
  }
  while(moveToY < currentY){
    currentY--;
    moveXY(currentX, currentY);
  }

  while(moveToX > currentX){
    currentX++;
    moveXY(currentX, currentY);
  }
  while(moveToX < currentX){
    currentX--;
    moveXY(currentX, currentY);
  }
printf("countStepsLeft=%i  countStepsRight=%i\n", countStepsLeft,countStepsRight);
printf("X0=%ld, Y0=%ld, CL=%ld, CR=%ld StepsPermm=%lf\n", X0, Y0, CordLengthLeft, CordLengthRight, StepsPermm);

return 0;
}
