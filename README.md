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

## Insight

_Our robot is an innovative robot engineered for the WRO Future Engineers competition. This documentation offers an in-depth examination of its architecture, detailing the rationale behind its design choices, and presents the code responsible for its precise maneuvering capabilities.



### Hardware Overview:

#### Frame Design:
Our robot incorporates a distinctive design composed of a large spike frame and other lego components, enhancing its aesthetic appeal and structural integrity. The fusion of LEGO parts and 3D printed models further reinforces the robots's stability.

#### Drivetrain:
A crucial element of the robot's performance lies in its drivetrain. It employs two motors - one for driving and another for steering. The driving motor, an N20 servo motor, provides 600 RPM, balancing speed and torque effectively. The selection of this motor ensures optimal performance under various conditions.

#### Steering Mechanism:
For steering, a high-torque servo motor has been integrated, allowing for precise control over directional changes. This choice of motor aligns with the competition's requirements and contributes to the robot's agility.

#### Differential Drive System:
The drivetrain utilizes a LEGO differential mechanism, distributing power evenly to the driving wheels. This differential system plays a pivotal role in maintaining stability during maneuvers.

#### Center of Gravity:
The robot's center of gravity is a critical factor influencing its stability and maneuverability. With the Arduino and battery positioned atop the driving wheels at the rear, and the motors situated in the center, the robot achieves a balanced distribution of weight. This configuration minimizes the risk of tipping during rapid movements or sharp turns. Additionally, the placement of the camera at the front ensures that its weight contributes to the overall equilibrium. This strategic arrangement optimizes the robot's center of gravity, allowing it to navigate with precision and confidence, even in dynamic and challenging environments.



### Sensors and Perception:

#### Ultrasonic Sensors:
The robot employs three ultrasonic sensors strategically positioned for accurate distance measurement. The front sensor aids in precise turns and obstacle avoidance, while the left and right sensors detect inner and outer walls, enhancing accuracy in confined spaces.

#### Pixy Camera:
A Pixy camera is integrated to identify obstacle colors. It distinguishes between green (left) and red (right), facilitating intelligent decision-making during navigation.



### Code Overview:
_The robot's functionality is driven by a carefully crafted code that orchestrates its movements. Below are key components of the code:

#### Library Inclusions:
The code includes essential libraries such as Wire, Adafruit_Sensor, and Adafruit_BNO055 for interfacing with sensors and motor control.

#### Pin Configuration:
The code establishes pin modes for ultrasonic sensors, motors, and the servo, ensuring proper communication between the components.

#### Initialization:
The setup function initializes the robot, configuring pins, setting up the serial communication, and initializing the BNO055 sensor.

#### Distance Measurement:
The 'dist' function uses the ultrasonic sensors to calculate distances from obstacles. It employs the pulseIn function to obtain accurate readings.

#### Orientation Data:
The 'printEvent' function extracts orientation data from the BNO055 sensor, providing critical information about the robot's spatial orientation.

#### Steering and Driving Control:
The 'loop' function is the heart of the code. It manages the robot's movements, including steering, driving, and obstacle avoidance. The motors are controlled using digitalWrite and analogWrite functions, allowing for precise speed and direction control.

#### Obstacle Handling:
The code implements intelligent obstacle handling logic based on sensor readings. When an obstacle is detected, the robot adjusts its orientation and steering to navigate around it.



### Conclusion:
_Our Robot is a testament to thoughtful engineering and strategic component selection. Its unique frame design, coupled with a differential drive system, provides a solid foundation for agile movements. The integration of ultrasonic sensors and a Pixy camera elevates its perception capabilities, making it a formidable competitor in the WRO Future Engineers competition.
