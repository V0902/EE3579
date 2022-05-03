#ifndef MOTOR_H
#define MOTOR_H

#include<Arduino.h>
#include <Basic_Input.h>
#include <Beginner_LED.h>
#include <InterruptBasedSpeedMeasure.h>
#include <IntervalCheckTimer.h>


class Motor{
protected:

	//pins (button & potentiometer)
	int startButtonPin, stopButtonPin, reverseButtonPin, potentiometerPin;
	//0 - start, 1 - stop, 2 - change dir
	int previousDetection[3] = {HIGH, HIGH, HIGH};
	//securing from spurious reads.
	int previousReads[2] = {0, 0};
	//motor power control
	int powerPin;
	ana_LED power;
	bool isOn;
	bool pid;
	int currentPow;
	//motor direction control
	int directionPin;
	bool isClockwise;
	//buttons
	//startB, stopB, revB;
	in_digital  digitalPins[3];
	
	//time between button presses
	int delayTime;
	long int lastButtonPress;

	//jumpstart
	int jumpstartValue;
	int minValue;
	bool dontUpdate;

public:
	void reset(){
		delayTime = 1200;
		pid = false;
		isClockwise = true;
		isOn = false;
		dontUpdate = false;
		jumpstartValue = 90;
		minValue = 50;
	}
	Motor(){
		reset();
	}
	//setting of all the pins. boring stuff
	//TODO IDEA: check which pins have already been taken
	void setStartButton(int pin){
		if(pin<0 || pin > 13){
			Serial.println("Fatal error - bad pin number for Arduino Uno.");
			return;
		}
		else{
			startButtonPin = pin;
			digitalPins[0].setup_in_digital(startButtonPin, true);
		}
	}
	void setStopButton(int pin){
		if(pin<0 || pin > 13){
			Serial.println("Fatal error - bad pin number for Arduino Uno.");
			return;
		}
		else{
			stopButtonPin = pin;
			digitalPins[1].setup_in_digital(stopButtonPin, true);
		}
	}
	void setReverseButton(int pin){
		if(pin<0 || pin > 13){
			Serial.println("Fatal error - bad pin number for Arduino Uno.");
			return;
		}
		else{
			stopButtonPin = pin;
			digitalPins[2].setup_in_digital(stopButtonPin, true);
		}
	}
	void setPotentiometerPin(int pin){
		if(pin < A0 || pin > A5){ //for analog pins
			Serial.println("Fatal error - potentiometer pin is not recognized. Aborting...");
			return;
		}
		potentiometerPin = pin;
	}
	void setDirectionPin(int pin){
		if(pin<0 || pin > 13){
			Serial.println("Fatal error - bad pin number for Arduino Uno.");
			return;
		}
		else{
			directionPin = pin;
			pinMode(directionPin, OUTPUT);
		}
	}
	void setPowerPin(int pin){
		if(pin!= 3 && pin !=5 && pin!=6 && pin!=9 && pin!= 10 && pin !=11){
			Serial.println("Fatal error - bad pin number for Arduino Uno. Choose variable digital pin.");
			return;
		}
		else{
			powerPin = pin;
			power.setup_LED(powerPin);
		}
	}
	void switchDirection(){
		isClockwise = !isClockwise;
		digitalWrite(directionPin, isClockwise);
	}
	//button checks are performed on update()
	void switchOn(){

		power.switch_on();
		if(power.get_brightness() < jumpstartValue && !pid){
			power.set_brightness(jumpstartValue);
			}
		//Serial.println("Switching on")
		isOn = true;
	}
	void switchOff(){
		power.switch_off();
		//Serial.println("Switching off")
		isOn = false;
	}
	//resolves from 0 to 255
	void setPower(int level){
		//checks done inside the class
		//Serial.println(level);
		power.set_brightness(level);
	}
	
	int readPotentiometer(){
		//in case the pin is set wrong, ensure that the motor wont spin
		if(potentiometerPin < A0 || potentiometerPin > A5){
			return 0;
		}
		else{
			return analogRead(potentiometerPin);
		}
	}
	void update(){
		dontUpdate = false;
		//take a look at buttons if delay elapsed
		if(millis()-lastButtonPress > delayTime){
			//inspect all buttons
			for (int i = 0; i < 3; i++) {
				//detection variable, pass by reference.
				int detection;
				digitalPins[i].read_input(detection);
				if (detection == LOW) {
					//checking whether it's a new button press
					if(previousDetection[i] == HIGH){
						lastButtonPress = millis();
					}
					previousDetection[i] = LOW;
					//an enormous part of the heavy-lifting is done
					//inside provided libraries. I don't have to check
					//for whether the device is swtiched on or not as it's
					//already done implicitely in Beginner_LED
					if(i == 0){
						switchOn();
						dontUpdate = true;
					}
					else if(i == 1){
						switchOff();
					}
					else if(i == 2){
						switchDirection();
					}

				}			
				else if (detection == HIGH){
					previousDetection[i] = HIGH;
				}

			}
			updatePotentiometer();
		}
	}
	bool on(){return isOn;}

