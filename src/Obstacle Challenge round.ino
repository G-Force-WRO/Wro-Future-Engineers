#include <Adafruit_BNO055.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include <TimerOne.h>
#include <Pixy2.h>

// BNO055 object
Adafruit_BNO055 bno = Adafruit_BNO055(55);
//TCS3475 object 
Adafruit_TCS34725

//TCS3475 object 
// Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//pixy2 object
Pixy2 pixy;
// Servo object
Servo servo;

//Global Variables
bool steering_tracking = true;
int setpoint[13] = {0,271,183,92,1,271,183,92,1,271,183,92,1} ;
int currentSetpoint = 357;

// Constants
const int FRONT_OBSTACLE_DISTANCE = 70;
const int SAFE_LEFT_DISTANCE = 100;
const int TURN_TARGET_ANGLE = 280;

// // Steering Constrains
// const int front = 103;
// const int left = 70;
// const int right = 135;


const int front = 45;
const int left = 0;
const int right = 90;



// Pins definition for motor, servo, and ultrasonic sensors
const int SERVO_PIN = 2;
const int MOTOR1_PIN1 = 4;
const int MOTOR1_PIN2 = 5;
const int ENABLE_PIN = 3;
// const int TRIG_PIN[] = {A0, 7, A2}; // {FirstSensor, SecondSensor, ThirdSensor}
// const int ECHO_PIN[] = {A1, 8, A3}; // {FirstSensor, SecondSensor, ThirdSensor}

const int TRIG_PIN[] = {8, A1, A3}; // {FirstSensor, SecondSensor, ThirdSensor}
const int ECHO_PIN[] = {7, A0, A2}; // {FirstSensor, SecondSensor, ThirdSensor}

// Enum for ultrasonic sensors
enum { FRONT = 0, LEFT = 1, RIGHT = 2 };



// For steering angle smoothing
float previousServoAngle = front;  // starts with the front direction
const float WEIGHT = 1;  // you can adjust this value to tune the smoothing

// Sensor data structure
struct SensorData {
    float angle;
    float distances[3];
    long color;
};

struct ObjectData {
  uint16_t signature;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
};


// Create an instance of the structure
SensorData sensorData;
ObjectData firstObject;

///////////////////////////////////////////////////// Functions/////////////////////////////////////////////////////                                                                                                            

//function to get angle
float get_angle_z() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return euler.x();
}



//update_angle function
void update_angle() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    sensorData.angle = euler.x();

}

//function to get color
long get_color() {
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);
    return c;
}

