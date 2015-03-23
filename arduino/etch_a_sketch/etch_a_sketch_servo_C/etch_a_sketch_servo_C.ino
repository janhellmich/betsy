/*Version Notes
VERSION 3.0!!!

This is able to write IEEE in roughly 16 seconds + - 1 second. 
Arms are able to move in sync. 
However, it is only able to write IEEE when it is at a 90 degree angle. Otherwise it looks wierd. 


/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 
 
 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Knob
 */

#include <Servo.h>
//#include <AFMotor.h>

// create servo objects to control servos 1 and 2 and their pin values
//Servo armservoleft;
//Servo armservoright;
Servo arm;
Servo knobservoright;
Servo knobservoleft;
int pin1 = 7; // Left and right (left) knob
int pin2 = 8; //Up and down (right) knob

int pin4 = 5; //right arm //BOTH ARMS
int pos = 0;

// create drawing direction indicators. Servo 2 should be acting on the up/down direction
int UP = 2;
int DOWN = 1;
int RIGHT = 4;
int LEFT = 3;
// set null speed for both servos
int null_speed = 90;
// set drive speed for both motors
int drive_speed = 8;
int turn_value = 0;


// create a draw action funcion that takes direction and seconds the motor spins in that direction as input
void draw_action(int draw_dir, int seconds)
{
  turn_value = 0;
  switch(draw_dir){
  case 1:
   
    knobservoleft.attach(pin2);   
    turn_value = null_speed + drive_speed + 4;  
    knobservoleft.write(turn_value);  
    delay(seconds);
    knobservoleft.detach();
    break;
  case 2:
    knobservoleft.attach(pin2);
    turn_value = null_speed - drive_speed - 4;
    knobservoleft.write(turn_value);  
    delay(seconds);
    knobservoleft.detach();
    break;
  case 3:
    knobservoright.attach(pin1); 
    turn_value = null_speed + drive_speed + 2;
    knobservoright.write(turn_value); 
    delay(seconds);
    knobservoright.detach();
    break;
  case 4:
    knobservoright.attach(pin1); 
    turn_value = null_speed - drive_speed - 4;
    knobservoright.write(turn_value);  
    delay(seconds);
    knobservoright.detach();
  }
}


void setup() 
{ 
 arm.attach(pin4);
 arm.write(90);
 delay(3000);
} 

void loop() 
{ 
//  while (true) 
//  {
//    arm.write(0);              // tell servo to go to position in variable 'pos' 
//    delay(3000); 
//  }
  arm.write(0);              // tell servo to go to position in variable 'pos' 
  delay(3000);                      // waits 15ms for the servo to reach the position 

  draw_action(LEFT, 250);
  draw_action(UP, 700);
  draw_action(DOWN, 700);
  draw_action(RIGHT, 400);
  

  // draw E
  draw_action(UP, 700);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 250);
  draw_action(LEFT, 400);
  draw_action(DOWN, 100);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 100);
  draw_action(LEFT, 400);
  draw_action(DOWN, 100);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 100);
  draw_action(LEFT, 500);
  draw_action(RIGHT, 750);
  

  // draw E
  draw_action(UP, 700);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 250);
  draw_action(LEFT, 400);
  draw_action(DOWN, 100);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 100);
  draw_action(LEFT, 400);
  draw_action(DOWN, 100);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 100);
  draw_action(LEFT, 500);
  draw_action(RIGHT, 750);
 
  // draw E
  draw_action(UP, 700);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 250);
  draw_action(LEFT, 400);
  draw_action(DOWN, 100);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 100);
  draw_action(LEFT, 400);
  draw_action(DOWN, 100);
  draw_action(RIGHT, 400);
  draw_action(DOWN, 100);
  draw_action(LEFT, 500);
  draw_action(RIGHT, 750);
            
  arm.write(90); 
  finished();                      // waits 15ms for the servo to reach the position
}

void finished() 
{
  while(true) 
  {
    delay(2000);
  }
}
