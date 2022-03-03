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
	bool feedbackSoundPlaying;
	unsigned long soundPlayStart;
	int playTime;

	//I found using inheritance redundant and easier to keep it this way.
	bool advanced;

public:
	outputUnit() {
		reset();
	}
	outputUnit(int pinNo) {
		reset();
		setPin(pinNo);
	}
	void setPin(int pinNo){
		buzzerPin = pinNo;
		myBuzzer.setup_buzzer(buzzerPin);
	}
	void reset(){
		higherFreq = 0;
		playTime = 1000;
		advanced = false;
	}
	void playSound(int index) {
		myBuzzer.set_pitch(frequencies[index]);
		myBuzzer.switch_on();
		//this is bad and does not provide functionality for multiple Simon Says games in parallel.
		//Regardless, it is a very convenient way of dealing with timing so that the programme is hanging
		//when playing sounds.
		delay(playTime);
		myBuzzer.switch_off();
	}
	//used to play the users' button presses
	void playFeedbackSound(int index) {
		if(feedbackSoundPlaying){
			stopPlayingFeedbackSound();
		}
		myBuzzer.set_pitch(frequencies[index] + higherFreq);
		myBuzzer.switch_on();
		feedbackSoundPlaying = true;
		soundPlayStart = millis();
	}
	void stopPlayingFeedbackSound(){
		myBuzzer.switch_off();
		feedbackSoundPlaying = false;
	}
	//low-level time managment for feedback sounds playing.
	void update(){
		if((millis() - soundPlayStart >= playTime) && feedbackSoundPlaying){
			stopPlayingFeedbackSound();

		}
	}
	//functions that will be called through the control unit, purely user output.
	void gameLostSound(){
		//hard coded to save memory.
		Serial.println("Game lost.");
		myBuzzer.set_pitch(100);
		myBuzzer.switch_on();
		delay(3000);
		myBuzzer.switch_off();
	}
	void gameWonSound(){
		//Might make this sound good.

		Serial.println("Game won.");
		myBuzzer.set_pitch(1500);
		myBuzzer.switch_on();
		delay(3000);
		myBuzzer.switch_off();
	}

	void startGameMessage() {
		Serial.println("Welcome to the Simon Says game! A match spans 6 games + an additional tie-breaker, if necessary.");
		Serial.println("Please choose difficulty first by spinning the potentiometer. Higher pitch = higher difficulty.");
		Serial.println("Once you have selected your difficulty, please press any button to confirm it.");
	}
	void printGameStatus(int currentScore) {
		Serial.print("Current score is ");
		Serial.println(currentScore);
	}
	//no time managment needed as user is not to input any data at this moment.
	void startCountdown(int seconds) {
		Serial.println("Starting countdown...");
		delay(1000);
		myBuzzer.set_pitch(700);
		for(int i = seconds; i >= 0; i--){
			Serial.println(i);
			myBuzzer.switch_on();
			delay(200);
			myBuzzer.switch_off();
			delay(800);
		}
	}
	//advanced functions.
	void setAdvanced(bool a){advanced = a; higherFreq= a ?  30:0;}
	void setRespTime(float modifier){playTime = playTime*modifier;}


};