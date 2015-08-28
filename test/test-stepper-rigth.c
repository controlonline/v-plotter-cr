#include <stdio.h>
#include <wiringPi.h>

#define STEP 23
#define DIR 22
#define STEP_PAUSE 15000

int main (int argc, char *argv[])
{

int steps= atoi(argv[1]);
int dir = atoi(argv[2]);
int i;

wiringPiSetupGpio();

pinMode (STEP, OUTPUT);
pinMode (DIR, OUTPUT);

//pinMode (STEP, 1);
//pinMode (DIR, 1);

digitalWrite(DIR, dir);

for(i=0;i<steps;i++){
digitalWrite(STEP, 1);
usleep(STEP_PAUSE);
//delay(100);
digitalWrite(STEP,0);
usleep(STEP_PAUSE);
//delay(100);
printf("%d\n",i);
}
return 0;
}
