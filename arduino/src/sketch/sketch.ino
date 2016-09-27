#include "AccelStepper.h"
#include "axisDirectionStruct.h" //importing AxisAndDirection 
#include "MultiStepper.h"

String incomingByte;   // for incoming serial data
int speed = 20;
// The X Stepper pins
#define STEPPER1_DIR_PIN 2
#define STEPPER1_STEP_PIN 3

#define STEPPER2_DIR_PIN 8
#define STEPPER2_STEP_PIN 9

//#define STEPPER3_DIR_PIN 10
//#define STEPPER3_STEP_PIN 2
#define led_pin 13

/** Define some stepper and the pins the will use
AccelStepper::DRIVER means that we are using Driver, or we can write simply "1"
stepper1 : x-axis movement
stepper2, stepper3 : y and z axis movement
when both motor move in same direction with same speed : y axis, and when both motor moves in different direction with same speed : z axis
*/
AccelStepper stepper1(AccelStepper::DRIVER, STEPPER1_DIR_PIN, STEPPER1_STEP_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, STEPPER2_DIR_PIN, STEPPER2_STEP_PIN);

//To add multiple steppers to MultiStepper
MultiStepper steppers;
void setup()
{
  
	Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  //Set Max speed of steppers
	stepper1.setMaxSpeed(10);
  stepper2.setMaxSpeed(10);
  stepper1.moveTo(0);
  stepper2.moveTo(0);
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
}

void blockingRunSpeedToPosition(long position)
{
	/*
		runSpeedToPosition is non blocking.
		You must call this as frequently as possible, but at least once per step interval,		
		But we want blocking so we have to implement own loop using while
	*/

	stepper1.setCurrentPosition(0);
  stepper2.setCurrentPosition(0);
  stepper1.setAcceleration(100);
  stepper2.setAcceleration(100);
     /**stepper.moveTo function : Set the target position absolute to the current position
        stepper.moveTo(position);
        stepper.move funtion : Set the target position relative to the current position
       if position is negative then anticlockwise from the current position, else clockwise from current position
       */
//    stepper1.move(position);
//    stepper1.setSpeed(speed);
//    stepper.setAcceleration(100.0);
//    while (stepper1.distanceToGo() != 0)
//      stepper1.runSpeedToPosition();

  long positions[2];
  positions[0] = position;
  positions[1] = position;
//  steppers.setCurrentPosition(0);
  steppers.moveTo(positions);
  steppers.runSpeedToPosition();
}


void loop()
{
   while (Serial.available()==0) { }  //Wait for serial data
   incomingByte=Serial.readString();
   incomingByte.trim();
   Serial.println(incomingByte);
	  AxisAndDirection axisAndDirection = getAxisAndDirection(incomingByte);
		int stepsPerKeyStrock = 200;
		if(axisAndDirection.axis == 'y') {
      digitalWrite(led_pin,HIGH);
			if(axisAndDirection.direction == -1) {
				stepsPerKeyStrock = -stepsPerKeyStrock;
			}
			blockingRunSpeedToPosition(stepsPerKeyStrock);
	}else{
	       digitalWrite(led_pin,LOW);
	  }
// delay(500);
}

AxisAndDirection getAxisAndDirection(String data){
  if(data.equals("x")){
    return {'x', 1};
    }else if(data.equals("x-")){
      Serial.println("data is : " +data);
    return {'x', -1};
    }else if(data.equals("y")){
    return {'y', 1};
    }else if(data.equals("y-")){
    return {'y', -1};
    }else{
      return{'A',0};
      }

  
//	switch(directionSign(incomingByte)) {
//		case +x:
//			return {'x', 1};
//		case -x:
//			return {'x', -1};
//		case +y:
//			return {'y',1};
//		case -y:
//			return {'y',-1};
//		default:
//			return {'A',0};
//	}
}