//function to get distance
int get_distance(int sensor) {
    digitalWrite(TRIG_PIN[sensor], LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN[sensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN[sensor], LOW);
    long duration = pulseIn(ECHO_PIN[sensor], HIGH, 6000);
    if (duration == 0) {
        return 101; // value greater than 100 indicating out of range or timeout
    }
    return (duration/2) / 29.1;
}

//function to get filtered distance
float filtered_distance(int sensor) {
    float filter = 0;
    for (int i = 0; i < 5; i++) {
        filter += get_distance(sensor);
    }
    return filter / 5;
}

//function to update sensor data
void updateSensorData() {
    sensorData.angle = get_angle_z();
    sensorData.distances[FRONT] = filtered_distance(FRONT);
    sensorData.distances[LEFT] = filtered_distance(LEFT);
    sensorData.distances[RIGHT] = filtered_distance(RIGHT);
}

//function to update blockdata 

void updateBlockData() {
    pixy.ccc.getBlocks();
    if (pixy.ccc.numBlocks)
    {
        firstObject.signature = pixy.ccc.blocks[0].m_signature;
        firstObject.x = pixy.ccc.blocks[0].m_x;
        firstObject.y = pixy.ccc.blocks[0].m_y;
        firstObject.width = pixy.ccc.blocks[0].m_width;
        firstObject.height = pixy.ccc.blocks[0].m_height;
    }
    
}


// void updateSensorData() {
//     sensorData.angle = get_angle_z();
//     sensorData.distances[FRONT] = get_distance(FRONT);
//     sensorData.distances[LEFT] = get_distance(LEFT);
//     sensorData.distances[RIGHT] = get_distance(RIGHT);
//     sensorData.color = get_color();
// }



//function to set motor speed and start motor
void set_motor(int speed) {
    analogWrite(ENABLE_PIN, speed);
    digitalWrite(MOTOR1_PIN1, LOW);
    digitalWrite(MOTOR1_PIN2, HIGH);
    digitalWrite(6, HIGH);
}


//function to stop motor
void stop_motor() {
    analogWrite(ENABLE_PIN, 0);
    digitalWrite(MOTOR1_PIN1, LOW);
    digitalWrite(MOTOR1_PIN2, LOW);
    digitalWrite(6, LOW);
}

//function to correct steering
int steringCorrection(int bnoValue, int straight_setpoint) {
    // Normalize BNO value using modulo and conditional logic
    // Calculate deviation from the desired straight direction
    int deviation = bnoValue - straight_setpoint;
  
    // Handle wrap-around for values close to 360
    if (deviation > 180) {
        deviation -= 360;
    } else if (deviation < -180) {
        deviation += 360;
    }

    // Calculate corrected servo position
    int correctedServoPosition = front - deviation; 

    // Ensure the corrected value is within servo limits
    return constrain(correctedServoPosition, left, right);
    //return correctedServoPosition;
}

//function to set servo angle
void set_steer(int angle) {
    servo.write(angle);
}


// void set_steer(int angle) {
//     // Calculate the weighted average of the current angle and the previous angle
//     float smoothedAngle = WEIGHT * previousServoAngle + (1 - WEIGHT) * angle;
    
//     servo.write((int)smoothedAngle);
//     previousServoAngle = smoothedAngle;  // update the previous angle with the smoothed angle
// }



//function to print sensor data
void print_sensor_data() {
    Serial.print("Angle: ");
    Serial.print(sensorData.angle);
    
    Serial.print(" | Front Distance: ");
    Serial.print(sensorData.distances[FRONT]);
    
    Serial.print(" | Left Distance: ");
    Serial.print(sensorData.distances[LEFT]);
    
    Serial.print(" | Right Distance: ");
    Serial.print(sensorData.distances[RIGHT]);

    // Serial.print(" | Color: ");
    // Serial.println(sensorData.color);
    
}


//function to stop tracking
void stop_steeringTracking() {
  steering_tracking = false;
}

//function to start tracking
void start_steeringTracking() {
  steering_tracking = true;
}

//function to set heading
int set_heading(int heading) {
  
    if (steering_tracking == true) {
  
    float current_angle = sensorData.angle;
    int servoAngle = steringCorrection(current_angle, heading);
    set_steer(servoAngle);
    Serial.print(steering_tracking);
    Serial.print("\tCorrected Servo Value: ");
    Serial.print(servoAngle);
    Serial.print("\tSet point: ");
    Serial.println(heading);
    return servoAngle;
    }    
}

void setup() {

    // Initialize serial communication
    Serial.begin(115200);

    // Initialize BNO055 sensor
    if (!bno.begin()) {
        Serial.print("No BNO055 detected");
        while (1);
    }

    pixy.init();
    pixy.setLamp(1, 0);

    pinMode(6, OUTPUT);
    servo.attach(SERVO_PIN);
    pinMode(MOTOR1_PIN1, OUTPUT);
    pinMode(MOTOR1_PIN2, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    for (int i = FRONT; i <= RIGHT; i++) {
        pinMode(TRIG_PIN[i], OUTPUT);
        pinMode(ECHO_PIN[i], INPUT);
    }
    
    // set_steer(108); // turn straight
    delay(1000);
    set_motor(200); // start moving
    //set TCP34725
    // if (tcs.begin()) {
    //     Serial.println("TCS34725 found");
    // } else {
    //     Serial.println("No TCS34725 found");
    //     while (1);
    // }
}

void loop() {
    pixy.setLamp(1, 0);
    // Update sensor data
    updateSensorData();
    updateBlockData();
    // Print sensor data
    //print_sensor_data();
    start_steeringTracking();
    set_heading(currentSetpoint); // (heading, steering tracking flag)

    int i = 1;
    while (i<=12)
    {   
        updateSensorData();
        updateBlockData();
        set_heading(currentSetpoint);

        if (firstObject.signature == 2 && firstObject.height >=40)
        {
            stop_steeringTracking();
            set_steer(120);
            delay(600);
            //updateSensorData();
            // if(sensorData.distances[left]<10)
            // {
            //     set_steer(100);
            //     delay(500);
            // }
            set_steer(70);
            delay(500);
            set_steer(95);
            start_steeringTracking();
            set_heading(currentSetpoint);
            updateSensorData();
            updateBlockData();
        }

        if (firstObject.signature == 1 && firstObject.height >=40)
        {
            stop_steeringTracking();
            set_steer(75);
            delay(500);
            updateSensorData();
            // if(sensorData.distances[left]<10)
            // {
            //     set_steer(100);
            //     delay(500);
            // }
            set_steer(120);
            delay(500);
            set_steer(95);
            start_steeringTracking();
            set_heading(currentSetpoint);
            updateSensorData();
            updateBlockData();
        }

        if (sensorData.distances[FRONT] < FRONT_OBSTACLE_DISTANCE && sensorData.distances[LEFT] == 101) {
            stop_steeringTracking();
            print_sensor_data();
            set_motor(170);
            set_steer(70);
            while(sensorData.angle >= setpoint[i])
            {   
                // set_motor(200);
                // Serial.println("****Entered While *********");
                update_angle();
                // Serial.println("****Turning Left*********");
                // print_sensor_data();
                // Serial.print("  Setpoint: ");
                // Serial.print(currentSetpoint);/
                Serial.print("Current Angle: ");
                Serial.print(sensorData.angle);
                Serial.print(" | Current Setpoint: ");
                Serial.println(setpoint[i]);

            }
            updateSensorData();
            Serial.println("****Exited While *********");
            currentSetpoint = setpoint[i];
            start_steeringTracking();
            set_heading(currentSetpoint);
            set_motor(200);
            Serial.print("  Setpoint: ");
            Serial.print(currentSetpoint);
            i=i+1;
            delay(500);
          

         }
    
    }
    stop_motor();
    while (1)
    {
        /* code */
    }
    
}


725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
//pixy2 object
Pixy2 pixy;
// Servo object
Servo servo;

//Global Variables
bool steering_tracking = true;
int setpoint[13] = {0,270,180,95,8,270,180,95,5,270,180,95,8} ;
int currentSetpoint = 357;

// Constants
const int FRONT_OBSTACLE_DISTANCE = 70;
const int SAFE_LEFT_DISTANCE = 100;
const int TURN_TARGET_ANGLE = 280;

// // Steering Constrains
// const int front = 103;
// const int left = 70;
// const int right = 135;


const int front = 95;
const int left = 65;
const int right = 120;



// Pins definition for motor, servo, and ultrasonic sensors
const int SERVO_PIN = 2;
const int MOTOR1_PIN1 = 4;
const int MOTOR1_PIN2 = 5;
const int ENABLE_PIN = 3;
// const int TRIG_PIN[] = {A0, 7, A2}; // {FirstSensor, SecondSensor, ThirdSensor}
// const int ECHO_PIN[] = {A1, 8, A3}; // {FirstSensor, SecondSensor, ThirdSensor}

const int TRIG_PIN[] = {8, A1, A3}; // {FirstSensor, SecondSensor, ThirdSensor}
const int ECHO_PIN[] = {7, A0, A2}; // {FirstSensor, SecondSensor, ThirdSensor}

// Enum for ultrasonic sensors
enum { FRONT = 0, LEFT = 1, RIGHT = 2 };



// For steering angle smoothing
float previousServoAngle = front;  // starts with the front direction
const float WEIGHT = 1;  // you can adjust this value to tune the smoothing

// Sensor data structure
struct SensorData {
    float angle;
    float distances[3];
    long color;
};

struct ObjectData {
  uint16_t signature;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
};


// Create an instance of the structure
SensorData sensorData;
ObjectData firstObject;

///////////////////////////////////////////////////// Functions/////////////////////////////////////////////////////                                                                                                            

//function to get angle
float get_angle_z() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    return euler.x();
}



//update_angle function
void update_angle() {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    sensorData.angle = euler.x();

}

//function to get color
long get_color() {
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);
    return c;
}

