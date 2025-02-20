int flag;
bool PIDyes;
int threashhold[8];
int sensors[8] = {4,12,13,14,25,26,27,35};
int weights[8] = {-15,-9,-6,-5,5,6,9,15};
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
  kp = 1;
  kd = 1;
  ki = 0;
  ks = 1.0;
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
  speedRight(min(255,  (int)( (baseSpeedRight + getPIDValue() * ks ) )) );
  speedLeft(min(255,(int)( (baseSpeedLeft - getPIDValue() * ks ) ) ) );
  
}
