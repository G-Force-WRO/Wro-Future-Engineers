#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Servo.h>

const int trigPin0 = A3;
const int echoPin0 = A2;
const int trigPin1 = A0;
const int echoPin1 = A1;
const int trigPin2 = 7;
const int echoPin2 = 8;
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

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3-5V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

void setup(void)
{
  pinMode(trigPin0, OUTPUT);
  pinMode(echoPin0, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(spd,OUTPUT);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(stb,OUTPUT);

  servo.attach(servoPin);
  servo.write(105);
  
  Serial.begin(115200);

  while (!Serial) delay(10);  // wait for serial port to open!

  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  bno.begin();

  delay(1000);

  digitalWrite(stb, HIGH);
  turning = false;
}

void loop(void)
{
  //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  
  // dist(trigPin0, echoPin0);
  // servo.write(0);
  dist(trigPin2, echoPin2, 2);
  Serial.println();  
  digitalWrite(m1,  LOW);
  digitalWrite(m2, HIGH);
  if(distance > 100){
    turning = true;
  }
  if(!turning){
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    printEvent(&orientationData);
    servo.write(105-comp(x,center));
    analogWrite(spd, 170);  
  } else {
    center -= 90;
    if (center == -90){
      center = 270;
    }
    while(comp(x,center) != 0){
      servo.write(105-comp(x,center));
      analogWrite(spd, 140); 
    }
    
    // analogWrite(spd, 170);    
    // // digitalWrite(m1,  LOW);
    // // digitalWrite(m2, HIGH);
    // servo.write(30);
    // delay(700);
    // turning = false;
    // center += 90;
    // if (center == 360){
    //   center = 0;
    // }
  }
}

void printEvent(sensors_event_t* event) {
  x = -1000000; //dumb values, easy to spot problem
  
  if (event->type == SENSOR_TYPE_ORIENTATION) {
    Serial.print("Orient:");
    x = event->orientation.x;
  }
  Serial.print("\tx= ");
  Serial.println(x);
}

void dist(int t, int e, int k) {
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
  Serial.print("u");
  Serial.print(k);
  Serial.print(": ");
  Serial.println(distance);
  // Serial.println(distance);
}

int comp(int angle, int fixed){
  if(fixed == 0 && angle >180){
    fixed = 360;
  }
  if(fixed == 360 && angle<180){
    fixed = 0;
  }

  if(angle - fixed > 50){
    return 50;
  }

  if(angle - fixed < 50){
    return -50;
  }
  
  Serial.println(angle-fixed);
  return(angle-fixed);
}