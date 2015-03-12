//motor A connected between A01 and A02
//motor B connected between B01 and B02

int STBY = 24; //standby

//Motor A
int PWMA = 2; //Speed control 
int AIN1 = 23; //Direction
int AIN2 = 22; //Direction

//Motor B
int PWMB = 3; //Speed control
int BIN1 = 25; //Direction
int BIN2 = 26; //Direction

void setup(){
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop(){
  move(1, 128, 1); //motor 1, half speed, left
  //move(2, 128, 1); //motor 2, half speed, left

  delay(1000); //go for 1 second
  stop(); //stop
  delay(250); //hold for 250ms until move again

  move(1, 128, 0); //motor 1, half speed, right
  //move(2, 128, 0); //motor 2, half speed, right

  delay(1000);
  stop();
  delay(250);
}


void move(int motor, int spd, int dir){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(dir == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, spd);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, spd);
  }
}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}