//function to get distance
int get_distance(int sensor) {
    digitalWrite(TRIG_PIN[sensor], LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN[sensor], HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN[sensor], LOW);
    long duration = pulseIn(ECHO_PIN[sensor], HIGH, 6000);
    if (duration == 0) {
        return 101; // value greater than 100 indicating out of range or timeout
    }
    return (duration/2) / 29.1;
}

//function to get filtered distance
float filtered_distance(int sensor) {
    float filter = 0;
    for (int i = 0; i < 5; i++) {
        filter += get_distance(sensor);
    }
    return filter / 5;
}

//function to update sensor data
void updateSensorData() {
    sensorData.angle = get_angle_z();
    sensorData.distances[FRONT] = filtered_distance(FRONT);
    sensorData.distances[LEFT] = filtered_distance(LEFT);
    sensorData.distances[RIGHT] = filtered_distance(RIGHT);
}

//function to update blockdata 

void updateBlockData() {
    pixy.ccc.getBlocks();
    if (pixy.ccc.numBlocks)
    {
        firstObject.signature = pixy.ccc.blocks[0].m_signature;
        firstObject.x = pixy.ccc.blocks[0].m_x;
        firstObject.y = pixy.ccc.blocks[0].m_y;
        firstObject.width = pixy.ccc.blocks[0].m_width;
        firstObject.height = pixy.ccc.blocks[0].m_height;
    }
    
}


