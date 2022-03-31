#ifndef MOTOR_H
#define MOTOR_H

#include<Arduino.h>
#include <Basic_Input.h>
#include <Beginner_LED.h>


class Motor{
protected:

	//pins (button & potentiometer)
	int startButtonPin, stopButtonPin, reverseButtonPin, potentiometerPin;
	//0 - start, 1 - stop, 2 - change dir
	int previousDetection[3] = {HIGH, HIGH, HIGH};
	//motor power control
	int powerPin;
	ana_LED power;
	bool isOn;
	int currentPow;
	//motor direction control
	int directionPin;
	bool isClockwise;
	//buttons
	//startB, stopB, revB;
	in_digital  digitalPins[3];
	
	//time between button presses
	int delayTime = 200;
	long int lastButtonPress;

public:
	void reset(){
		isClockwise = true;
		isOn = false;
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
		if(pin!= 3 || pin !=5 || pin!=6 || pin!=9 || pin!= 10 ||pin !=11){
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
		isOn = true;
	}
	void switchOff(){
		power.switch_off();
		isOn = false;
	}
	//resolves from 0 to 255
	void setPower(int level){
		//checks done inside the class
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
					//for whether the device is wtiched on or not as it's
					//already done implicitely in Beginner_LED
					Serial.println(i);
					if(i == 0){
						switchOn();
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
		}
		//always look at the potentiometer. From 0-1023 we go to 0-255, so
		//a simple floor division by 4 suffices, and also minimizes noise
		//only if it is on.
		if(isOn){
			currentPow = readPotentiometer()/4;
			setPower(readPotentiometer()/4);
		}


	}

};

#endif