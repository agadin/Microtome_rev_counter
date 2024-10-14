#include <LCD-I2C.h>
#include <Wire.h>

// Pin definitions
const int trigPin = 4; // Trigger pin for the HC-SR04 sensor
const int echoPin = 2; // Echo pin for the HC-SR04 sensor

// Variables for distance measurement
long duration; // Duration of the pulse
int distance; // Calculated distance

// LCD setup
LCD_I2C lcd(0x27, 16, 2); // Initialize the LCD with I2C address 0x27 and size 16x2

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the LCD
  Wire.begin();
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");

  // Initialize the sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Measure distance
  digitalWrite(trigPin, LOW); // Set the trigger pin low
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH); // Set the trigger pin high
  delayMicroseconds(10); // Wait for 10 microseconds
  digitalWrite(trigPin, LOW); // Set the trigger pin low
  duration = pulseIn(echoPin, HIGH); // Read the echo pin and get the duration of the pulse
  distance = duration * 0.034 / 2; // Calculate the distance in cm

  // Display the distance on the LCD
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear the previous distance
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  delay(100); // Wait for 100 milliseconds before the next loop
}