// void updateSensorData() {
//     sensorData.angle = get_angle_z();
//     sensorData.distances[FRONT] = get_distance(FRONT);
//     sensorData.distances[LEFT] = get_distance(LEFT);
//     sensorData.distances[RIGHT] = get_distance(RIGHT);
//     sensorData.color = get_color();
// }



//function to set motor speed and start motor
void set_motor(int speed) {
    analogWrite(ENABLE_PIN, speed);
    digitalWrite(MOTOR1_PIN1, LOW);
    digitalWrite(MOTOR1_PIN2, HIGH);
    digitalWrite(6, HIGH);
}


//function to stop motor
void stop_motor() {
    analogWrite(ENABLE_PIN, 0);
    digitalWrite(MOTOR1_PIN1, LOW);
    digitalWrite(MOTOR1_PIN2, LOW);
    digitalWrite(6, LOW);
}

//function to correct steering
int steringCorrection(int bnoValue, int straight_setpoint) {
    // Normalize BNO value using modulo and conditional logic
    // Calculate deviation from the desired straight direction
    int deviation = bnoValue - straight_setpoint;
  
    // Handle wrap-around for values close to 360
    if (deviation > 180) {
        deviation -= 360;
    } else if (deviation < -180) {
        deviation += 360;
    }

    // Calculate corrected servo position
    int correctedServoPosition = front - deviation; 

    // Ensure the corrected value is within servo limits
    return constrain(correctedServoPosition, left, right);
    //return correctedServoPosition;
}

//function to set servo angle
void set_steer(int angle) {
    servo.write(angle);
}


// void set_steer(int angle) {
//     // Calculate the weighted average of the current angle and the previous angle
//     float smoothedAngle = WEIGHT * previousServoAngle + (1 - WEIGHT) * angle;
    
//     servo.write((int)smoothedAngle);
//     previousServoAngle = smoothedAngle;  // update the previous angle with the smoothed angle
// }



//function to print sensor data
void print_sensor_data() {
    Serial.print("Angle: ");
    Serial.print(sensorData.angle);
    
    Serial.print(" | Front Distance: ");
    Serial.print(sensorData.distances[FRONT]);
    
    Serial.print(" | Left Distance: ");
    Serial.print(sensorData.distances[LEFT]);
    
    Serial.print(" | Right Distance: ");
    Serial.print(sensorData.distances[RIGHT]);

    // Serial.print(" | Color: ");
    // Serial.println(sensorData.color);
    
}


