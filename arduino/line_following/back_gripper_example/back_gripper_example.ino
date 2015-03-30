#include <Servo.h>

#define STBY 24
#define RIGHTMOTORFORWARD 23        
#define RIGHTMOTORBACKWARD 22       
#define RIGHTMOTORPWM 2             
#define LEFTMOTORFORWARD 25         
#define LEFTMOTORBACKWARD 26        
#define LEFTMOTORPWM 3              


//define constants for motor control
#define RIGHTMOTOR 1
#define LEFTMOTOR 0
#define FWD 1
#define BWD 0
#define RIGHT 1
#define LEFT 0


Servo backGripper;


void setup() 
{
  delay(500);
  setupMotorshield();
  backGripper.attach(4);
  backGripper.write(0);
  delay(1000);
}

void loop()
{
  
  drive_motor(RIGHT, BWD, 30);
  drive_motor(LEFT, BWD, 30);
  delay(2000);
  stop_motors();
  for (int i = 0; i < 180; i++) 
  {
    backGripper.write(i);
    delay(10);
  }
  delay(5000);
  
  reset_gripper();
  
  delay(5000);  
  
}

void reset_gripper() 
{
  backGripper.write(0);
}

void grip_action()
{
  for (int i = 0; i < 180; i++) 
  {
    backGripper.write(i);
    delay(50);
  }
}

/************************** SETUP PINS ************************************************/

void setupMotorshield()
{
  pinMode(STBY, OUTPUT);
  pinMode(RIGHTMOTORFORWARD, OUTPUT);
  pinMode(RIGHTMOTORBACKWARD, OUTPUT);
  pinMode(RIGHTMOTORPWM, OUTPUT);
  pinMode(LEFTMOTORFORWARD, OUTPUT);
  pinMode(LEFTMOTORBACKWARD, OUTPUT);
  pinMode(LEFTMOTORPWM, OUTPUT);
  
  digitalWrite(STBY, HIGH);
  digitalWrite(RIGHTMOTORFORWARD, HIGH);
  digitalWrite(RIGHTMOTORBACKWARD, LOW);
  digitalWrite(LEFTMOTORFORWARD, HIGH);
  digitalWrite(LEFTMOTORBACKWARD, LOW);
}


/************************** DRIVE MOTORS **********************************************************************/
// this function drives the motors: specify motor, direction, speed
void drive_motor(boolean motor, boolean dir, int spd)
{
  if (motor == RIGHTMOTOR)
  {
    if (dir == FWD) //Right motor forward
    {
      digitalWrite(RIGHTMOTORFORWARD, HIGH);
      digitalWrite(RIGHTMOTORBACKWARD, LOW);
      analogWrite(RIGHTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
      
    }
    else if(dir == BWD)//Right motor backwards
    {
      digitalWrite(RIGHTMOTORFORWARD, LOW);
      digitalWrite(RIGHTMOTORBACKWARD, HIGH);
      analogWrite(RIGHTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
    }
  }
  else if(motor == LEFTMOTOR)
  {
    if (dir == FWD)//Left motor forwards
    {
      digitalWrite(LEFTMOTORFORWARD, HIGH);
      digitalWrite(LEFTMOTORBACKWARD, LOW);
      analogWrite(LEFTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
      
    }
    else if(dir == BWD)//Right motor backwards
    {
      digitalWrite(LEFTMOTORFORWARD, LOW);
      digitalWrite(LEFTMOTORBACKWARD, HIGH);
      analogWrite(LEFTMOTORPWM, spd);                            // Send the new calculated motor speeds from the motor controller to the motors
    }
  }  
}

/************************* STOP MOTORS ***************************************************************/
// stop both motors
void stop_motors()
{
  drive_motor(RIGHTMOTOR, FWD, 0);
  drive_motor(LEFTMOTOR, FWD, 0);
}
