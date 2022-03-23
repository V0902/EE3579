#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

//?
//#include <SSInputUnit.h>
#include <UltrasoundSensor.h>

class proximityInput: public inputUnit {
protected:
	PingProximitySensor sensor;
	int distance;
	int lastDistance;

public:
	//default timeout is 9ms. This is decent for this implementation.
	proximityInput(){
		distance = 0;
	}
	//Take the measurement when called internally. Whether it was successful or not will determine the behavior of the program.
	bool takeMeasurement(){
		lastDistance = distance;
		if(sensor.SenseDistance(distance)){
			lastDistance = distance;
			return true;
		}
		//in case a measurement is failed, assume it's the furthest distance down.
		//this is stupid but for sizes of game like 5 the sensor a lot of measurements fail
		//it will be reduced later depending on game size either way.
		distance = 50;		return false;

	}
//overrride from inputUnit(). Pin 0 - for the button, pin 1 for echo, pin 2 for trigger
//Only one button will activate the measurement.
	void setDigitalPins(int pins[3]) {
		//initializaiton of the in_digital array.
		for(int i = 0; i < 3; i++){
			if(pins[i] < 0 || pins[i] > 13){
				Serial.println("Fatal error - bad pin number for Arduino Uno.");
				return;
			}
		}
		//just setting up the button
		digitalPins[0].setup_in_digital(pins[0], true);
		//setting up the sensor
		sensor.setup_PingProximitySensor(pins[1], pins[2]);		
	}
	//Override from inputUnit(). Check whether pin 0 was pressed, if yes - take measurement.
	//we're reusing the old previousDetection array, because there's no need to introduce new variables.
	void readBinaryInputs() {
		if(canPressButton){
			//detection variable, pass by reference.
			int detection;
			digitalPins[0].read_input(detection);
			if (detection == LOW) {
				if(previousDetection[0] == LOW){
					distance = -1;
				}
				//checking whether it's a new button press
				else if(previousDetection[0] == HIGH){
					//This only matters on the first execution of this block.
					distance = lastDistance;
					Serial.print("Button pressed. ");
					takeMeasurement();
					canPressButton = false;
					lastButtonPress = millis();
				}
				previousDetection[0] = LOW;
			}
			else if (detection == HIGH){
				previousDetection[0] = HIGH;
				//ensuring returnPressedButton returns -1 if there has been no detection
				distance = -1;
			}		
		}
	}
	//overloading this function to get user input from distance. 
	//Name is not very fitting but I am avoiding having to do any modifications in the main game loop.
	//0 = <12; 1 = <22, 2 = <32, 3 = <41, 4 = anything else
	int returnPressedButton() {
		int userInput = (distance < 0) ? -1 :((distance < 12) ? 0 : ((distance < 22) ? 1 : ((distance<32) ? 2: ((distance <41) ? 3 : 4))));
		distance = -1;
		//out of range limits.
		if (userInput > size){
			userInput = size - 1;
		}
		if(userInput != -1){
			Serial.println(userInput);
		}
		return userInput;
	}
};