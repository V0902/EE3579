
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include <time.h>
#include <stdlib.h>
//?
//#include "SSControlUnit.h"

class advancedControlUnit : public controlUnit{
public:
	advancedControlUnit(){

	}
	void setDifficulty(int difficulty, float& playTimeModifier){
		if(difficulty>0 && difficulty <6){
			//that's how we make each run variable. Seed depends on when user selects difficulty.
			srand(millis());
			//still increases the sequence length.
			sequenceLength = difficulty +2;
			//hard coded speed decrease for response time. From 1.0 to 0.6 normal response.
			timeToRespond = timeToRespond - (difficulty-1)*(0.1*timeToRespond);
			//this is slightly janky, but we need to update OutputUnit, and we can't do it from here
			//we're returning a modifier that will be applied into OutputUnit. Going from 1 to 0.6
			playTimeModifier = 1 - (difficulty - 1)*0.1;
			if(debug){
				Serial.print("The playtime modifier is ");
				Serial.println(playTimeModifier);
			}
		}
		else{
			Serial.println("Invalid value of difficulty supplied.");
		}
	}

};