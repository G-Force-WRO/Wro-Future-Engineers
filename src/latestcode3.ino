#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Servo.h>

const int trigPin0 = A3;
const int echoPin0 = A2;
// const int trigPin1 = A0;
// const int echoPin1 = A1;
// const int trigPin2 = 7;
// const int echoPin2 = 8;
const int servoPin = 2;
int turns = 0;
int center = 0;
double x;
#define spd 3
#define m1 4
#define m2 5
#define stb 6
boolean turning;

Servo servo;

int distance;
long duration;
#include <utility/imumaths.h>
uint16_t BNO055_SAMPLERATE_DELAY_MS = 70;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin0, OUTPUT);
  pinMode(echoPin0, INPUT);
  // pinMode(trigPin1, OUTPUT);
  // pinMode(echoPin1, INPUT);
  // pinMode(trigPin2, OUTPUT);
  // pinMode(echoPin2, INPUT);
  pinMode(spd,OUTPUT);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(stb,OUTPUT);
  pinMode(13,OUTPUT);

  Serial.begin(9600);

  Serial.println("Code begins here ");

  digitalWrite(stb, HIGH);
  servo.attach(servoPin);
  bno.begin();
  servo.write(140);
}



void dist(int t, int e, int k){
  digitalWrite(t, LOW);
  delayMicroseconds(2);
  digitalWrite(t, HIGH);
  delayMicroseconds(10);
  digitalWrite(t, LOW);
  duration = pulseIn(e, HIGH, 30000);
  if(duration * 0.0343/2 != 0){
    distance = duration * 0.0343/2;
  } else {
    distance = 200;
  }
  // Serial.print("u");
  // Serial.print(k);
  // Serial.print(": ");
  // Serial.println(distance);
}

void printEvent(sensors_event_t* event) {
  x = -1000000; //dumb values, easy to spot problem
  
  if (event->type == SENSOR_TYPE_ORIENTATION) {
    // Serial.print("Orient:");
    x = event->orientation.x;
  }
  // Serial.print("\tx= ");
  // Serial.println(x);
}

int comp(int angle, int fixed){
  int a = angle-fixed;
  
  if(a < 0){
    a += 360;
  }

  if(angle - fixed > 30){
    return 30;
  }

  if(angle - fixed < 30){
    return -30;
  }
  
  Serial.println(a);
  return a;
}


void loop()
 {
  // put your main code here, to run repeatedly:
  dist(trigPin0, echoPin0, 0);
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  printEvent(&orientationData);

  
  digitalWrite(m1,  LOW);
  digitalWrite(m2, HIGH);
  analogWrite(spd, 170);  

  if(distance > 100){
    center -= 90;
    if(center == -90){
      center = 270;
    }
    
    digitalWrite(13, HIGH);
    
    while(x != center){
      sensors_event_t orientationData;
      bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
      printEvent(&orientationData);
      servo.write(140+comp(x, center));
    }
  }
    
   else {
    servo.write(140+comp(x, center));
  }
  

}