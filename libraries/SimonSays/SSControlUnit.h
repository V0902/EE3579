#ifndef CONTROL_UNIT
#define CONTROL_UNIT

#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include <time.h>
#include stdlib.h>

class controlUnit {
protected:
	int noOfCues;
	int currentGame;			//will go up to 7 games
	int sequenceLength;
	int timeToRespond;
	int gamesWon;
	//7, because maximum of 7 ques.
	int playerResponses[7];
	int cueToPlay[7];
	int previousResponseTime;	//used to record how much time player took
	int playerResponse;			//How many time has the player responded?
	int currentCue;

public:
	controlUnit(){
		reset();
	}
	void reset(){
		noOfCues = 0;
		currentGame = 0;
		currentCue = 0;
		gamesWon = 0;
		matchWon = false;
		matchLost = false;
		setUnitResponse(2000);
	}
	//potentiometer ranges are handled inside InputUnit. difficulty ranges between 1 - 5.
	void setDifficulty(int difficulty){
		sequenceLength = difficulty +2;
	}
	void setUnitResponse(respTime){
		timeToRespond = respTime;
	}
	void getPlayerInput(){

	}
	//pseudo randomly generate cues for M frequencies.
	void generateCueSequence(int M){
	if(sequenceLength > 0){
		for(int i = 0; i < sequenceLength; i++){
			cueToPlay[i] = rand() % M;
			
		}
	}
	int accessCue(int index){
		return cueToPlay[index];
	}

	//game outcome trigger. Checked after player has given all their cues.
	void updateScore(){
		if(compareArrays()){
				gamesWon+=1;
				playerResponse = 0;
		}
		currentGame+=1;
	}
	int checkGameStatus(){
	//function called to check what state the game is in.
		if(playerResponse == sequenceLength){
			updateScore();
		}
		//check if any player has won when reaching game 6
		if(currentGame == 5 and gamesWon >3){
			return 1;
		}
		else if(currentGame == 5 && gamesWon <3){
			return -1;
		}
		else{
			//move on to the next game.
			return 0;
		}


	}

private:
	bool compareArrays(){
		for(int i = 0; i < sequenceLength; i++){
			if(playerResponses[i] != cueToPlay[i]){
				return false;
			}
		}
		return true;
	}

	



};






#endif