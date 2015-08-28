#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <softPwm.h>

int main (int argc, char *argv[])
{
    if (wiringPiSetup () == -1)
    exit (1) ;

    pinMode (1, OUTPUT) ;
    digitalWrite (1, LOW) ;

    softPwmCreate(1,0,200);

    int pos=atoi(argv[1]);
    //if(pos<180) pos=180;
    //if(pos>194) pos=194;
    softPwmWrite(1,pos);

    delay(3000);
    return 0 ;
}

