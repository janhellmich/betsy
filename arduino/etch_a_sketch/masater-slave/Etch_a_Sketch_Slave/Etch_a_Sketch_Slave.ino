#include <Servo.h>

#define PIN_ARMS 3
#define PIN_LEFT_KNOB 9
#define PIN_RIGHT_KNOB 6

Servo Arms;
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

  switch(draw_dir)
  {
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
    rightKnob.detach();
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
  pinMode(8, INPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  Arms.attach(PIN_ARMS);
  Arms.write(180);
  delay(3000);
}
void loop() 
{
  while  (digitalRead(8) != HIGH)
  {}                      //Do Nothing
  while  (digitalRead(8) == HIGH)
  {
    leftKnob.attach(PIN_LEFT_KNOB);
    rightKnob.attach(PIN_RIGHT_KNOB);
    leftKnob.write(90);
    rightKnob.write(90);

    for (int i = 180; i >= 77; i--)
    {
      Arms.write(i);
      delay(15);
    }
    delay(3000);
    //draw I
    draw_action(LEFT, 300);
    draw_action(UP, 680);  
    draw_action(RIGHT, 300);
    draw_action(DOWN, 700);
    draw_action(RIGHT, 400);

    // draw E
    draw_action(UP, 750);
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
    draw_action(LEFT, 600);
    draw_action(RIGHT, 850);

    // draw E
    draw_action(UP, 750);
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
    draw_action(LEFT, 600);
    draw_action(RIGHT, 850);

    // draw E
    draw_action(UP, 750);
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
    draw_action(LEFT, 600);
    draw_action(RIGHT, 850);

    for (int i = 85; i <= 180; i+=2)
    {
      Arms.write(i);
      delay(15);
    }
    delay(1000);
    digitalWrite(12, HIGH);    //Let the Mega know we are done
    finished();
  }
}

void finished() 
{
  while (true)
  {
    delay(2000);
  }
}
   
