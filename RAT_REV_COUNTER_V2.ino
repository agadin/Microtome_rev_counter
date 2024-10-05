// library imports
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// variable setup
int count = 0; 
int prevCount = -1;
boolean countFlag = LOW; 
File dailyFile; 
String fileName; 
LiquidCrystal_I2C lcd(0x27,16,2); // create object for lcd
const int hallSensorPin = 2; // connect the hall effect sensor on pin 2

void setup() {
  // initialize serial monitor
  Serial.begin(9600);

  // initialize hall effect sensor, lcd
  pinMode(hallSensorPin, INPUT);
  lcd.init();

  // print a message to the lcd
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initializing");
  delay(1000);
  lcd.clear();
  lcd.print("Welcome to Rat Rev Counter");
  for (int positionCounter = 0; positionCounter < 26; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(350);
  }

  // initialize sd card
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // initialize clock
  setTime(RTC.get());
  dailyReset(); // creates file for day 1

  delay(1000);
  lcd.clear();
}

void loop() {
  // check for reset
  if (returnTime() == "07:00:00") {
    dailyReset();
    delay(1000);
  }

  // check for sensor, increment count
  if (digitalRead(hallSensorPin) == HIGH) {
    countFlag = HIGH;
  }
  if (digitalRead(hallSensorPin) == LOW && countFlag == HIGH) {
    countFlag = LOW;
    count++;
    Serial.println(count);

    // write to SD card
    dailyFile = SD.open(fileName, FILE_WRITE); 

    // if the file opened okay, write to it:
    if (dailyFile) {
      dailyFile.println(String(count) + "  " + String(millis()) + "  " + returnDate() + "T" + returnTime()); //oriented this way for MATLAB
      
      // close the file:
      dailyFile.close();
    }

    Serial.println(returnTime());
    delay(100);
  }

  //update display
  if (count != prevCount) {
    lcd.setCursor(0, 0);
    lcd.print("REVS = " + String(count));
    prevCount = count;
  }

  lcd.setCursor(0, 1);
  lcd.print("TIME = " + returnTime());

  //set time
  setTime(RTC.get());
}

// time functions
String addZero(int n) {
  String value = String(n);
  
  if (n < 10) {
    value = "0" + String(n);
  }

  return value;
}

String returnDate() { //returns date in YYYY-MM-DD
  String dateStr = String(year()) + "-" + addZero(month()) + "-" + addZero(day());

  return dateStr;
}

String returnTime() { //returns time in HH:MM:SS
  String timeStr = addZero(hour()) + ":" + addZero(minute()) + ":" + addZero(second());

  return timeStr;
}

void dailyReset() {
  //create new daily file 
  fileName = String(year() - 2000) + "-" + addZero(month()) + "-" + addZero(day()) + ".txt";      //YY-MM-DD.txt (for 8.3 file name restrictions)

  dailyFile = SD.open(fileName, FILE_WRITE);

  if (dailyFile) {
    Serial.print("Writing to " + fileName + "...");
    dailyFile.println("Revs  Milliseconds  Timestamp");

    //prints current count w/millis as "zero" value
    dailyFile.println(String(count) + "  " + String(millis()) + "  " + returnDate() + "T" + returnTime()); //oriented this way for MATLAB

    // close file
    dailyFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening " + fileName);  
  }
}