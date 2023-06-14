#include <Adafruit_MotorShield.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

#define IR_PIN A0
#define LINE_THRESHOLD 500
#define POS_POLARITY_MOTOR_PIN_1 1
#define POS_POLARITY_MOTOR_PIN_2 2
#define NEG_POLARITY_MOTOR_PIN_3 3
#define NEG_POLARITY_MOTOR_PIN_4 4

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *POS_MOTOR_1 = AFMS.getMotor(POS_POLARITY_MOTOR_PIN_1);
Adafruit_DCMotor *POS_MOTOR_2 = AFMS.getMotor(POS_POLARITY_MOTOR_PIN_2);
Adafruit_DCMotor *NEG_MOTOR_3 = AFMS.getMotor(NEG_POLARITY_MOTOR_PIN_3);
Adafruit_DCMotor *NEG_MOTOR_4 = AFMS.getMotor(NEG_POLARITY_MOTOR_PIN_4);

int sensorValue = 0;
int leftSpeed = 0;   //for left wheels
int rightSpeed = 0;  // for right wheels

void setup() {
  pinMode(IR_PIN, INPUT);
  AFMS.begin();  //sets up the communication between the microcontroller (Arduino) and the Adafruit Motor Shield (Similar to serial.begin). Not sure if necessary.
  POS_MOTOR_1->setSpeed(255);
  POS_MOTOR_2->setSpeed(255);
  NEG_MOTOR_3->setSpeed(255);
  NEG_MOTOR_4->setSpeed(255);
  POS_MOTOR_1->run(RELEASE);
  POS_MOTOR_2->run(RELEASE);
  NEG_MOTOR_3->run(RELEASE);
  NEG_MOTOR_4->run(RELEASE);
}

void loop() {
  sensorValue = analogRead(IR_PIN);

  // Speeds:
  if (sensorValue > LINE_THRESHOLD) {  //LINE_THRESHOLD defined above
    leftSpeed = 200;
    rightSpeed = 200;
  } else {
    leftSpeed = -200;
    rightSpeed = 200;
  }

  POS_MOTOR_1->setSpeed(abs(leftSpeed));  //abs converts int to an absolute value (no negative)
  POS_MOTOR_2->setSpeed(abs(rightSpeed));
  NEG_MOTOR_3->setSpeed(abs(leftSpeed));
  NEG_MOTOR_4->setSpeed(abs(rightSpeed));

  // Directions:
  if (leftSpeed >= 0) {
    POS_MOTOR_1->run(FORWARD);  // Forward and backward are keywords. They are part of Adafruit_MotorShield.h library
    NEG_MOTOR_3->run(FORWARD);
  } else {
    POS_MOTOR_1->run(BACKWARD);
    NEG_MOTOR_3->run(BACKWARD);
  }

  if (rightSpeed >= 0) {
    POS_MOTOR_2->run(FORWARD);
    NEG_MOTOR_4->run(FORWARD);
  } else {
    POS_MOTOR_2->run(BACKWARD);
    NEG_MOTOR_4->run(BACKWARD);
  }
}
