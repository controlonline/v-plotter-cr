#include <wiringPi.h>
//#include <softPwm.h>
//#include <stdio.h>
//#include <stdlib.h>

int main(void)
{
//wiringPiSetupGpio();
wiringPiSetup();

//pwmSetMode(PWM_MODE_MS);
//pinMode(18,PWM_OUTPUT);
//pwmSetClock(4000);
//pwmSetRange(10);
pinMode(1,OUTPUT);
for(;;){
digitalWrite(1,0);
delay(2000);
//printf("0");
digitalWrite(1,1);
delay(2000);
//printf("1");
}
//softPwmCreate(18,0,200);
//softPwmWrite(18,190);
//delay(2000);
//softPwmWrite(18,180);
//delay(2000);
//

//softPwmWrite(18,190);
return 0;
}
