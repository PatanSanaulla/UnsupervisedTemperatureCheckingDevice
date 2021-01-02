#include <Wire.h>
#include <Adafruit_MLX90614.h>

//constants
const int Trigger_Pin = 2;
const int Echo_Pin = 3;
const int Led_Pin = 13;
const int Servo_Pin = 10;
const int Laser_Pin = 5;
bool Tested;
bool Allowed;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  pinMode(Led_Pin, OUTPUT);
  pinMode(Servo_Pin, OUTPUT);
  pinMode(Echo_Pin, INPUT);
  pinMode(Trigger_Pin,OUTPUT);
  pinMode(Laser_Pin,OUTPUT);
  bool Tested = false;
  bool Allowed = false;
  mlx.begin();
}

void loop() {
  if (Tested == false){
    Serial.println("Welcome, person to be tested!");
    blinkLight();
    moveSensor();
  }
  if (Allowed == false){
    digitalWrite(Led_Pin, HIGH);
    Serial.println("Please, Do not enter!");
  }
  if (Allowed == true){
    digitalWrite(Led_Pin, LOW);
    Serial.println("You are free to enter!");
  }
}

void blinkLight(){
  digitalWrite(Led_Pin, HIGH);
  delay(1000);
  digitalWrite(Led_Pin, LOW);
  delay(1000);
  Serial.println("LED blinking!");
}

void angle(int a){
  int highDur, lowDur;
  highDur = map(a, 0, 180, 500, 2600);
  lowDur = 20000 - highDur;

  digitalWrite(Servo_Pin, HIGH);
  delayMicroseconds(highDur);
  digitalWrite(Servo_Pin, LOW);
  delayMicroseconds(lowDur);
  Serial.println("Moving the servo !");
}

void moveSensor(){
  int pos;
  int rotation = 0;
  for (pos = 90; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
    Serial.print(pos);
    angle(pos);              // tell servo to go to position in variable 'pos'
    //dist();                  // Calculate the distance to the nearest object
    delay(1000);               // waits 15ms for the servo to reach the position
  }
  for (pos = 0; pos <= 90; pos += 10) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serial.print(pos);
    angle(pos);              // tell servo to go to position in variable 'pos'
    bool personFound = distanceSensor();   // Calculate the distance to the nearest object
    if(personFound == true){
      rotation = pos;
      getSensorData();
      break;
    }
    delay(1000);               // waits 15ms for the servo to reach the position
  }

}

void getSensorData(){
  float AmbientTemp = mlx.readAmbientTempF();
  float ObjectTemp = mlx.readObjectTempF();
  Tested = true;
  if (ObjectTemp >= 100.4){
    Allowed = false;
  }else{
    Allowed = true;
  }
  Serial.print("Your body temperature is :");
  Serial.print(ObjectTemp);
  Serial.print(" degrees Farenheit");
  digitalWrite(Laser_Pin, HIGH);
  delay(500);
}

bool distanceSensor(){
  long dur, dist = -1;

  digitalWrite(Trigger_Pin, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trigger_Pin, LOW);

  dur = pulseIn(Echo_Pin, HIGH);
  dist = (dur/2) * 0.0343;
  
  if(dist >= 50){
    Serial.println("Out of range");
    return false;
    //Serial.println(" No"); //Print No obstacle near a range of 200 cms at the particular angle.
  }else{
    Serial.print(dist);
    Serial.println(" cm");
    Serial.println("person found!");
    return true;
    //Serial.println(" Yes"); //Print obstacle within a range of 200 cms at the particular angle.
  }
}
