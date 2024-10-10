#include <LCD-I2C.h>
#include <Wire.h>

// Pin definitions
const int trigPin = 4; // Trigger pin for the HC-SR04 sensor
const int echoPin = 2; // Echo pin for the HC-SR04 sensor

// Variables for distance measurement
long duration; // Duration of the pulse
int distance; // Calculated distance
int minDistance = 1000; // Initialize with a high value for finding local minimum
int maxDistance = 0; // Initialize with 0 for finding local maximum

// Variables for counting passes
int passCount = 0; // Number of blade passes
boolean bladeClose = false; // Flag to check if the blade is close to the sensor
boolean bladeFar = false; // Flag to check if the blade moved far
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
const int messageDuration = 5000; // Display message for 5 seconds
unsigned long lastMessageTime = 0; // Last time a message was displayed
boolean showingMessage = false; // Flag to track if message is currently being shown

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

  // Track local minimum (closest) and maximum (farthest) distance in a cycle
  if (distance < minDistance) {
    minDistance = distance; // Update minimum if current distance is closer
  }
  if (distance > maxDistance) {
    maxDistance = distance; // Update maximum if current distance is farther
  }

  // Check if the blade is close (within 15 cm) and detect when it moves away (above 20 cm)
  if (distance <= 15 && !bladeClose) { // Blade is close, start tracking
    bladeClose = true;
    minDistance = distance; // Reset min distance for this cycle
    startTime = millis(); // Record start time
  } else if (distance >= 20 && bladeClose) { // Blade moves far, end tracking
    bladeClose = false;
    bladeFar = true;
    maxDistance = distance; // Record the maximum distance the blade moved away
    endTime = millis(); // Record end time

    // Calculate speed (maxDistance - minDistance) / time
    float travelDistance = maxDistance - minDistance;
    float timeElapsed = (endTime - startTime) / 1000.0; // Convert ms to seconds
    speed = travelDistance / timeElapsed;

    totalDistance += travelDistance; // Add this cycle's travel distance to total

    // Update the speed buffer for moving average
    speedBuffer[bufferIndex] = speed;
    bufferIndex = (bufferIndex + 1) % bufferSize;

    // Calculate the moving average speed
    float sum = 0;
    for (int i = 0; i < bufferSize; i++) {
      sum += speedBuffer[i];
    }
    movingAverageSpeed = sum / bufferSize;

    passCount++; // Increment pass count

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

    // Check if it's time to display a motivational message
    if (passCount % messageInterval == 0 && millis() - lastMessageTime > messageDuration) {
      showingMessage = true; // Set flag to show message
      lastMessageTime = millis(); // Reset message display time
      lcd.setCursor(0, 1); // Set to the second line of the LCD
      lcd.print(messages[random(0, 5)]);
    }
  }

  // Always show the pass count on the top line
  lcd.setCursor(0, 0);
  lcd.print("Passes: " + String(passCount));

  // Handle the bottom line display
  if (showingMessage) {
    // Keep showing the message for 5 seconds
    if (millis() - lastMessageTime >= messageDuration) {
      showingMessage = false; // Stop showing the message after 5 seconds
    }
  } else {
    // Cycle between showing total distance and speed when not showing a message
    if (millis() % 2000 < 1000) {
      lcd.setCursor(0, 1);
      lcd.print("Speed: " + String(speed) + " cm/s");
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Distance: " + String(totalDistance) + " cm");
    }
  }

  delay(100); // Wait for 100 milliseconds before the next loop
}
