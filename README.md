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
The microtome is a device used to cut thin slices of material, typically for microscopy. The blade moves up and down, cutting the material as it passes by. Out of curosity, and because our arms hurt from using this machine so much, we wanted to know the total distance and other facts about our microtoming efforts. This project uses an HC-SR04 ultrasonic sensor to count the number of times the blade passes by, the speed, and total distance traveled by the blade with some motivational quotes along the way.

`microtome_speed.ino` is the main file that contains the code for the project. The code is written in C++ and is designed to be run on an ESP32 or arduino capable device. The code reads the distance from the ultrasonic sensor and calculates the speed of the blade based on the time between each pass. The results are displayed on both the serial monitor and an LCD screen.

# Installation
**To run this project, you will need the following hardware:**
* ESP32 Wroom
* HC-SR04 ultrasonic sensor
* 16 char LCD screen connected via i2c (using a HD44780 for example)

**You will also need the following libraries:**
* [LCD-I2C](https://github.com/hasenradball/LCD-I2C)

## Circuit Diagram
![Circuit Diagram](
**HC-SR04 Ultrasonic Sensor:**  
* VCC to 5V 
* GND to GND 
* Trig to digital pin 9 
* Echo to digital pin 10

**LCD Screen:**  
* Connect the LCD screen to the I2C pins on the D1_mini (SDA and SCL).
* VCC to 5V
* GND to GND


**To install the code onto your ESP32, follow these steps:**
1. Clone the repository
2. Open the `microtome_counter.ino` file in the Arduino IDE
3. Install the required libraries (LCD-I2c) using the library tab on the right vertical ribbon
4. Install the ESP32 board in the Arduino IDE by following the instructions [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
5. Plug in your ESP32 and select the correct port and board from the tools menu (see link above for more details).
6. Upload the code to the ESP32 by clicking the upload button in the Arduino IDE
7. Open the serial monitor to view the results and ensure the LCD screen is displaying the results as well.




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






