*Our robot is an innovative robot engineered for the WRO Future Engineers competition. This documentation offers an in-depth examination of its architecture, details the rationale behind its design choices, and presents the code responsible for its precise maneuvering capabilities.*

### Hardware Overview:

#### Frame Design:
The core of our robot's identity lies in its striking and unique design. It boasts a prominent frame, a defining feature that not only adds a distinctive visual flair but also contributes significantly to its structural integrity. Complementing this primary element are carefully integrated Lego components, forming a harmonious fusion of form and function. These Lego elements serve as integral building blocks, adding both aesthetic and structural value to the robot. In addition to the Lego components, we've leveraged 3D-printed models, strategically incorporated to provide additional support and strength, ultimately enhancing the overall stability and robustness of our robot's framework. This meticulous integration of Lego parts and 3D-printed models is a testament to our team's commitment to creating a well-balanced and visually impressive robotic platform.

#### Drivetrain:
The drivetrain constitutes a pivotal component that significantly influences the robot's overall performance. It operates on a system of two distinct motors, each assigned a specialized role. One of these motors is dedicated to propelling the robot forward, while the other is responsible for steering its direction. The driving motor, specifically an N20 servo motor, is engineered to deliver an impressive 600 revolutions per minute (RPM). This particular choice of motor is a careful equilibrium between speed and torque, both of which are critical aspects in determining the robot's efficiency. By opting for the N20 servo motor, the team ensures that the robot can operate optimally across a wide range of conditions and challenges it may encounter during the competition. This choice underscores the team's commitment to achieving top-notch performance and adaptability.

#### Steering Mechanism:
The steering mechanism of the robot is facilitated by the incorporation of a high-torque MG90 servo motor. This component plays a pivotal role in determining the direction of the robot's movement. Unlike the driving motor, which focuses on propelling the robot forward, the servo motor's primary function is to control the orientation of the wheels, enabling the robot to make precise turns and navigate intricate paths.

The selection of a high-torque motor is a strategic decision, as it ensures that the motor possesses the necessary strength to exert control over the steering mechanism. This attribute is particularly crucial in scenarios where the robot encounters obstacles or requires swift directional adjustments.

By integrating the MG90 servo motor, the robot aligns with the specific requirements set forth by the competition organizers. This meticulous attention to detail underscores the team's dedication to meeting and exceeding the competition's standards.

Furthermore, the high-torque nature of the motor translates into a heightened level of responsiveness in steering operations. This directly contributes to the robot's agility, allowing it to maneuver deftly and with precision, a key factor in excelling in the competition's challenges. The integration of the MG90 servo motor is thus a crucial element in ensuring the SpikeBot's ability to perform intricate maneuvers and maintain a competitive edge.


