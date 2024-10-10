#include <LCD-I2C.h>
#include <Wire.h>

// Pin definitions
const int trigPin = 9; // Trigger pin for the HC-SR04 sensor
const int echoPin = 10; // Echo pin for the HC-SR04 sensor

// Variables for distance measurement
long duration; // Duration of the pulse
int distance; // Calculated distance

// Variables for counting passes
int passCount = 0; // Number of blade passes
boolean bladeClose = false; // Flag to check if the blade is close to the sensor
unsigned long startTime; // Start time of the blade pass
unsigned long endTime; // End time of the blade pass
float speed; // Speed of the blade
float totalDistance = 0; // Total distance traveled by the blade

// Moving average variables
const int bufferSize = 10; // Size of the buffer for moving average
float speedBuffer[bufferSize]; // Buffer to store recent speed measurements
int bufferIndex = 0; // Current index in the buffer
float movingAverageSpeed = 0; // Moving average of the speed

// LCD setup
LCD_I2C lcd(0x27, 16, 2); // Initialize the LCD with I2C address 0x27 and size 16x2

// Motivational messages
const char* messages[] = {
  "Keep going!",
  "You're doing great!",
  "Almost there!",
  "Stay strong!",
  "You can do it!"
};
int messageInterval = 10; // Interval for displaying messages (in cycles)
const int messageDuration = 20000; // Duration for displaying messages (in milliseconds)
unsigned long lastMessageTime = 0; // Last time a message was displayed

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the LCD
  Wire.begin();
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Microtome Counter");

  // Initialize the sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize the speed buffer
  for (int i = 0; i < bufferSize; i++) {
    speedBuffer[i] = 0;
  }
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

  // Check for blade pass
  if (distance <= 15 && !bladeClose) { // If the blade is close and was not previously close
    bladeClose = true; // Set the bladeClose flag to true
    startTime = millis(); // Record the start time
  } else if (distance >= 20 && bladeClose) { // If the blade is far and was previously close
    bladeClose = false; // Set the bladeClose flag to false
    endTime = millis(); // Record the end time
    passCount++; // Increment the pass count
    speed = (20.0 - 15.0) / ((endTime - startTime) / 1000.0); // Calculate the speed in cm/s
    totalDistance += 5.0; // Increment the total distance by 5 cm (distance between 20 cm and 15 cm)

    // Update the speed buffer
    speedBuffer[bufferIndex] = speed;
    bufferIndex = (bufferIndex + 1) % bufferSize;

    // Calculate the moving average speed
    float sum = 0;
    for (int i = 0; i < bufferSize; i++) {
      sum += speedBuffer[i];
    }
    movingAverageSpeed = sum / bufferSize;

    // Display on serial monitor
    Serial.print("Pass Count: ");
    Serial.println(passCount);
    Serial.print("Speed: ");
    Serial.print(speed);
    Serial.println(" cm/s");
    Serial.print("Moving Avg Speed: ");
    Serial.print(movingAverageSpeed);
    Serial.println(" cm/s");
    Serial.print("Total Distance: ");
    Serial.print(totalDistance);
    Serial.println(" cm");

    // Display on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Passes: " + String(passCount));

    // Check if it's time to display a motivational message
    if (passCount % messageInterval == 0 && millis() - lastMessageTime > messageDuration) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(messages[random(0, 5)]);
      lastMessageTime = millis();
      delay(messageDuration);
      lcd.clear();
      messageInterval = random(10, 51); // Set a new random interval between 10 and 50
    }
  }

  // Cycle between showing total distance and velocity
  if (millis() % 2000 < 1000) {
    lcd.setCursor(0, 1);
    lcd.print("Speed: " + String(speed) + " cm/s");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Distance: " + String(totalDistance) + " cm");
  }

  delay(100); // Wait for 100 milliseconds before the next loop
}