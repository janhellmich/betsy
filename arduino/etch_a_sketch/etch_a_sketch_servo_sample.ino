/* 
 Controlling a servo position using a potentiometer (variable resistor) 
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott> 

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>
#include <AFMotor.h> 

Servo myservo1;
Servo myservo2;// create servo object to control a servo 
 
//int potpin = 0;  // analog pin used to connect the potentiometer
//int val;    // variable to read the value from the analog pin 
// 
void setup() 
{ 
  
  myservo2.attach(10);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 
//  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
//myse  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180) 
  myservo1.attach(9);
  myservo1.write(82);     
  // sets the servo position according to the scaled value 
  delay(1000);

  myservo1.detach();
  myservo2.attach(10); 
  myservo2.write(98);  
  delay(1000);
  
  myservo2.detach();
  
  
  
  
  // waits for the servo to get there 
} 
