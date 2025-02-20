int flag;
bool PIDyes;
int threashhold[8];
int sensors[8] = {4,12,13,14,25,26,27,35};
int weights[8] = {100,80,70,50,50,70,80,100};
int oldSums[8] = {0,0,0,0,0};
int oldTimes[8] = {0,0,0,0,0};
int lastMeasure;
int kp, kd, ki, ks;
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
    for(int i=0; i<4; i++){
      sum += getValue(i) * weights[i];
    }
    for(int i=4; i<8; i++){
      sum -= getValue(i) * weights[i];
    }
    delayMicroseconds(100);
  }
  int currentMeasure = millis();
  int value =    kp*sum   +    kd*(sum - oldSums[4])/(currentMeasure-oldTimes[4])   +  ki*(sum + oldSums[4])*(currentMeasure-oldTimes[4])/2 ;
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
  for(int j = 0; j<15; j++){
    for(int i=0; i<8; i++){
      sumBlack[i] += analogRead(sensors[i]);
    }
    digitalWrite(led, HIGH);
    delay(20);
    digitalWrite(led, LOW);
    delay(20);
  }
  digitalWrite(led, HIGH);
  delay(3000);
  digitalWrite(led, LOW);
  delay(500);
  int sumWhite[8] = {0,0,0,0,0,0,0,0};
  for(int j = 0; j<15; j++){
    for(int i=0; i<8; i++){
      sumWhite[i] += analogRead(sensors[i]);
    }
    digitalWrite(led, HIGH);
    delay(20);
    digitalWrite(led, LOW);
    delay(20);
  }
  for(int i = 0; i <8; i++){
    threashhold[i] = (sumBlack[i] + sumWhite[i])/30;
  }
}

void speedRight(int speed){
  analogWrite(rightMotorB, speed);
}
void speedLeft(int speed){
  analogWrite(leftMotorB, speed);
}

void setup() {
  flag = 0;
  PIDyes = false;
  lastMeasure = millis();
  kp = 1;
  kd = 2;
  ki = 0;
  ks = 18;
  led = 32;
  pushButton = 33;
  trig = 5;
  echo = 18;
  baseSpeedRight = 152;
  baseSpeedLeft = 130;
  rightMotorB = 19;
  leftMotorB = 22;
  rightMotorA = 21;
  leftMotorA = 23;
  delay(1000);
  analogWrite(rightMotorA, 0);
  analogWrite(leftMotorA, 0);
  /*
  speedRight(baseSpeedRight);
  speedLeft(baseSpeedLeft);
  */
  Serial.begin(9600);
}

void loop() {
  /*
  speedRight((baseSpeedRight - getPIDValue() * ks)%255 );
  speedLeft((baseSpeedLeft + getPIDValue() * ks)%255 );
  */
  Serial.println(analogRead(sensors[0]));
}
