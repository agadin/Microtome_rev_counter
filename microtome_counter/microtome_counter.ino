#include <LCD-I2C.h>
#include <Wire.h>

// Pin definitions
const int trigPin = 4; // Trigger pin for the HC-SR04 sensor
const int echoPin = 2; // Echo pin for the HC-SR04 sensor
const int resetButtonPin = 15; // Pin for the reset button

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
const int bufferSize = 5; // Size of the buffer for moving average
float speedBuffer[bufferSize]; // Buffer to store recent speed measurements
int bufferIndex = 0; // Current index in the buffer
float movingAverageSpeed = 0; // Moving average of the speed

// LCD setup
LCD_I2C lcd(0x27, 16, 2); // Initialize the LCD with I2C address 0x27 and size 16x2

// Motivational messages- have to be 16 characters long
const char* messages[] = {
  "Keep going!     ",
  "Great job!      ",
  "Almost there!   ",
  "Stay strong!    ",
  "You can do it!  ",
  "Don't give up!  ",
  "Keep pushing!   ",
  "You're amazing! ",
  "You got this!   ",
  "Keep it up!     ",
  "Stay focused!   ",
  "You're a star!  ",
  "You're a champ! ",
  "You're a hero!  ",
  "You're a legend!",
  "You're a winner!",
  "Keep hustling!  ",
  "Onwards!        ",
  "Believe it!     ", // Naruto reference
  "To infinity!    ", // Toy Story reference
  "Stay frosty!    ", // Call of Duty reference
  "The Force is w/ ", // Star Wars reference
  "Smash that goal! ",
  "Wakanda forever!", // Black Panther reference
  "Go Super Saiyan!", // Dragon Ball Z reference
  "Never tell odds ", // Star Wars Han Solo ref
  "Make it so!     ", // Star Trek ref
  "Power up!       ", // General video game ref
  "Suit up!        ", // Barney from HIMYM
  "Eyes on prize!  ",
  "Just one more!  ",
  "You're the GOAT! ",
  "Boom! Headshot! ", // FPS gaming ref
  "Channel Hulk!   ", // Avengers ref
  "You're a wizard!", // Harry Potter ref
  "You're a ninja! ", // Naruto ref
  "You're a pirate!", // One Piece
  "Seize the day!  ",
  "Be unstoppable! ",
  "Rise and shine! ",
  "Level up!       ",
  "Game on!        ",
  "Rock on!        ",
  "Feel the power! ",
  "One step closer!",
  "Let's roll!     ",
  "Just keep swim! ", // Finding Nemo ref
  "Keep moving!    ",
  "You're unstoppable!",
  "Be the change!  ",
  "Keep the faith! ",
  "Believe in you! ",
  "Push through!   ",
  "Go beyond!      ", // My Hero Academia ref
  "You are Iron Man", // Iron Man ref
  "Hero in making! ",
  "Chase your fire!",
  "Never back down!",
  "Run with wolves!",
  "Stay in control!",
  "Be your best!   ",
  "You're in beast!",
  "Claim your crown",
  "Grit and grind! ",
  "The way's open! ",
  "Do or do not!   ", // Star Wars Yoda ref
  "Hold the line!  ",
  "Full throttle!  ",
  "All in!         ",
  "Forward march!  ",
  "Eye of tiger!   ", // Rocky ref
  "Maximum effort! ", // Deadpool ref
  "Born to rise!   ",
  "No limits!      ",
  "Dig deep!       ",
  "Up, up, away!   ", // Superman ref
  "To the moon!    ",
  "Epic win ahead! ",
  "Keep the hustle!",
  "Untouchable!    ",
  "Power through!  ",
  "Bright future!  ",
  "Make it happen! ",
  "Catch the wave! ",
  "You're shining! ",
  "Victory is near!",
  "Let's get loud! ",
  "Charge forward! ",
  "Rise, Jedi!     ", // Star Wars ref
  "I am Groot!     ", // Guardians of Galaxy
  "Lock and load!  ",
  "Wow, so hard!   ",
  "Keep slacking!  ",
  "Nailed it!      ",
  "That was easy!  ",
  "Try harder...   ",
  "Yup, nailed it! ",
  "Oh, impressive. ",
  "Such effort!    ",
  "Really though?  ",
  "Keep 'winning'! ",
  "Well, almost... ",
  "Bravo, champ... ",
  "Gold star...not!",
  "You tried???????",
  "This is fine.   ",
  "Wow, so close!  ",
  "A for effort... ",
  "Keep dreaming!  ",
  "Sure, why not!  ",
  "Such wow! Much. ",
  "Zero chill!     ",
  "Don't trip!     ",
  "Let's taco 'bout!",
  "You're on fire! ",
  "Avoid humans!   ",
  "Run like Wi-Fi! ",
  "Stay weird!     ",
  "Adulting...lol! ",
  "May the snacks! ",
  "Brace yourself! ",
  "Ctrl+Alt+Win!   ",
  "Nap later!      ",
  "Pizza's waiting!",
  "You got this... ?",
  "Send coffee!    ",
  "Slay all day!   "
};


