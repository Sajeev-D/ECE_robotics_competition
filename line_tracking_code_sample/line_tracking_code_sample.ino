#include <Adafruit_MotorShield.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_LSM303_U.h>
#include <hp_BH1750.h>//light sensor
#define IR_PIN_1 A0
#define IR_PIN_2 A1
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
int MAX_SPEED = 255;

hp_BH1750 lightSensor;//

int lightTurnedOn = 0;//kinda like a boolean
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

  //light sensor (robot initialization step)
  if(lightTurnedOn == 0){
    lightSensor.start();
    int lux = lightSensor.getLux();

    // if light is shining on
    if(lux>3000){
      lightTurnedOn = 1;
    }

  }



  if(lightTurnedOn == 1){

    //black returns 1, white returns 0
    sensorValue1 = digitalRead(IR_PIN_1); //left sensor
    sensorValue2 = digitalRead(IR_PIN_2); //right sensor

    // Directions: (assume the IR sensors are on either side of the black tape)
    if (sensorValue1==0 && sensorValue2==0) { //on either side of the tape
      //straight
      straight();
    } else if (sensorValue1==1 && sensorValue2==1){//stop, T shape end
      //stop
      stop();
    } else if( sensorValue1==1){ //left turn
      //left 
      left();
    } else if (sensorValue2==1){ //right turn
      //right
      right();
    }
  }



}

void straight(){
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

void right(){
  //Directions:
  LEFT_MOTOR_1->run(BACKWARD);
  LEFT_MOTOR_2->run(FORWARD);
    //remember, left and right motors are set up in opposite directions
  RIGHT_MOTOR_3->run(FORWARD);
  RIGHT_MOTOR_4->run(BACKWARD);

  //Speed:
    //note: left goes slow, right stays still
  LEFT_MOTOR_1->setSpeed(255);//less than 20% of max speed
  LEFT_MOTOR_2->setSpeed(255);//less than 20% of max speed
  RIGHT_MOTOR_3->setSpeed(70); //
  RIGHT_MOTOR_4->setSpeed(70);
}

void left(){
  //Directions:
  LEFT_MOTOR_1->run(BACKWARD);
  LEFT_MOTOR_2->run(FORWARD);
    //remember, left and right motors are set up in opposite directions
  RIGHT_MOTOR_3->run(FORWARD);
  RIGHT_MOTOR_4->run(BACKWARD);

  //Speed:
    //note: right goes slow, left stays still
  LEFT_MOTOR_1->setSpeed(70);//less than 20% of max speed
  LEFT_MOTOR_2->setSpeed(70);//less than 20% of max speed
  RIGHT_MOTOR_3->setSpeed(255);
  RIGHT_MOTOR_4->setSpeed(255);
}

void stop(){
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