	void updatePotentiometer(){
				//The potentiometer also waits (for jumpstart for example). From 0-1023 we go to 0-255, so
				//a simple floor division by 4 suffices, and also minimizes noise
				//only if it is on.
				//It is in the same "if" as the lastButtonPress might have been updated.
				if(isOn && !dontUpdate && !pid){
					currentPow = readPotentiometer()/4;
					//securing against singular spurious reading. checking if between two reads ago and now
					//there hasn't been a weird reading that makes the programme behave bad.
					//update if all the values are different - user is spinning the knob
					if((currentPow != previousReads[0] && previousReads[1] != currentPow) || (currentPow == previousReads[1])){
						//motor will stop below a certain value (with the LED)
						currentPow = max(currentPow, minValue);
						setPower(currentPow);
					}
					//otherwise, do nothing and keep the old reading.
					//update the array
					previousReads[0] = previousReads[1];
					previousReads[1] = currentPow;

				}
	}
	int getPwm(){
		return power.get_brightness();
	}

};

class pidMotor: public Motor{
protected:
	int targetRpm;

public:
	pidMotor(){
		pid = true;

	}
	//overwriting updatePotentiometer() to reflect that spinning the potentiometer changes the target RPM.
	void updatePotentiometer(){
		//this function will still be called inside update() from the base class
		if(isOn && !dontUpdate){
		//linear mapping. 0-255 to 600 - 10000 (more or less). I will first perform a floor division to get rid of some noise
		//from the potentiometer reads
		if((currentPow != previousReads[0] && previousReads[1] != currentPow) || (currentPow == previousReads[1])){
			targetRpm = readPotentiometer()/4 * 37 + 600;
		}
		previousReads[0] = previousReads[1];
		previousReads[1] = currentPow;

		}
	}
	//this is upsetting, as I just wanted to "cheat" one function in, but it doesn't seem to work as it will call the base class and all the definitions from the base class.
	void update(){
		dontUpdate = false;
		//take a look at buttons if delay elapsed
		if(millis()-lastButtonPress > delayTime){
			//inspect all buttons
			for (int i = 0; i < 3; i++) {
				//detection variable, pass by reference.
				int detection;
				digitalPins[i].read_input(detection);
				if (detection == LOW) {
					//checking whether it's a new button press
					if(previousDetection[i] == HIGH){
						lastButtonPress = millis();
					}
					previousDetection[i] = LOW;
					//an enormous part of the heavy-lifting is done
					//inside provided libraries. I don't have to check
					//for whether the device is swtiched on or not as it's
					//already done implicitely in Beginner_LED
					if(i == 0){
						switchOn();
						dontUpdate = true;
					}
					else if(i == 1){
						switchOff();
					}
					else if(i == 2){
						switchDirection();
					}

				}			
				else if (detection == HIGH){
					previousDetection[i] = HIGH;
				}

			}
			updatePotentiometer();
		}
	}
	int getTargetRpm(){
		return targetRpm;
	}

};

class PositionalMotor: public Motor{
protected:
	IntervalCheckTimer speedCheck;
	InterruptSpeedMeasure rotationCounter;
	//
	//int rotationStart;

	int rotationEnd; 
	//^ is found by:
	double rotationWhenBraking = 0;

	long int timer;

	int targetRotations;
	int rotationalSpeed;

	double remainingRotations;
	double totalRotations;
	double takenRotations;

	bool startChecking = false;


public:
	PositionalMotor(){
		rotationCounter.setupSpeedMeasure(int_1);
		//updating the rotation counter every 100 ms.
		speedCheck.setInterCheck(100);
		setMaxSpeed();
	}
	//experimentally found how many rotations it takes from 0 to rotationalSpeed and vice versa.
	void inputRotations(int fT){
		//rotationStart = rT;
		rotationEnd = fT;
	}
	void setMaxSpeed(int pwm = 255){
		rotationalSpeed = pwm;
	}
	void inputTargetRotations(int tR){
		targetRotations = tR;
		if(rotationEnd != 0){
			remainingRotations = targetRotations - rotationEnd;
		}
	}
	void updatePotentiometer(){
		//don't do anything.
	}
	void switchOn(){
		if(!isOn){
			power.switch_on();
			power.set_brightness(rotationalSpeed);
			isOn = true;
		}


	}
	void update(){
		switchOn();
		//update every 100ms
		if(speedCheck.isMinChekTimeElapsedAndUpdate()){
			//rotations that happened within those 100ms
			double rotationsSinceLastCheck = rotationCounter.getRPMandUpdate()/600;
			timer +=1;
			totalRotations+=rotationsSinceLastCheck;
			remainingRotations-=rotationsSinceLastCheck;
			//reached the slowdown period - turn off the engine
			if(remainingRotations < 0){
				power.set_brightness(0);
			}
			if(rotationsSinceLastCheck == 0 && !startChecking){
				onFinish();
				//reusing the variable from test()
				startChecking = true;
			}
		}
	}
	void onFinish(){
		  Serial.print("The total rotations we're looking to achieve are: "); Serial.println(targetRotations); 
		  Serial.print("The total rotations we ended with are: "); Serial.println(totalRotations); 
		  Serial.print("And it only took us "); Serial.print((double)timer/10); Serial.println(" seconds!");
	}
	void test(){
		  if(Serial.available() == 1){
			if (Serial.parseInt() == 1){
				Serial.println("1 clicked");
				switchOn();
				//give six seconds for speeding up
				delay(6000);
				switchOff();
				startChecking = true;
			}
		  }
		  if(startChecking){
		  	if(speedCheck.isMinChekTimeElapsedAndUpdate()){
			  	double rotationsSinceLastCheck = rotationCounter.getRPMandUpdate()/600;
				rotationWhenBraking+=rotationsSinceLastCheck;
				Serial.println(rotationWhenBraking);
				//last result will be on Serial
				if(rotationsSinceLastCheck == 0){
					startChecking = false;
					rotationWhenBraking = 0;
				}
				}
		  }
		}
	
};

#endif