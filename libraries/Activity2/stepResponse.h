#ifndef STEPRESPONSE_H
#define STEPRESPONSE_H

#include<Arduino.h>

//class used for the collection of data
class stepResponse{
protected:
	//will initialize all elements to 0. Measure only for 150 seconds (at example measurement time 500ms)
	int readingsValue[300] = {};
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
	long int tempSettlingTime;

	//parameters
	int targetValue = 2000;
	int percentageBracketValue = 10;
	int stableTime = 5;
	//based on percentage
	int upperBracket;
	int lowerBracket;

	bool justCrossed;
	bool isOn = false;

public:

	stepResponse(){
		currentIndex = 0;
	}
	void turnOn(){
		if(!isOn){
			isOn = true;
			startTime = millis();
		}
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
		upperBracket = (100 + percentageBracketValue)*(double)targetValue/100;
		lowerBracket = (100 - percentageBracketValue)*(double)targetValue/100;
		Serial.println("Brackets:");
		Serial.println(upperBracket);
		Serial.println(lowerBracket);
	}

	

	void takeMeasurement(int measurement){
		//frankly speaking, it's a lot of computetions in one function, but it's probably fine.
		readingsValue[currentIndex] = measurement;
		//determining rise time
		if(!riseTime && (measurement > 9*targetValue/10)){
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
				settlingMinTime = millis() - startTime - 1000; 
			}
		}

		//determinig stable time
		if(measurement < upperBracket && measurement > lowerBracket && justCrossed){
			if(millis() - tempSettlingTime > stableTime){
				settlingTime = tempSettlingTime;
			}
		}
		//reset time upon first entry into the stable position
		else if(measurement < upperBracket && measurement > lowerBracket && !justCrossed){
			justCrossed = true;
			tempSettlingTime = millis() -startTime;
		}
		//not in the brackets - reset everything.
		else{
			tempSettlingTime = 0;
			justCrossed = false;
		}



		currentIndex +=1;
	}
	void printParameters(){
		Serial.println("-------"); 	Serial.print("Rise time is: "); Serial.println((double)riseTime/1000);
		Serial.print("Peak time is: "); Serial.println((double)peakTime/1000);
		Serial.print("Peak is: "); Serial.println(peak);
		Serial.print("Settling min time is: "); Serial.println((double)settlingMinTime/1000);
		Serial.print("Setting min point is: "); Serial.println(settlingMin);
		Serial.print("Settling time is: "); Serial.println((double)settlingTime/1000);
		Serial.println("-------");
	}
};












#endif