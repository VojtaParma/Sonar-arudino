#include <Servo.h>

// set output pins
const int TriggerPin = 9;
const int EchoPin = 10;
const int motorSignalPin = 2;

// starting location
const int startAngle = 90;

// rotation limits
const int minimumAngle = 6;
const int maximumAngle = 175;

// speed (reduced for more frequent measurements)
const int degreesPerCycle = 2; // Smaller steps for more frequent measurements

// Library class instance
Servo motor;

void setup(void) 
{
    pinMode(TriggerPin, OUTPUT);
    pinMode(EchoPin, INPUT);
    motor.attach(motorSignalPin);
    Serial.begin(9600);
}

void loop(void)
{
    static int currentAngle = startAngle;
    static int motorRotateAmount = degreesPerCycle;

    // move motor
    motor.write(currentAngle);
    delay(50); // Reduced delay for faster response

    // calculate the distance from the sensor, and write the value with location to serial
    int distance = CalculateDistance();
    SerialOutput(currentAngle, distance);

    // update motor location
    currentAngle += motorRotateAmount;

    // if the motor has reached the limits, change direction
    if(currentAngle <= minimumAngle || currentAngle >= maximumAngle) 
    {
        motorRotateAmount = -motorRotateAmount;
    }
}

int CalculateDistance(void)
{
    // trigger the ultrasonic sensor and record the time taken to reflect
    digitalWrite(TriggerPin, LOW);
    delayMicroseconds(2); // Ensure the trigger pin is low for a short period
    digitalWrite(TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TriggerPin, LOW);

    long duration = pulseIn(EchoPin, HIGH, 30000); // Added timeout for pulseIn to prevent long waits
    if (duration == 0) {
        return -1; // Return -1 if no pulse was received
    }

    // convert this duration to a distance
    float distance = duration * 0.034 / 2.0; // Speed of sound is 0.034 cm/us, divided by 2 for round trip
    return int(distance);
}

void SerialOutput(const int angle, const int distance)
{
    if (distance == -1) {
        Serial.print("No echo received at angle ");
        Serial.println(angle);
    } else {
        // send the angle and distance directly to the serial output
        Serial.print(angle);
        Serial.print(",");
        Serial.println(distance);
    }
}