#### Differential Drive System:
The drivetrain of the robot incorporates a crucial component known as a LEGO differential mechanism. This specialized component serves a vital function by ensuring that power from the motors is evenly distributed to the driving wheels. This even distribution of power is instrumental in maintaining stability, especially during intricate manoeuvres. It prevents one wheel from receiving an excess of power compared to the other, which could lead to erratic movements or loss of control. By utilizing this differential system, the SpikeBot is able to navigate with a high degree of precision and stability, contributing significantly to its overall performance and effectiveness in the competition.
![image](https://github.com/G-Force-WRO/Wro-Future-Engineers/assets/145182523/408ad570-71b2-4058-ac00-8f3ea07179a5)


#### Center of Gravity:
The center of gravity, a pivotal consideration in the design of the robot, significantly impacts its stability and maneuvering capabilities. The placement of key components is strategically arranged to achieve optimal balance. Specifically, the Arduino and battery are situated atop the driving wheels at the rear, while the motors are positioned at the center. This configuration effectively distributes the weight, minimizing the likelihood of tipping, particularly during swift movements or sharp directional changes. Moreover, the camera, a prominent component, is intentionally positioned at the front. This placement ensures that its weight contributes harmoniously to the overall equilibrium of the robot. The collective result of this thoughtfully planned arrangement is an optimized center of gravity, empowering the robot to navigate with precise control and unwavering confidence, even in dynamic and demanding environments.


### Sensors and Perception:

#### Ultrasonic Sensors:
The incorporation of three strategically positioned ultrasonic sensors is a pivotal aspect of our robot's design, playing a crucial role in executing our competition strategy with precision and efficiency. The foremost sensor, strategically placed at the front, serves a dual purpose. Firstly, it significantly contributes to the robot's ability to execute precise turns, a critical skill in navigating the competition's obstacle course. Secondly, it aids in real-time obstacle avoidance, allowing the robot to make split-second adjustments to its trajectory, ensuring it safely navigates around potential obstructions.

In tandem with the front sensor, the left and right sensors assume distinct responsibilities. Their placement is calibrated to detect inner and outer walls, an invaluable feature when navigating confined spaces. This capability equips the robot with a heightened sense of spatial awareness, enabling it to navigate tight corners and narrow passageways with finesse. By intelligently leveraging this sensor arrangement, our plan is to equip the SpikeBot with an enhanced level of accuracy and adaptability, setting it apart as a formidable contender in the WRO Future Engineers competition.

#### Pixy 2 Camera:
The Pixy 2 camera is a sophisticated smart vision system that seamlessly integrates into our robot. Its plug-and-play nature means it can be easily incorporated into our existing setup without the need for extensive configuration. This powerful component plays a crucial role in our robot's ability to perceive and interact with its environment.

One of the key functionalities of the Pixy 2 camera is its capacity to discern between different obstacle colors. Specifically, it is adept at distinguishing between green, which corresponds to the left side, and red, indicative of the right side. This capability is invaluable as it provides our robot with a clear and reliable method for identifying and classifying obstacles in its path.

By recognizing and categorizing obstacles based on their color, the Pixy 2 camera significantly enhances our robot's decision-making process during navigation. This enables the robot to make informed choices about its path, allowing it to efficiently maneuver around obstacles and reach its destination.

Furthermore, the Pixy 2 camera employs SPI (Serial Peripheral Interface) for communication. This high-speed, synchronous serial communication protocol ensures efficient and reliable data exchange between the camera and the other components of our robot. This means that the Pixy 2 camera can seamlessly transmit information to our robot's processing unit, enabling timely and accurate responses to its environment.

In essence, the Pixy 2 camera serves as a crucial sensory tool for our robot, providing it with the ability to visually identify and differentiate obstacles based on their colors. This capability significantly enhances our robot's decision-making process, allowing it to navigate complex environments with precision and efficiency. Moreover, its seamless integration through SPI communication ensures smooth operation within our existing system. The Pixy 2 camera stands as a testament to the advanced technology employed in our robot, contributing significantly to its performance in the WRO Future Engineers competition.


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
The 'dist' function uses the ultrasonic sensors to calculate distances from obstacles. It employs the pulseIn function to obtain accurate readings (and restricts distance readings over 200 for a smooth operation without the ultrasonic waiting for too long to receive the impulse). It works by sending out an ultrasonic signal in a 30–40 degree range and measuring the time taken for the signal to return. Then, using the basic formula: `speed = distance/time` we can calculate distance considering that the speed of sound in air is ~340 meters per second and with information on the time taken. We have used an ultrasonic sensor in the front, left and write sides to track the walls around us and accordingly turn and move. The front sensor is used in both rounds to determine when to turn and the right and left sensors are used to get close to walls in the obstacle round so that we dont crash into the side walls while avoiding obstacles. The specific ultrasonic sensors we have used are `HC-SR04` ultrasonic sensors.

#### Orientation Data:
The 'printEvent' function extracts orientation data from the BNO055 sensor, providing critical information about the robot's spatial orientation, and sets it in a variable so we can autonomously correct any play by our servo. Our IMU has a gyroscope which shows angle relative to the initial position, so we can calculate deflection compared to the axis we are at, and add or subtract 90 degrees to this central axis. Based on this deflection, we correct the servo to move straight. While doing more work, we realised that angle does not consistently increase by 90 in turns, so we ran our bot and programmed set points to calibrate the sensor.

#### Steering and Driving Control:
The 'loop' function is the heart of the code. It manages the robot's movements, including steering, driving, and obstacle avoidance. The motors are controlled using digitalWrite and analogWrite functions, allowing for precise speed and direction control. We get constant data from an IMU to correct our position. Finally, we have an ultrasonic sensor in the front to detect walls and alert us about when to turn. In the open rouns, we simply loop the above two steps to move in a circle which is fairly consistent. 

#### Obstacle Handling:
The implemented code incorporates an advanced obstacle-handling mechanism that operates in response to data received from the sensors. When the robot's sensors detect the presence of an obstacle, the code initiates a sequence of actions aimed at ensuring safe navigation. For the second challenge, we are moving in a straight line, and according to the pillar closest to us (determined by the size of the block read by our pixy camera). If the pillar is green, we move close to the left wall (by writing a code to decrease distance read by the left ultrasonic sensor) and vice verca for the red pillars. We have trained the pixy camera to detect colours by declaring unique signatures for each colour.

### Conclusion:
*Our Robot is a testament to thoughtful engineering and strategic component selection. Its unique frame design, coupled with a differential drive system, provides a solid foundation for agile movements. The integration of ultrasonic sensors and a Pixy camera elevates its perception capabilities, making it a formidable competitor in the WRO Future Engineers competition.*
