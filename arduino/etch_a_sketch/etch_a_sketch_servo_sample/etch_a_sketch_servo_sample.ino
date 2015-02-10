/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 
 
 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Knob
 */

#include <Servo.h>
#include <AFMotor.h>

// create servo objects to control servos 1 and 2 and their pin values
Servo myservo1;
Servo myservo2;

int pin1 = 10;
int pin2 = 9;

// create drawing direction indicators. Servo 2 should be acting on the up/down direction
int UP = 1;
int DOWN = 2;
int RIGHT = 3;
int LEFT = 4;

// set null speed for both servos
int null_speed_1 = 93;
int null_speed_2 = 88;

// set drive speed for both motors
int drive_speed_1 = 6;
int drive_speed_2 = 8;

// create a draw action funcion that takes direction and seconds the motor spins in that direction as input
void draw_action(int draw_dir, int seconds)
{
  switch(draw_dir){
  case 1:
    myservo2.attach(pin2); 
    myservo2.write(null_speed_2 + drive_speed_2);  
    delay(seconds);
    myservo2.detach();
    break;
  case 2:
    myservo2.attach(pin2); 
    myservo2.write(null_speed_2 - drive_speed_2);  
    delay(seconds);
    myservo2.detach();
    break;
  case 3:
    myservo1.attach(pin1); 
    myservo1.write(null_speed_1 + drive_speed_1);  
    delay(seconds);
    myservo1.detach();
    break;
  case 4:
    myservo1.attach(pin1); 
    myservo1.write(null_speed_1 - drive_speed_1-1);  
    delay(seconds);
    myservo1.detach();
  }
}


void setup() 
{ 

} 

void loop() 
{ 
  
  delay(5000); 
  // draw I
  draw_action(UP, 2600);
  draw_action(LEFT, 500);
  draw_action(DOWN, 3000);
  draw_action(RIGHT, 1000);
  
  // draw E
  draw_action(UP, 2600);
  draw_action(RIGHT, 1000);
  draw_action(DOWN, 550);
  draw_action(LEFT, 750);
  draw_action(DOWN, 550);
  draw_action(RIGHT, 550);
  draw_action(DOWN, 550);
  draw_action(LEFT, 650);
  draw_action(DOWN, 550);
  draw_action(RIGHT, 650);
  draw_action(DOWN, 550);
  draw_action(LEFT, 1350);  
  
    // draw E
  draw_action(RIGHT, 1550);
  draw_action(UP, 2600);
  draw_action(RIGHT, 1000);
  draw_action(DOWN, 550);
  draw_action(LEFT, 750);
  draw_action(DOWN, 550);
  draw_action(RIGHT, 550);
  draw_action(DOWN, 550);
  draw_action(LEFT, 650);
  draw_action(DOWN, 550);
  draw_action(RIGHT, 650);
  draw_action(DOWN, 550);
  draw_action(LEFT, 1350);
  
    // draw E
  draw_action(RIGHT, 1550);
  draw_action(UP, 2600);
  draw_action(RIGHT, 1000);
  draw_action(DOWN, 550);
  draw_action(LEFT, 750);
  draw_action(DOWN, 550);
  draw_action(RIGHT, 550);
  draw_action(DOWN, 550);
  draw_action(LEFT, 650);
  draw_action(DOWN, 550);
  draw_action(RIGHT, 650);
  draw_action(DOWN, 550);
  draw_action(LEFT, 1350);
  delay(5000);

} 