//function to stop tracking
void stop_steeringTracking() {
  steering_tracking = false;
}

//function to start tracking
void start_steeringTracking() {
  steering_tracking = true;
}

//function to set heading
int set_heading(int heading) {
  
    if (steering_tracking == true) {
  
    float current_angle = sensorData.angle;
    int servoAngle = steringCorrection(current_angle, heading);
    set_steer(servoAngle);
    Serial.print(steering_tracking);
    Serial.print("\tCorrected Servo Value: ");
    Serial.print(servoAngle);
    Serial.print("\tSet point: ");
    Serial.println(heading);
    return servoAngle;
    }    
}

void setup() {

    // Initialize serial communication
    Serial.begin(115200);

    // Initialize BNO055 sensor
    if (!bno.begin()) {
        Serial.print("No BNO055 detected");
        while (1);
    }

    pixy.init();
    pixy.setLamp(1, 0);

    pinMode(6, OUTPUT);
    servo.attach(SERVO_PIN);
    pinMode(MOTOR1_PIN1, OUTPUT);
    pinMode(MOTOR1_PIN2, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    for (int i = FRONT; i <= RIGHT; i++) {
        pinMode(TRIG_PIN[i], OUTPUT);
        pinMode(ECHO_PIN[i], INPUT);
    }
    
    // set_steer(108); // turn straight
    delay(1000);
    set_motor(200); // start moving
    //set TCP34725
    // if (tcs.begin()) {
    //     Serial.println("TCS34725 found");
    // } else {
    //     Serial.println("No TCS34725 found");
    //     while (1);
    // }
}

void loop() {
    pixy.setLamp(1, 0);
    // Update sensor data
    updateSensorData();
    updateBlockData();
    // Print sensor data
    //print_sensor_data();
    start_steeringTracking();
    set_heading(currentSetpoint); // (heading, steering tracking flag)

    int i = 1;
    while (i<=12)
    {   
        updateSensorData();
        updateBlockData();
        set_heading(currentSetpoint);

        if (firstObject.signature == 2 && firstObject.height >=40)
        {
            stop_steeringTracking();
            set_steer(120);
            delay(600);
            //updateSensorData();
            // if(sensorData.distances[left]<10)
            // {
            //     set_steer(100);
            //     delay(500);
            // }
            set_steer(70);
            delay(500);
            set_steer(95);
            start_steeringTracking();
            set_heading(currentSetpoint);
            updateSensorData();
            updateBlockData();
        }

        if (firstObject.signature == 1 && firstObject.height >=40)
        {
            stop_steeringTracking();
            set_steer(75);
            delay(500);
            updateSensorData();
            // if(sensorData.distances[left]<10)
            // {
            //     set_steer(100);
            //     delay(500);
            // }
            set_steer(120);
            delay(500);
            set_steer(95);
            start_steeringTracking();
            set_heading(currentSetpoint);
            updateSensorData();
            updateBlockData();
        }

        if (sensorData.distances[FRONT] < FRONT_OBSTACLE_DISTANCE && sensorData.distances[LEFT] == 101) {
            stop_steeringTracking();
            print_sensor_data();
            set_motor(170);
            set_steer(70);
            while(sensorData.angle >= setpoint[i])
            {   
                // set_motor(200);
                // Serial.println("****Entered While *********");
                update_angle();
                // Serial.println("****Turning Left*********");
                // print_sensor_data();
                // Serial.print("  Setpoint: ");
                // Serial.print(currentSetpoint);/
                Serial.print("Current Angle: ");
                Serial.print(sensorData.angle);
                Serial.print(" | Current Setpoint: ");
                Serial.println(setpoint[i]);

            }
            updateSensorData();
            Serial.println("****Exited While *********");
            currentSetpoint = setpoint[i];
            start_steeringTracking();
            set_heading(currentSetpoint);
            set_motor(200);
            Serial.print("  Setpoint: ");
            Serial.print(currentSetpoint);
            i=i+1;
            delay(500);
          

         }
    
    }
    stop_motor();
    while (1)
    {
        /* code */
    }
    
}
