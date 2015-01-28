#include <AFMotor.h>
 
AF_DCMotor motor2(2, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor #2, 64KHz pwm
 
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");
  
  motor1.setSpeed(160);     // set the speed to 200/255
  motor2.setSpeed(200);     // set the speed to 200/255
}
 
void loop() {
 
  motor1.run(BACKWARD);     // the other way
  motor2.run(BACKWARD);     // the other way
  delay(1500);
  
  motor1.run(BACKWARD);   // turn it on going forward
  motor2.run(RELEASE);      // turn it on going forward
  delay(3000);
  
  motor1.run(BACKWARD);     // the other way
  motor2.run(BACKWARD);     // the other way
  delay(6000);
 
  motor1.run(RELEASE);      // stopped
  motor2.run(RELEASE);      // stopped
  delay(1000);
}
