from machine import Pin, I2C, time_pulse_us
import utime
from lcd_api import LcdApi
from pico_i2c_lcd import I2cLcd

# Pin definitions
trigPin = Pin(14, Pin.OUT)
echoPin = Pin(15, Pin.IN)

# I2C setup for LCD
I2C_ADDR = 63
I2C_NUM_ROWS = 2
I2C_NUM_COLS = 16
i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=400000)
lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)

# Variables for distance measurement
duration = 0
distance = 0

# Variables for counting passes
passCount = 0
bladeClose = False
startTime = 0
endTime = 0
speed = 0
totalDistance = 0

# Moving average variables
bufferSize = 10
speedBuffer = [0] * bufferSize
bufferIndex = 0
movingAverageSpeed = 0

# Motivational messages
messages = [
    "Keep going!",
    "You're doing great!",
    "Almost there!",
    "Stay strong!",
    "You can do it!"
]
messageInterval = 10
messageDuration = 20000
lastMessageTime = 0

def measure_distance():
    global duration, distance
    trigPin.low()
    utime.sleep_us(2)
    trigPin.high()
    utime.sleep_us(10)
    trigPin.low()
    duration = time_pulse_us(echoPin, 1)
    distance = (duration * 0.0343) / 2

def update_speed_buffer(speed):
    global bufferIndex, movingAverageSpeed
    speedBuffer[bufferIndex] = speed
    bufferIndex = (bufferIndex + 1) % bufferSize
    movingAverageSpeed = sum(speedBuffer) / bufferSize

lcd.putstr("Microtome Counter\nInitializing...")

while True:
    measure_distance()

    if distance <= 15 and not bladeClose:
        bladeClose = True
        startTime = utime.ticks_ms()
    elif distance >= 20 and bladeClose:
        bladeClose = False
        endTime = utime.ticks_ms()
        passCount += 1
        speed = 5.0 / (utime.ticks_diff(endTime, startTime) / 1000.0)
        totalDistance += 5.0
        update_speed_buffer(speed)

        print(f"Pass Count: {passCount}")
        print(f"Speed: {speed:.2f} cm/s")
        print(f"Moving Avg Speed: {movingAverageSpeed:.2f} cm/s")
        print(f"Total Distance: {totalDistance:.2f} cm")

        lcd.clear()
        lcd.putstr(f"Passes: {passCount}\n")

        if passCount % messageInterval == 0 and utime.ticks_diff(utime.ticks_ms(), lastMessageTime) > messageDuration:
            lcd.clear()
            lcd.putstr(messages[utime.ticks_ms() % len(messages)])
            lastMessageTime = utime.ticks_ms()
            messageInterval = utime.ticks_ms() % 41 + 10

    if utime.ticks_ms() % 2000 < 1000:
        lcd.move_to(0, 1)
        lcd.putstr(f"Speed: {speed:.2f} cm/s")
    else:
        lcd.move_to(0, 1)
        lcd.putstr(f"Distance: {totalDistance:.2f} cm")

    utime.sleep_ms(100)