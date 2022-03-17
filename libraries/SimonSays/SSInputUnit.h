#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include <Basic_Input.h>

class inputUnit {
protected:
	//array of binary inputs, for each button.

	bool binaryInputs[5] = { false, false, false, false, false };
	int previousDetection[5] = {HIGH, HIGH, HIGH, HIGH, HIGH};
	in_digital digitalPins[5];
	int analogVal;
	int size;
	int analogPin;
	int difficulty;
	unsigned long lastButtonPress;
	int breakBetweenPresses;
	bool canPressButton;
public:
	inputUnit() {
		reset();
	}

	inputUnit(int givenSize) {
		reset();
		setSize(givenSize);
	}
	void reset(){
		canPressButton = true;
		breakBetweenPresses = 300;
		size = 5;
	}
	void setSize(int givenSize){
	if(size >5 || size < 0){
		Serial.println("fatal error - supplied size is bigger than allowed. Aborting...");
		return;
	}
		size = givenSize;
	}
	void setPotentiometer(int pin){
	if(pin < A0 || pin > A5){ //for analog pins
		Serial.println("Fatal error - potentiometer pin is not recognized. Aborting...");
		return;
	}
		analogPin = pin;
	}
	//small scale time management. 
	void update(){
		//this is largely irrelevant, but prevents two button presses at almost the same time.
		if(!canPressButton && millis() - lastButtonPress >= breakBetweenPresses){
			canPressButton = true;
		}
	}
	void setDigitalPins(int pins[5]) {
		for (int i = 0; i < size; i++) {
			//initializaiton of the in_digital array.
			
			if(pins[i] < 0 || pins[i] > 13){
				Serial.println("Fatal error - bad pin number for Arduino Uno.");
				return;
			}
			digitalPins[i].setup_in_digital(pins[i], true);
		}
	}
	int setDifficultyPin(){
		analogVal = analogRead(analogPin);
		difficulty = (analogVal<=200) ? 1 : ((analogVal <=400) ? 2 : ((analogVal<= 600) ? 3 : ((analogVal<= 800) ? 4 : 5)));
		return difficulty;
	}
	void readBinaryInputs() {
		if(canPressButton){
			for (int i = 0; i < size; i++) {
				//detection variable, pass by reference.
				int detection;
				digitalPins[i].read_input(detection);
				if (detection == LOW) {
					//checking whether it's a new button press
					if(previousDetection[i] == HIGH){
						Serial.print("Button pressed at index ");
						Serial.println(i);
						binaryInputs[i] = true;
						canPressButton = false;
						lastButtonPress = millis();
					}
					previousDetection[i] = LOW;
				}
				else if (detection == HIGH){
					previousDetection[i] = HIGH;
				}
			}
		}
	}

	void resetButtons() {
		for (int i = 0; i < size; i++) {
			binaryInputs[i] = false;
		}
	}

	int returnPressedButton() {
		int flag = -1;
		for (int i = 0; i < size; i++) {
			if (binaryInputs[i]) {
				//one button press during one clock.
				flag = i;
			}
		}
		resetButtons();
		return flag;
	}
};

