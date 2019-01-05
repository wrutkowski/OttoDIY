// OttoDIY main code
// Author: Wojciech Rutkowski, 05/01/2019
// MIT License

#include <Arduino.h>
#include <Servo.h>
#include <US.h>

// connections
#define HIP_LEFT_PIN        2
#define HIP_RIGHT_PIN       3
#define ANKLE_LEFT_PIN      4
#define ANKLE_RIGHT_PIN     5

#define ULTRASONIC_TRIG_PIN 8
#define ULTRASONIC_ECHO_PIN 9

// movement
#define HIP_LEFT    0
#define HIP_RIGHT   1
#define ANKLE_LEFT  2
#define ANKLE_RIGHT 3

#define FORWARD     1
#define BACKWARD    -1

Servo servos[4];
int servoTrims[] = { -8, 0, 0, 0 };
US ping;

void writeServo(int number, int value);
void resetPosition();
void walk(int direction = FORWARD);

void setup() {
    
    Serial.begin(19200);
    Serial.println("Starting...");

    delay(1000);

    servos[HIP_LEFT].attach(HIP_LEFT_PIN);
    servos[HIP_RIGHT].attach(HIP_RIGHT_PIN);
    servos[ANKLE_LEFT].attach(ANKLE_LEFT_PIN);
    servos[ANKLE_RIGHT].attach(ANKLE_RIGHT_PIN);

    resetPosition();

    ping.init(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN);

    Serial.println("READY!");
}

void loop() {

    int distance = ping.read();

    Serial.print("Ping: ");
    Serial.println(distance);

    if (distance < 10) {
        walk(BACKWARD);
    } else if (distance > 20 && distance < 50) {
        walk();
    }
}

void writeServo(int number, int value) {

    servos[number].write(value + servoTrims[number]);
    Serial.print("servo ");
    Serial.print(number);
    Serial.print(" -> ");
    Serial.println(value);
}

void resetPosition() {

    for (int i = 0; i < 4; ++i) {
        writeServo(i, 90);
    }
}

void walk(int direction) {

    // safety check - constrain direction to be -1 or 1 only
    if (direction != 1 && direction != -1) {
        direction = 1;
    }

    int ankleChange = 20;
    int hipChange = 60 * direction;
    int ankleChangeTime = 50;
    int hipChangeTime = 150;

    // left
    writeServo(ANKLE_LEFT, 90 - ankleChange);
    delay(ankleChangeTime);
    writeServo(HIP_LEFT, 90 + hipChange);
    delay(hipChangeTime);
    writeServo(ANKLE_LEFT, 90);
    delay(ankleChangeTime);

    // right
    writeServo(ANKLE_RIGHT, 90 + ankleChange);
    delay(ankleChangeTime);
    writeServo(HIP_LEFT, 90);
    writeServo(HIP_RIGHT, 90 - hipChange);
    delay(hipChangeTime);
    writeServo(ANKLE_RIGHT, 90);
    delay(ankleChangeTime);

    // reset
    writeServo(HIP_RIGHT, 90);
    delay(hipChangeTime);

}
