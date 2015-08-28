#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>


//#include <softPwm.h>
//#include <stdio.h>
//#include <stdlib.h>

int main()
{
wiringPiSetupGpio();
//wiringPiSetup();
  

//pwmSetMode(PWM_MODE_MS);
//pinMode(18,PWM_OUTPUT);

//pwmSetClock(4000);
//pwmSetRange(10);
pinMode(1,OUTPUT);
//for(;;){
//digitalWrite(1,1);
//usleep(1500);
//printf("0");
//digitalWrite(1,0);
//usleep(3500);
//printf("1");
//}
for(;;){
softPwmCreate(18,0,200);
softPwmWrite(18,11);
delay(2000);
softPwmWrite(18,19);
delay(2000);
//softPwmWrite(1,20);
delay(2000);
}
return 0;
}
