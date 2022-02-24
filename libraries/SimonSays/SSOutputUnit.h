#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif
#include <Buzzer.h>

class outputUnit {
protected:
	buzzer myBuzzer;
	//C4, E4, G4, B4, D5. Why not.
	static const int MAX_SIZE = 5;
	int frequencies[MAX_SIZE] = { 261, 329, 392, 493, 587 };
	int buzzerPin;
	//feedback for playing a higher-pitched sound.
	int higherFreq;

public:
	outputUnit() {
		higherFreq = 30;
	}
	outputUnit(int pinNo) {
		buzzerPin = pinNo;
		myBuzzer.setup_buzzer(buzzerPin);
	}
	void playSound(int index) {
		myBuzzer.set_pitch(frequencies[index]);
		myBuzzer.switch_on();
		delay(1000);
		myBuzzer.switch_off();
	}
	//used to play the users' button presses
	void playFeedbackSound(int index) {
		myBuzzer.set_pitch(frequencies[index] + higherFreq);
		myBuzzer.switch_on();
		delay(1000);
		myBuzzer.switch_off();
	}
	//functions that will be called through the control unit, purely user output.
	void gameLostSound(){
		//hard coded to save memory.
		myBuzzer.set_pitch(100);
		myBuzzer.switch_on();
		delay(3000);
		myBuzzer.switch_off();
	}
		void gameWonSound(){
		//Might make this a funky tune or sth.
		myBuzzer.set_pitch(1500);
		myBuzzer.switch_on();
		delay(3000);
		myBuzzer.switch_off();
	}

	void startGameMessage() {
		Serial.println("Welcome to the Simon Says game! A match spans 6 games + an additional tie-breaker, if necessary.");
	}
	void printGameStatus(int currentScore) {
		Serial.print("Current score is");
		Serial.println(currentScore);
	}
	void startCountdown(int seconds){
	myBuzzer.set_pitch(700);
		for(int i = seconds; i > 0; i--){
			Serial.println(i);
			myBuzzer.switch_on();
			delay(200);
			myBuzzer.switch_off();
			delay(800);
		}

	}

};