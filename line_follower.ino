int TRIG_PIN 8
int ECHO_PIN 7
int lastRubyTime=-500;
int flag;
bool PIDyes;
int threashhold[8];
int sensors[8] = {4,12,13,14,25,26,27,35};
int weights[8] = {-100,-9,-6,-5,5,6,9,100};
int oldSums[8] = {0,0,0,0,0};
unsigned long oldTimes[8] = {0,0,0,0,0};
int kp, kd, ki;
float ks;
int led;
int baseSpeedRight;
int baseSpeedLeft;
int rightMotorB;
int leftMotorB;
int rightMotorA;
int leftMotorA;
int pushButton;
int trig, echo;


int getPIDValue(){
  int sum = 0;
  for(int j=0; j<4; j++){
    for(int i=0; i<8; i++){
      sum += getValue(i) * weights[i];
    }
    delayMicroseconds(200);
  }
  unsigned long currentMeasure = millis();
  int value =    kp*sum   +    kd*(sum - oldSums[4])/(3)   +  ki*(sum + oldSums[4])*(3)/2 ;
  for(int i=1;i<4; i++){
    oldSums[i] = oldSums[i-1];
    oldTimes[i] = oldTimes[i-1];
  }
  oldSums[0] = sum;
  oldTimes[0] = currentMeasure;
  return value;
}

int getValue(int sensor){
  return( analogRead(sensors[sensor]) > threashhold[sensor]);
}

void calibrate(){
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(500);
  int sumBlack[8] = {0,0,0,0,0,0,0,0};
  for(int j = 0; j<100; j++){
    for(int i=0; i<8; i++){
      sumBlack[i] += analogRead(sensors[i]);
    }
    digitalWrite(led, HIGH);
    delay(15);
    digitalWrite(led, LOW);
    delay(15);
  }
  digitalWrite(led, HIGH);
  delay(3000);
  digitalWrite(led, LOW);
  delay(500);
  int sumWhite[8] = {0,0,0,0,0,0,0,0};
  for(int j = 0; j<100; j++){
    for(int i=0; i<8; i++){
      sumWhite[i] += analogRead(sensors[i]);
    }
    digitalWrite(led, HIGH);
    delay(15);
    digitalWrite(led, LOW);
    delay(15);
  }
  for(int i = 0; i <8; i++){
    threashhold[i] = (sumBlack[i] + sumWhite[i])/200;
  }
}

void calibrate2(){
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(500);
  int sumBlack[8] = {4000,4000,4000,4000,4000,4000,4000,4000};
  for(int j = 0; j<100; j++){
    for(int i=0; i<8; i++){
      if(analogRead(sensors[i]) < sumBlack[i]){
        sumBlack[i] = analogRead(sensors[i]);
      }
    }
    digitalWrite(led, HIGH);
    delay(15);
    digitalWrite(led, LOW);
    delay(15);
  }
  digitalWrite(led, HIGH);
  delay(3000);
  digitalWrite(led, LOW);
  delay(500);
  int sumWhite[8] = {0,0,0,0,0,0,0,0};
  for(int j = 0; j<100; j++){
    for(int i=0; i<8; i++){
      if(analogRead(sensors[i]) > sumBlack[i]){
        sumBlack[i] = analogRead(sensors[i]);
      }
    }
    digitalWrite(led, HIGH);
    delay(15);
    digitalWrite(led, LOW);
    delay(15);
  }
  for(int i = 0; i <8; i++){
    threashhold[i] = (sumBlack[i] + sumWhite[i])/2;
  }
}

void speedRight(int speed){
  analogWrite(rightMotorB, speed);
}
void speedLeft(int speed){
  analogWrite(leftMotorB, speed);
}

int ruby()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration= pulseIn(ECHO_PIN, HIGH);

  float distance = duration* 0.034/2;

  if(distance <=25){
    return (1);
  }
  return 0;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  flag = 0;
  PIDyes = false;
  kp = 1;
  kd = 1;
  ki = 0;
  ks = 1.3;
  led = 32;
  pushButton = 33;
  trig = 5;
  echo = 18;
  baseSpeedRight = 152;
  baseSpeedLeft = 130;
  rightMotorB = 21;
  leftMotorB = 23;
  rightMotorA = 19;
  leftMotorA = 22;
  pinMode(led, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(rightMotorB,OUTPUT);
  pinMode(leftMotorB,OUTPUT);
  pinMode(rightMotorA,OUTPUT);
  pinMode(leftMotorA,OUTPUT);
  delay(1000);
  calibrate();
  while (digitalRead(pushButton) == 1){
    delay(1);
  };
  delay(1000);
  analogWrite(rightMotorA, 0);
  analogWrite(leftMotorA, 0);
  
  speedRight(baseSpeedRight);
  speedLeft(baseSpeedLeft);
  Serial.begin(9600);
}
void loop() {
<<<<<<< HEAD
  /*
  speedRight(min(150,  (int)( (baseSpeedRight + getPIDValue() * ks ) )) );
  speedLeft(min(150,  (int)( (baseSpeedLeft - getPIDValue() * ks ) ) ) );
  */
  if (millis()-1000>lastRubyTime && ruby())
  {
    lastRubyTime=millis();
    speedRight(0);
    speedLeft(0);
    digitalWrite(led,HIGH);
    delay(3000);
  }
=======
  speedRight(min(255,  (int)( (baseSpeedRight + getPIDValue() * ks ) )) );
  speedLeft(min(255,  (int)( (baseSpeedLeft - getPIDValue() * ks ) ) ) );
>>>>>>> 48d85f86986b39103573c82324493eddcd7e4952
  //Serial.println(analogRead(sensors[0]));
}
