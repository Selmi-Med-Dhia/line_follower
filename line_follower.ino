int flag;
bool PIDyes;
int threashhold;
int sensors[8] = {A0,A1,0,0,0,0,0,0};
int sensorCount = 2;
int oldsum;
int lastMeasure;
int kp, kd, ki, ks;
int led;
int baseSpeedRight;
int baseSpeedLeft;
int rightMotorB;
int leftMotorB;
int rightMotorA;
int leftMotorA;


int getPIDValue(){
  int sum = 0;
  for(int j=0; j<4; j++){
    for(int i=0; i<sensorCount/2; i++){
      sum += getValue(i);
    }
    for(int i=sensorCount/2; i<sensorCount; i++){
      sum -= getValue(i);
    }
    delay(1);
  }
  int currentMeasure = millis();
  int value =    kp*sum   +    kd*(sum - oldsum)/(currentMeasure-lastMeasure)   +  ki*(sum - oldsum)*(currentMeasure-lastMeasure)/2 ;
  lastMeasure = currentMeasure;
  oldsum = sum;
  return value;
}

int getValue(int sensor){
  return( analogRead(sensors[sensor]) > threashhold);
}

void calibrate(){
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(500);
  int sumBlack = 0;
  for(int i=0; i<10; i++){
    sumBlack += analogRead(sensors[0]);
    digitalWrite(led, HIGH);
    delay(20);
    digitalWrite(led, LOW);
    delay(20);
  }
  digitalWrite(led, HIGH);
  delay(3000);
  digitalWrite(led, LOW);
  delay(500);
  int sumWhite = 0;
  for(int i=0; i<10; i++){
    sumWhite += analogRead(sensors[0]);
    digitalWrite(led, HIGH);
    delay(20);
    digitalWrite(led, LOW);
    delay(20);
  }
  threashhold = (sumBlack + sumWhite)/20;
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
  oldsum = 0;
  lastMeasure = millis();
  kp = 1;
  kd = 2;
  ki = 0;
  ks = 18;
  led = 0;
  threashhold = 800;
  baseSpeedRight = 120;
  baseSpeedLeft = 100;
  rightMotorB = 11;
  leftMotorB = 6;
  rightMotorA = 10;
  leftMotorA = 9;
  analogWrite(rightMotorA, 0);
  analogWrite(leftMotorA, 0);
  pinMode(led, OUTPUT);
  //calibrate();
  delay(5000);
  speedLeft(baseSpeedLeft);
  speedRight(baseSpeedRight);
  Serial.begin(9600);
}

void loop() {
  speedRight((baseSpeedRight - getPIDValue() * ks)%255 );
  speedLeft((baseSpeedLeft + getPIDValue() * ks)%255 );
}
