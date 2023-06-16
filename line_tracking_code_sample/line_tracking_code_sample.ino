#include <Adafruit_MotorShield.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_LSM303_U.h>
#include <hp_BH1750.h>  //light sensor

// left IR sensor is numbered even
// right IR sensor is numbered odd

#define IR_PIN_1 A0
#define IR_PIN_2 A1
#define IR_PIN_3 A2
#define IR_PIN_4 A3
#define LEFT_MOTOR_PIN_1 1
#define LEFT_MOTOR_PIN_2 2
#define RIGHT_MOTOR_PIN_3 3
#define RIGHT_MOTOR_PIN_4 4

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *LEFT_MOTOR_1 = AFMS.getMotor(LEFT_MOTOR_PIN_1);
Adafruit_DCMotor *LEFT_MOTOR_2 = AFMS.getMotor(LEFT_MOTOR_PIN_2);
Adafruit_DCMotor *RIGHT_MOTOR_3 = AFMS.getMotor(RIGHT_MOTOR_PIN_3);
Adafruit_DCMotor *RIGHT_MOTOR_4 = AFMS.getMotor(RIGHT_MOTOR_PIN_4);

int sensorValue1;
int sensorValue2;
int sensorValue3;
int sensorValue4;
int MAX_SPEED = 255;

hp_BH1750 lightSensor;  //

int lightTurnedOn = 0;  //kinda like a boolean
int LUX_THRESHOLD = 3000;

void setup() {
  //IR pins
  pinMode(IR_PIN_1, INPUT);
  pinMode(IR_PIN_2, INPUT);

  //driver
  AFMS.begin();

  //motors
  LEFT_MOTOR_1->setSpeed(0);
  LEFT_MOTOR_2->setSpeed(0);
  RIGHT_MOTOR_3->setSpeed(0);
  RIGHT_MOTOR_4->setSpeed(0);
  LEFT_MOTOR_1->run(RELEASE);
  LEFT_MOTOR_2->run(RELEASE);
  RIGHT_MOTOR_3->run(RELEASE);
  RIGHT_MOTOR_4->run(RELEASE);

  //light sensor
  lightSensor.begin(BH1750_TO_GROUND);
}

void loop() {

  //light sensor reading 
  lightSensor.start();
  int lux = lightSensor.getLux();

  // if light is shining on, switch. either GO or stop initiated
  if (lux > 3000) {
    if(lightTurnedOn==1){
      lightTurnedOn = 0;
    }else{
      lightTurnedOn = 1;
    }
  }


  if (lightTurnedOn == 1) { //bot is good to go
    following();
  }else{ //
    stop();
  }
}


void following(){
  
  //black returns 1, white returns 0
  sensorValue1 = digitalRead(IR_PIN_1);  //left sensor (Outer)
  sensorValue2 = digitalRead(IR_PIN_2);  //right sensor (Outer)
  sensorValue3 = digitalRead(IR_PIN_3);  //left sensor (Inner)
  sensorValue4 = digitalRead(IR_PIN_4);  //right sensor (Inner)

  // Directions: (assume the IR sensors are on either side of the black tape)
  if (sensorValue1 == 0 && sensorValue2 == 0 && sensorValue3 == 1 && sensorValue4 == 1) {
    straight();
  } else if (sensorValue1 == 0 && sensorValue3 == 0) {  //if inner and outer of left detects white, the robot will turn right
    right();
  } else if (sensorValue2 == 0 && sensorValue4 == 0) {  // Opposite to above explanation
    left();
  }
}

void straight() {
  //Directions:
  LEFT_MOTOR_1->run(BACKWARD);
  LEFT_MOTOR_2->run(FORWARD);
  //remember, left and right motors are set up in opposite directions
  RIGHT_MOTOR_3->run(FORWARD);
  RIGHT_MOTOR_4->run(BACKWARD);

  //Speed:
  LEFT_MOTOR_1->setSpeed(MAX_SPEED);
  LEFT_MOTOR_2->setSpeed(MAX_SPEED);
  RIGHT_MOTOR_3->setSpeed(MAX_SPEED);
  RIGHT_MOTOR_4->setSpeed(MAX_SPEED);
}

void right() {
  //Directions:
  LEFT_MOTOR_1->run(BACKWARD);
  LEFT_MOTOR_2->run(FORWARD);
  //remember, left and right motors are set up in opposite directions
  RIGHT_MOTOR_3->run(FORWARD);
  RIGHT_MOTOR_4->run(BACKWARD);

  //Speed:
  //note: left goes slow, right stays still
  LEFT_MOTOR_1->setSpeed(255);  //less than 20% of max speed
  LEFT_MOTOR_2->setSpeed(255);  //less than 20% of max speed
  RIGHT_MOTOR_3->setSpeed(70);  //
  RIGHT_MOTOR_4->setSpeed(70);
}

void left() {
  //Directions:
  LEFT_MOTOR_1->run(BACKWARD);
  LEFT_MOTOR_2->run(FORWARD);
  //remember, left and right motors are set up in opposite directions
  RIGHT_MOTOR_3->run(FORWARD);
  RIGHT_MOTOR_4->run(BACKWARD);

  //Speed:
  //note: right goes slow, left stays still
  LEFT_MOTOR_1->setSpeed(70);  //less than 20% of max speed
  LEFT_MOTOR_2->setSpeed(70);  //less than 20% of max speed
  RIGHT_MOTOR_3->setSpeed(255);
  RIGHT_MOTOR_4->setSpeed(255);
}

void stop() {
  LEFT_MOTOR_1->setSpeed(0);
  LEFT_MOTOR_2->setSpeed(0);
  RIGHT_MOTOR_3->setSpeed(0);
  RIGHT_MOTOR_4->setSpeed(0);
  LEFT_MOTOR_1->run(RELEASE);
  LEFT_MOTOR_2->run(RELEASE);
  RIGHT_MOTOR_3->run(RELEASE);
  RIGHT_MOTOR_4->run(RELEASE);

  //restart initialization
  lightTurnedOn = 0;
}
