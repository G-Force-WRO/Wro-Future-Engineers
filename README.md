Engineering materials
====

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Future Engineers competition in the season 2023.

## Content

* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members)
* `v-photos` contains 6 photos of the vehicle (from every side, from top and bottom)
* `video` contains the video.md file with the link to a video where driving demonstration exists
* `schemes` contains one or several schematic diagrams in form of JPEG, PNG or PDF of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition
* `models` is for the files for models used by 3D printers, laser cutting machines and CNC machines to produce the vehicle elements. If there is nothing to add to this location, the directory can be removed.
* `other` is for other files which can be used to understand how to prepare the vehicle for the competition. It may include documentation how to connect to a SBC/SBM and upload files there, datasets, hardware specifications, communication protocols descriptions etc. If there is nothing to add to this location, the directory can be removed.

## Introduction

Our robot, a robust Arduino Uno-based robot, has been designed to excel in the WRO Future Engineers competition. This documentation provides a comprehensive overview of its components, design rationale, and the code that drives its performance. The robot employs advanced features such as ultrasonic sensors and a Pixy camera for accurate obstacle detection and color recognition. This document serves as a guide for those interested in understanding and replicating the our robot's functionality.

## About Us
G-FORCE is dynamic trio, each member contributing vital skills to the project. Keyaan, the proficient coder, brings technical expertise and ensures the robot's software operates seamlessly. Arham, the skilled constructor, plays a pivotal role in materializing the design, ensuring structural integrity and functionality. Suramrit, the dedicated documenter and co-constructor, bridges the gap between concept and reality by meticulously recording progress and aiding in construction. Despite starting late, our team showcases admirable dedication and a diverse skill set, promising a project that combines precision coding, solid construction, and thorough documentation for a competitive edge in the WRO Future Engineers competition.

## Insight

*Our robot is an innovative robot engineered for the WRO Future Engineers competition. This documentation offers an in-depth examination of its architecture, detailing the rationale behind its design choices, and presents the code responsible for its precise maneuvering capabilities.*



### Hardware Overview:

#### Frame Design:
Our robot incorporates a distinctive design composed of a large spike frame and other Lego components, enhancing its aesthetic appeal and structural integrity. The fusion of LEGO parts and 3D-printed models further reinforces the robots's stability.

#### Drivetrain:
A crucial element of the robot's performance lies in its drivetrain. It employs two motors, one for driving and another for steering. The driving motor, an n20 servo motor, provides 600 RPM, balancing speed and torque effectively. The selection of this motor ensures optimal performance under various conditions.

#### Steering Mechanism:
For steering, a high-torque MG90 servo motor has been integrated, allowing for precise control over directional changes. This choice of motor aligns with the competition's requirements and contributes to the robot's agility.

#### Differential Drive System:
The drivetrain utilizes a LEGO differential mechanism, distributing power evenly to the driving wheels. This differential system plays a pivotal role in maintaining stability during maneuvers.

#### Center of Gravity:
The robot's center of gravity is a critical factor influencing its stability and maneuverability. With the Arduino and battery positioned atop the driving wheels at the rear and the motors situated in the center, the robot achieves a balanced distribution of weight. This configuration minimizes the risk of tipping during rapid movements or sharp turns. Additionally, the placement of the camera at the front ensures that its weight contributes to the overall equilibrium. This strategic arrangement optimizes the robot's center of gravity, allowing it to navigate with precision and confidence, even in dynamic and challenging environments.



### Sensors and Perception:

#### Ultrasonic Sensors:
The robot employs three ultrasonic sensors strategically positioned for accurate distance measurement. The front sensor aids in precise turns and obstacle avoidance, while the left and right sensors detect inner and outer walls, enhancing accuracy in confined spaces. // how this helps our strategy. what our plan is. make it cohesive

#### Pixy 2 Camera:
A Pixy 2 camera is a plug-and-play smart vision system integrated to identify obstacle colors. It distinguishes between green (left) and red (right), facilitating intelligent decision-making during navigation. // communicates over spi. 


