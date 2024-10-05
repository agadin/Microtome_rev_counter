# Overview
This project uses an HC-SR04 ultrasonic sensor to count the number of times a microtome blade passes by and calculates the speed of the blade. The results are displayed on both the serial monitor and an LCD screen.

* [Introduction](#introduction)
* [Installation](#installation)
* [Usage](#usage)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)

# Introduction
The microtome is a device used to cut thin slices of material, typically for microscopy. The blade moves up and down, cutting the material as it passes by. Out of curosity, and because our arms hurt from using this machine so much, we wanted to know the total distance and other facts about our microtoming efforts. This project uses an HC-SR04 ultrasonic sensor to count the number of times the blade passes by, the speed, and total distance traveled by the blade.

`microtome_speed.ino` is the main file that contains the code for the project. The code is written in C++ and is designed to be run on an D1_mini. The code reads the distance from the ultrasonic sensor and calculates the speed of the blade based on the time between each pass. The results are displayed on both the serial monitor and an LCD screen.

# Installation
To run this project, you will need the following hardware:
* D1_mini
* HC-SR04 ultrasonic sensor
* 16 char LCD screen connected via i2c

You will also need the following libraries:
* [LiquidCrystal_I2C](https://www.arduino.cc/reference/en/libraries/liquidcrystal-i2c/)

To install the libraries, download the zip files from the links above and extract the contents to your Arduino libraries folder. The libraries can be installed by going to `Sketch > Include Library > Add .ZIP Library` in the Arduino IDE.

## Circuit Diagram
**HC-SR04 Ultrasonic Sensor:**  
* VCC to 5V 
* GND to GND 
* Trig to digital pin 9 
* Echo to digital pin 10

**LCD Screen:**  
* Connect the LCD screen to the I2C pins on the D1_mini (SDA and SCL).

# Usage
To use this project, connect the hardware as shown in the circuit diagram section above. Upload the code to the D1_mini by using the Arduino IDE (or another of your choice) and open the serial monitor to view the results. The LCD screen will also display the results.

### Additional Information
The blade must travel from at least 20 cm away from the sensor to 15 cm or closer to be counted as a pass.
The speed is calculated based on the time taken for the blade to travel between these two points.


# Contributing
Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.
* @agadin on github

# License
Distributed under the MIT License. See `LICENSE` for more information.

# Contact
* @agadin on github

# Acknowledgements
* [LiquidCrystal_I2C](https://www.arduino.cc/reference/en/libraries/liquidcrystal-i2c/)
* [HC-SR04 Ultrasonic Sensor](https://www.arduino.cc/en/Tutorial/HelloWorld)
* [D1_mini](https://www.arduino.cc/en/Guide/ArduinoPro)






