#include <Servo.h>

#define PIN_LEFT_ARM 11
#define PIN_RIGHT_ARM 10
#define PIN_LEFT_KNOB 12
#define PIN_RIGHT_KNOB 13

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
// rightArm.attach(PIN_RIGHT_ARM);
// leftArm.attach(PIN_LEFT_ARM);
// rightArm.write(180);
// leftArm.write(180);
// delay(3000);
} 

void loop() 
{ 
  leftKnob.attach(PIN_LEFT_KNOB);
  rightKnob.attach(PIN_RIGHT_KNOB);
  leftKnob.write(90);
  rightKnob.write(90);
  
  
  
  rightArm.write(110);
  leftArm.write(110);
  for (int i = 110; i >= 90; i--) 
  {
    rightArm.write(i);
    leftArm.write(i);
    delay(20);
  }
  delay(3000);                      
  
  
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
