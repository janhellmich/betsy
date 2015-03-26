#include <Servo.h>

Servo backGripper;


void setup() 
{
  delay(500);
  backGripper.attach(4);

  delay(1000);
}

void loop()
{
  
  for (int i = 0; i < 180; i++) 
  {
    backGripper.write(i);
    delay(50);
  }
  delay(3000);  
  
}