int messageInterval = 10; // Interval for displaying messages (in cycles)
const int messageDuration = 5000; // Display message for 5 seconds
unsigned long lastMessageTime = 0; // Last time a message was displayed
boolean showingMessage = false; // Flag to track if message is currently being shown

String repeatChar(char c, int count) {
  String result = "";
  for (int i = 0; i < count; i++) {
    result += c;
  }
  return result;
}

const int distanceBufferSize = 10; // Number of recent distance values to include in the rolling average
float distanceBuffer[distanceBufferSize]; // Buffer to store recent distances
int distanceBufferIndex = 0; // Index for circular buffer
float rollingAverageDistance = 0; // Rolling average distance

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the LCD
  Wire.begin();
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.setCursor(0, 0);

  // Initialize the sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  // Initialize the reset button pin
  pinMode(resetButtonPin, INPUT_PULLUP);

  // Initialize the speed buffer
  for (int i = 0; i < bufferSize; i++) {
    speedBuffer[i] = 0;
  }
  for (int i = 0; i < distanceBufferSize; i++) {
    distanceBuffer[i] = 0;
  }
}

const int debounceDelay = 50; // Debounce delay in milliseconds
unsigned long lastDebounceTime = 0; // Last time the button state changed
int lastButtonState = HIGH; // Previous state of the button




void loop() {
    int reading = digitalRead(resetButtonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset the debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
      // Reset all relevant variables
      passCount = 0;
      totalDistance = 0;
      bufferIndex = 0;
      for (int i = 0; i < bufferSize; i++) {
        speedBuffer[i] = 0;
      }
      movingAverageSpeed = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Reset done");
      delay(1000); // Wait for 1 second to show the reset message
      lcd.clear();
    }
  }

  lastButtonState = reading;
  // Measure distance
  digitalWrite(trigPin, LOW); // Set the trigger pin low
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(trigPin, HIGH); // Set the trigger pin high
  delayMicroseconds(10); // Wait for 10 microseconds
  digitalWrite(trigPin, LOW); // Set the trigger pin low
  duration = pulseIn(echoPin, HIGH); // Read the echo pin and get the duration of the pulse
  distance = duration * 0.034 / 2; // Calculate the distance in cm
  // scale distance down by 0.9600 to account for the difference in the sensor's distance reading
    distance = distance * 0.9600;

  distanceBuffer[distanceBufferIndex] = distance;
  distanceBufferIndex = (distanceBufferIndex + 1) % distanceBufferSize; // Move to next index, wrap around if necessary

  // Calculate the rolling average distance
  float distanceSum = 0;
  for (int i = 0; i < distanceBufferSize; i++) {
    distanceSum += distanceBuffer[i];
  }
  distance = distanceSum / distanceBufferSize;


  // Track local minimum (closest) and maximum (farthest) distance in a cycle
  if (distance < minDistance) {
    minDistance = distance; // Update minimum if current distance is closer
  }
  if (distance > maxDistance) {
    maxDistance = distance; // Update maximum if current distance is farther
  }

  // Check if the blade is close (within 15 cm) and detect when it moves away (above 20 cm)
  if (distance <= 11 && !bladeClose) { // Blade is close, start tracking
    bladeClose = true;
    minDistance = distance; // Reset min distance for this cycle
    startTime = millis(); // Record start time
  } else if (distance >= 16 && bladeClose) { // Blade moves far, end tracking
    bladeClose = false;
    bladeFar = true;
    maxDistance = distance; // Record the maximum distance the blade moved away
    endTime = millis(); // Record end time

    // Calculate speed (maxDistance - minDistance) / time
    float travelDistance = maxDistance - minDistance;
    float timeElapsed = (endTime - startTime) / 1000.0; // Convert ms to seconds
    speed = travelDistance / timeElapsed;

    totalDistance += travelDistance*2; // Add this cycle's travel distance to total

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
      lcd.print(messages[random(0, sizeof(messages) / sizeof(messages[0]))]); // Display a random message
      messageInterval = random(20, 51); // Randomize the interval for next message
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
    if (millis() % 4000 < 2000) {
        lcd.setCursor(0, 1);
        String speedStr = "Speed: " + String((int)speed) + " cm/s";
        if (speedStr.length() > 16) {
            speedStr = speedStr.substring(0, 16); // Truncate if too long
        } else {
            speedStr += repeatChar(' ', 16 - speedStr.length()); // Pad with spaces if too short
        }
        lcd.print(speedStr);
    } else {
        lcd.setCursor(0, 1);
        // Updated code for displaying total distance
        if (totalDistance > 1000) {
            lcd.print(String((totalDistance / 100)) + " meters" + repeatChar(' ', 16 - String((totalDistance / 100)).length() - 7));
        } else {
            lcd.print("Distance: " + String((int)totalDistance) + " cm" + repeatChar(' ', 16 - String("Distance: " + String((int)totalDistance) + " cm").length()));
        }
    }
  }

  delay(25); // Wait for 100 milliseconds before the next loop
}
