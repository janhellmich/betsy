#include <Servo.h>

#define PIN_LEFT_ARM 6
#define PIN_RIGHT_ARM 11
#define PIN_LEFT_KNOB 10
#define PIN_RIGHT_KNOB 9

Servo leftArm;
Servo rightArm;
Servo leftKnob;
Servo rightKnob;

// create drawing direction indicators. Servo 2 should be acting on the up/down direction
int UP = 2;
int DOWN = 1;
int RIGHT = 4;
int LEFT = 3;


// set null speed for both servos
int null_speed = 90;
// set drive speed for both motors
int drive_speed = 8;


void draw_action(int draw_dir, int seconds)
{
  turn_value = 0;
  switch(draw_dir){
  case 1:
    leftKnob.attach(PIN_LEFT_KNOB);  
    leftKnob.write(null_speed + drive_speed);  
    delay(seconds);
    leftKnob.detach();
    break;
  case 2:
    leftKnob.attach(PIN_LEFT_KNOB);
    leftKnob.write(null_speed - drive_speed);  
    delay(seconds);
    leftKnob.detach();
    break;
  case 3:
    rightKnob.attach(PIN_RIGHT_KNOB); 
    rightKnob.write(null_speed + drive_speed);  
    delay(seconds);
    break;
  case 4:
    rightKnob.attach(PIN_RIGHT_KNOB); 
    rightKnob.write(null_speed - drive_speed);  
    delay(seconds);
    rightKnob.detach();
  }
}


void setup() 
{ 
 rightArm.attach(PIN_RIGHT_ARM);
 leftArm.attach(PIN_LEFT_ARM);
 rightArm.write(180);
 leftArm.write(180);
 delay(3000);
} 

void loop() 
{ 
  
  rightArm.write(90);
  leftArm.write(90);
  
  delay(3000);                      
  
  Serial.println("Drawing I");
  draw_action(DOWN, 700);
  draw_action(RIGHT, 400);
  
  Serial.println("Drawing E");
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
  
 Serial.println("Drawing E");
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
  
  Serial.println("Drawing E");
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
  
  rightArm.write(180);
  leftArm.write(180);
  delay(1000);                       // waits 15ms for the servo to reach the position 
  finished();
  
}

void finished() {
  while (true) 
  {
    delay(2000);
  }
}