### Code Overview:
*The robot's functionality is driven by a carefully crafted code that orchestrates its movements. Our algorithm focuses on moving the bot in a straight line using an IMU (inertial measurement unit) to correct our position in the straight segments and turn when distance from an ultrasonic sensor is less than a certain threshold.*

#### Library Inclusions:
The code includes essential libraries such as Wire, Adafruit_Sensor, and Adafruit_BNO055 for interfacing with sensors and motor control. These libraries are used to extract orientation data from the IMU, which is essential for the robot to move accurately regardless of small errors like play in the servos, an imbalanced center of mass, or even friction. We also used libraries like Servo to communicate with our MG90 to write positions to it and be able to make turns based on calculations in the code.

#### Pin Configuration:
The code establishes pin modes for ultrasonic sensors, motors, and the servo, ensuring proper communication between the components. We have used all the pins in our Arduino except two: Pins 9 and 10. So we have maximized our use of the Arduino Uno. To decide which pins go where, we tried to judge what the minimum distance of components would be to reduce the wiring and soldering needed. Also, some components, like our Pixy, have a cable that directly connects to pins 11, 12, and 13, so special changes were needed for that. A more detailed circuit diagram can be found at: <ins>Wro-Future-Engineers/schemes/</ins>. Before set up, we also created an IMU object called `bno`.

![image](https://github.com/G-Force-WRO/Wro-Future-Engineers/assets/145182523/819b371f-9a15-488b-b71c-00a921376eee)

#### Initialization:
The setup function initializes the robot, configuring pins, setting up serial communication, and initializing the BNO055 sensor. We do this by setting each pin as either an `OUTPUT` pin or an `INPUT` pin, depending on whether it requires data to be written to it or to be received from it. For example, our `ECHO_PIN` for our ultrasonic sensor, which returns the distance needed to be an `INPUT` and `SERVO_PIN` which tells the MG90 Servo what position to go to, should be an `OUTPUT`. Then we had to begin the serial communication by stating the baud rate. The baud rate represents the rate at which data is sent in bits per second. The Arduino default is a baud rate of 9600 bits per second, but we increased it to 115200 to get faster readings and to see more outliers using a serial monitor to assist us with debugging, but simultaneously, it was not so high that it overwhelmed us with data to the point where we couldn't read the data. Finally, we set the angle of the servo to straight (115 degrees) and initialized the IMU (using `bno.begin`).

#### Distance Measurement:
The 'dist' function uses the ultrasonic sensors to calculate distances from obstacles. It employs the pulseIn function to obtain accurate readings (and restricts distance readings over 200 for a smooth operation without the ultrasonic waiting for too long to receive the impulse). It works by sending out an ultrasonic signal in a 30-40 degree range and measures time taken for the signal to return. Then using the basic formula: `speed = distance/time` we can calculate distance considering that speed of sound in air is ~340 metres per second and with information on the time taken. //add application of us in code

#### Orientation Data:
The 'printEvent' function extracts orientation data from the BNO055 sensor, providing critical information about the robot's spatial orientation, and sets it in a variable so we can autonomously correct any play by our servo.

#### Steering and Driving Control:
The 'loop' function is the heart of the code. It manages the robot's movements, including steering, driving, and obstacle avoidance. The motors are controlled using digitalWrite and analogWrite functions, allowing for precise speed and direction control. We get constant data from an IMU to correct our position. Finally, we have an ultrasonic sensor in the front to detect walls and alert us about when to turn.

#### Obstacle Handling:
The code implements intelligent obstacle handling logic based on sensor readings. When an obstacle is detected, the robot adjusts its orientation and steering to navigate around it.



### Conclusion:
*Our Robot is a testament to thoughtful engineering and strategic component selection. Its unique frame design, coupled with a differential drive system, provides a solid foundation for agile movements. The integration of ultrasonic sensors and a Pixy camera elevates its perception capabilities, making it a formidable competitor in the WRO Future Engineers competition.*
