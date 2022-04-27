#ifndef STEPRESPONSE_H
#define STEPRESPONSE_H

#include<Arduino.h>

//class used for the collection of data
class stepResponse{
protected:
	//will initialize all elements to 0.
	int readingsValue[1000] = {};
	//
	int currentIndex;
	//parameters we're measuring
	long int riseTime; //
	long int peakTime; //
	int peak; //
	int settlingMin;
	long int settlingMinTime;
	int settlingTime;

	//timers
	long int startTime;
	long int measureSettlingTime;

	//parameters
	int targetValue = 2000;
	int percentageBracketValue = 10;
	int stableTime = 5;
	//based on percentage
	int upperBracket;
	int lowerBracket;

	bool justCrossed;

public:

	stepResponse(){
		currentIndex = 0;
		startTime = millis();
	}

	void setParameters(int tV, int pbv, int sT){
		if(tV < 0 || pbv < 0 || pbv > 100 || sT < 0){
			Serial.println("Incorrect values supplied");
			return;
		}
		targetValue = tV;
		percentageBracketValue = pbv;
		stableTime = sT;

		//setting the brackets
		upperBracket = (100 + percentageBracketValue)*targetValue/100;
		lowerBracket = (100 - percentageBracketValue)*targetValue/100;
	}

	void takeMeasurement(int measurement){
		//frankly speaking, it's a lot of computetions in one function, but it's probably fine.
		readingsValue[currentIndex] = measurement;
		//determining rise time
		if(!riseTime && measurement > 9*targetValue/10){
			riseTime = millis() - startTime; 
		}
		//determining peak
		if(measurement > peak){
			peak = measurement;
			peakTime = millis() - startTime;
		}
		//detemining settlingMin.
		if(!settlingMin && currentIndex > 5){
			int tempPeak = readingsValue[currentIndex - 2];
			if(readingsValue[currentIndex -4] > tempPeak && readingsValue[currentIndex-3] > tempPeak 
			&& readingsValue[currentIndex-1] > tempPeak && measurement > tempPeak){
				settlingMin = tempPeak;
				//todo: add interval timer between measurements
				settlingMinTime = millis() - startTime; //-2*interval 
			}
		}

		if(measurement < upperBracket && measurement > lowerBracket && justCrossed){
			//nothing
		}
		//reset time upon first entry into the stable position
		else if(measurement < upperBracket && measurement > lowerBracket && !justCrossed){
			justCrossed = true;
			settlingTime = millis() -startTime;
		}
		//not in the brackets - reset everything.
		else{
			settlingTime = 0;
			justCrossed = false;
		}



		currentIndex +=1;
	}
}












#endif