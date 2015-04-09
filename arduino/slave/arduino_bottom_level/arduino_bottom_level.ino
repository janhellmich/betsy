#include <Servo.h>

// pin assignment
#define PIN_ARMS       3
#define PIN_LEFT_KNOB  9
#define PIN_RIGHT_KNOB 6

// arm constants
#define ARMS_START 180
#define ARMS_FINISH 77

// communication
#define SIGNAL_IN      8
#define SIGNAL_OUT     12



Servo arms;
Servo leftKnob;
Servo rightKnob;

// create drawing direction indicators. Servo 2 should be acting on the up/down direction
int UP    = 2;
int DOWN  = 1;
int RIGHT = 4;
int LEFT  = 3;


// set null speed for both servos
int null_speed = 90;
// set drive speed for both motors
int drive_speed = 8;



void setup() 
{
  
  pinMode(SIGNAL_IN, INPUT);
  pinMode(SIGNAL_OUT, OUTPUT);
  digitalWrite(SIGNAL_OUT, LOW);
  
  arms.attach(PIN_ARMS);
  arms.write(ARMS_START);
  
  delay(1000);
  
}


void loop() 
{
  
  while  (digitalRead(SIGNAL_IN) != HIGH)
  {}                      //Do Nothing
  // play etch a sketch
  
  play_etch_a_sketch();
  
  // signal completion of game
  finished();
  
  // wait for 2nd game
  while  (digitalRead(SIGNAL_IN) != HIGH)
  {}                      //Do Nothing
  
  pick_up_card();
  
  finished();
  
  while  (true)
  {}
  
}

/********************* FUNCTIONS  ************************************************************************/


/********************* FINISHED  ************************************************************************/

void finished() 
{
  
  digitalWrite(SIGNAL_OUT, HIGH);
  delay(1000);
  digitalWrite(SIGNAL_OUT, LOW);
  
}
   
/********************* PICK UP CARD  ************************************************************************/

void pick_up_card()
{    
  
    arms.attach(PIN_ARMS);
    // lower arms    
    for (int i = ARMS_START; i >= ARMS_FINISH; i--)
    {
      arms.write(i);
      delay(30);
    }
    delay(1000);
    //raise arms
    for (int i = ARMS_FINISH; i <= ARMS_FINISH + 15; i++)
    {
      arms.write(i);
      delay(50);
    }
    delay(1000);
    for (int i = ARMS_FINISH + 15; i <= ARMS_START; i++)
    {
      arms.write(i);
      delay(15);
    }    
    
    arms.detach();
    
}

/********************* PLAY ETCH A SKETCH  ************************************************************************/
   
void play_etch_a_sketch()
{
  
    arms.attach(PIN_ARMS);
  
    leftKnob.attach(PIN_LEFT_KNOB);
    rightKnob.attach(PIN_RIGHT_KNOB);
    leftKnob.write(90);
    rightKnob.write(90);

    for (int i = ARMS_START; i >= ARMS_FINISH; i--)
    {
      arms.write(i);
      delay(15);
    }
    
    delay(200);
    
    //draw I
    draw_action(LEFT, 300);
    draw_action(UP, 850);  
    draw_action(RIGHT, 300);
    draw_action(DOWN, 700);
    draw_action(RIGHT, 400);

    draw_e();
    
    draw_e();
    
    draw_e();
    

    for (int i = ARMS_FINISH; i <= ARMS_START; i+=2)
    {
      arms.write(i);
      delay(15);
    }
    
    arms.detach();
    
}


/********************* DRAW ACTION  ************************************************************************/

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

void draw_e()
{
  
      // draw E
    draw_action(UP, 800);
    draw_action(RIGHT, 400);
    draw_action(DOWN, 190);
    draw_action(LEFT, 350);
    draw_action(DOWN, 105);
    draw_action(RIGHT, 400);
    draw_action(DOWN, 105);
    draw_action(LEFT, 350);
    draw_action(DOWN, 105);
    draw_action(RIGHT, 400);
    draw_action(DOWN, 105);
    draw_action(LEFT, 500);
    draw_action(RIGHT, 1000);
    
}
