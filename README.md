# Overview
![LCD Screen](https://raw.githubusercontent.com/agadin/Microtome_rev_counter/refs/heads/main/img/microtome_counter_screen.gif)

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

## Features
* **Distance Measurement:** Uses an HC-SR04 ultrasonic sensor to measure the distance of the microtome blade. 
* **Speed Calculation:** Calculates the speed of the blade based on the time between passes. 
* **Pass Counting:** Counts the number of times the blade passes by the sensor. 
* **Total Distance Tracking:** Tracks the total distance traveled by the blade.

* **CD Display:** Displays the pass count, speed, and total distance on an LCD screen. 
* **Motivational Messages:** Displays random motivational messages after a certain number of passes. 
* **Reset Button:** Allows users to reset the pass count and total distance. 
* **Celebration Mode:** A fun mode that displays a congratulatory message when a button is pressed.


# Installation
**To run this project, you will need the following hardware:**
* ESP32 Wroom
* HC-SR04 ultrasonic sensor
* 16 char LCD screen connected via i2c (using a HD44780 for example)

**You will also need the following libraries:**
* [LCD-I2C](https://github.com/hasenradball/LCD-I2C)

## Circuit Diagram
![Circuit Diagram](https://raw.githubusercontent.com/agadin/Microtome_rev_counter/refs/heads/main/img/circuit_diagram.png)

**HC-SR04 Ultrasonic Sensor:**  
* VCC to 5V 
* GND to GND 
* Trig to digital pin D4 
* Echo to digital pin D2

**LCD Screen:**  
* Connect the LCD screen to the I2C pins on the D1_mini (SDA and SCL).
* VCC to 5V
* GND to GND

**Buttons:**
* Button 1: D15
* Button 2: D6

Note: The buttons are optional and can be used to reset the pass count and total distance or to enter celebration mode. These are connected to a digital in and ground (GND) on the ESP32.

**To install the code onto your ESP32, follow these steps:**
1. Clone the repository
2. Open the `microtome_counter.ino` file in the Arduino IDE
3. Install the required libraries (LCD-I2c) using the library tab on the right vertical ribbon
4. Install the ESP32 board in the Arduino IDE by following the instructions [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
5. Plug in your ESP32 and select the correct port and board from the tools menu (see link above for more details).
6. Upload the code to the ESP32 by clicking the upload button in the Arduino IDE
7. Open the serial monitor to view the results and ensure the LCD screen is displaying the results as well.

# Usage
To use this project, connect the hardware as shown in the circuit diagram section above. Upload the code to the ESP32 Wroom by using the Arduino IDE (or another of your choice) and open the serial monitor to view the results. The LCD screen will also display the results.

**NOTE:** The expressions `(distance <= 11 && !bladeClose)` and `(distance >= 16 && bladeClose)` are used to detect the blade passing by the sensor. These values are threshold in which the blade has to be below or exceed to be qualified as a pass by the counter. Change these values to suit your needs.

### Buttons
* **Button 1:** Resets the pass count and total distance.
* **Button 2:** Enters celebration mode and displays a congratulatory message.

### Optional CAD files
* The `microtome_counter.step` file contains a 3D model of the microtome counter. This model can be 3D printed and used to mount the LCD and other components to the microtome.
![3D Model](https://raw.githubusercontent.com/agadin/Microtome_rev_counter/refs/heads/main/img/holder_counter1.gif)

### Additional Information
* The ultrasonic sensor is used to detect the distance between the sensor and the blade. The sensor is placed at the top of the blade's path and measures the distance to the blade as it passes by accessing the periodic nature of the back and forth motion of teh microtome. The speed is calculated based on the time taken for the blade to travel between these two points.


### Changing motivational quotes
The motivational quotes are stored in the `messages` array in the `microtome_counter.ino` file. You can change the quotes by modifying the strings in this array. These messages are displayed after achieving a random amount of passes by the blade.  

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






