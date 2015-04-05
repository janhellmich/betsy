#include <Servo.h>

// This arduino is responsible to play the Rubik's Cube and the Simon Says game


// RUBIK'S CUBE CONSTANTS

// pin assignment 
#define PIN_RK_ARM 11
#define RK_ARM_START 180
#define RK_ARM_FINISH 50

#define PIN_RK_TURN 10

// SIMON SAYS CONSTANTS

// COMMUNICATION
#define SIGNAL_IN 12
#define SIGNAL_OUT 13

//Servo motor assignment
Servo rkArm;
Servo rkTurn;
Servo ssArm;
Servo ssStart;
Servo ssBlueYellow;
Servo ssGreenRed;


void setup()
{
  Serial.begin(9600);
  // define communication 
  pinMode(SIGNAL_IN, INPUT);
  pinMode(SIGNAL_OUT, OUTPUT);
  
  // initialize signal to low
  digitalWrite(SIGNAL_OUT, LOW);
  
  delay(1000);
  
  // attach RK servo
  rkArm.attach(PIN_RK_ARM);
  rkArm.write(RK_ARM_START);
  
  rkTurn.attach(PIN_RK_TURN);
  rkTurn.write(0);
  
}

void loop()
{
//  // wait for signal to play Simon Says
//  while (digitalRead(SIGNAL_IN) != HIGH)
//  {}
//  
//  play_simon_says();
//  // signal finished with game
//  finished();
//  
  // wait for signal to play Rubiks Cube
  Serial.print(digitalRead(SIGNAL_IN));
  while (digitalRead(SIGNAL_IN) != HIGH)
  {
    Serial.println("NO SIGNAL");
  }
  
  play_rubiks_cube();
  // signal finished with game
  finished();
  
  while (true)
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


/********************* PLAY RUBIK'S CUBE  ************************************************************************/

void play_rubiks_cube()
{
  //lower arm
  for (int i = RK_ARM_START; i > RK_ARM_FINISH; i--)
  {
    rkArm.write(i);
    delay(15);
  }
  
  // turn row
  for (int i = 0; i <= 180; i++)
  {
    rkTurn.write(i);
    delay(15);
  }
  rkTurn.detach();
  
  // raise arm
  for (int i = RK_ARM_FINISH; i < RK_ARM_START; i++)
  {
    rkArm.write(i);
    delay(15);
  }
  
  
}
  
