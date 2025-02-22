int lastRubyTime=-4000;
int flag;
bool PIDyes;
int threashhold[8];
int sensors[8] = {4,12,13,14,25,26,27,35};
int weights[8] = {-255,-9,-6,-5,5,6,9,255};
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
int tmp;
int direction;

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

void speedRight(int speed){
  analogWrite(rightMotorB, speed);
}
void speedLeft(int speed){
  analogWrite(leftMotorB, speed);
}

int ruby(){
  return 0;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration= pulseIn(echo, HIGH, 6000);

  float distance = duration* 0.034/2;
  //delay(1);
  if (distance == 0){
    return 0;
  }
  if(distance <=25){
    return (1);
  }
  return 0;
}

void setup() {
  flag = 0;
  PIDyes = false;
  kp = 1;
  kd = 1;
  ki = 0;
  ks = 2;
  led = 32;
  pushButton = 33;
  trig = 5;
  echo = 18;
  baseSpeedRight = 255;
  baseSpeedLeft = 233;

  rightMotorB = 23;
  leftMotorB = 21;
  rightMotorA = 22;
  leftMotorA = 19;
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
  if (ruby()){
    if (millis() - lastRubyTime> 5000){
      lastRubyTime=millis();
      speedRight(0);
      speedLeft(0);
      digitalWrite(led,HIGH);
      delay(3000);
      digitalWrite(led,LOW);
    }
  }
  tmp = getPIDValue();
  speedRight(max(0, min(178,  (int)( (baseSpeedRight - tmp * ks ) ) ) ) );
  speedLeft(max (0, min(178,  (int)( (baseSpeedLeft + tmp * ks ) ) ) ) );
  /*
  if(tmp > 0){
    //right
    direction = 1;
  }else if(tmp <0){
    //left
    direction = -1;
  }else{
    tmp = 255 * direction;
    speedRight(max(0, min(178,  (int)( (baseSpeedRight - tmp * ks ) ) ) ) );
    speedLeft(max (0, min(178,  (int)( (baseSpeedLeft + tmp * ks ) ) ) ) );
    delay(100);
  }
  */
}
