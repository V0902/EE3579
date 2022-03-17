
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include <time.h>
#include <stdlib.h>

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
	int currentCue;				//what cue is being played right now?
	int weirdFlag;				//this is a bad practice but I couldnt figure out how to fix something differently.

	unsigned long lastResponseTime;

	bool playCues;
	bool sequenceGenerated;
	bool acceptInput;			//check if control system accepts input.
	bool matchWon;
	bool matchLost;

	bool debug;
	bool compareArrays(){
		for(int i = 0; i < sequenceLength; i++){
			if(playerResponses[i] != cueToPlay[i]){
				return false;
			}
		}
		return true;
	}

public:
	controlUnit(){
		reset();
	}

	//potentiometer ranges are handled inside InputUnit. difficulty ranges between 1 - 5.
	void setDifficulty(int difficulty){
		if(difficulty>0 && difficulty <6){
			sequenceLength = difficulty +2;
		}
		else{
			Serial.println("Invalid value of difficulty supplied.");
		}
	}
	//set how long the user gets to resond.
	void setUnitResponse(int respTime){
		timeToRespond = respTime;
	}
	void getPlayerInput(int response){
		if(acceptInput){
			playerResponses[playerResponse] = response;
			playerResponse+=1;
			lastResponseTime = millis();
		}
	}
	void reset(){
		noOfCues = 0;
		currentGame = 0;
		currentCue = 0;
		gamesWon = 0;
		weirdFlag = 0;
		matchWon = false;
		matchLost = false;
		sequenceGenerated = false;
		playCues = false;
		acceptInput = false;
		setUnitResponse(2000);
	}

	//pseudo randomly generate cues for M frequencies. Also resets the round.
	void generateCueSequence(int M){
		currentCue = 0;
		playerResponse = 0;
		if(sequenceLength > 0){
			for(int i = 0; i < sequenceLength; i++){
				cueToPlay[i] = rand() % M;
				}
			sequenceGenerated = true;
		}
		if(debug){
			Serial.println("Sequence generated.");
		}
	}
	int accessCurrentCue(){
	//after accessing cue, increment
		if(debug){
			Serial.print("Current cue being played is: ");
			Serial.println(cueToPlay[currentCue]);
		}
		return cueToPlay[currentCue++];
	}

	//game outcome trigger. Checked after player has given all their cues.
	void updateScore(){
		if(compareArrays() && playerResponse == sequenceLength){
				gamesWon+=1;
				playerResponse = 0;
		}
		currentGame+=1;
	}
	int checkGameStatus(){
	//function called to check what state the game is in.
		updateScore();
		//check if any player has won when reaching game 6
		if(currentGame >= 6 and gamesWon >3){
			return 1;
		}
		else if(currentGame >= 6 && gamesWon <3){
			return -1;
		}
		else{
			//move on to the next game.
			return 0;
		}
	}
	//update the state of game. 1 - player won, -1 = player lost, 0 - move on to next round, 2 - stay in same round.
	int update(){
		//if sequence is not generated, treat it as if we were moving on to the next round.
		//This is for the start of the game mainly.
		if(!sequenceGenerated){
			return 0;
		}
		//After sequence is generated & cues have been played, start accepting inputs.
		else if (playCues == false){
			acceptInput = true;
			weirdFlag +=1;	//making sure the next checkGameStatus() will not be made same iteration as the first call of this.

		}
		//Check if we're playing the cues.
		if(currentCue < sequenceLength){
			playCues = true;
		}
		// This variable keeps track whether we're still playing cues.
		else{
			playCues = false;
		}

		//check whether player failed to respond in time OR answered on time. Check responses.
		if(weirdFlag > 1){
			if(acceptInput && ((millis() - lastResponseTime >= timeToRespond)||(playerResponse >= sequenceLength))){
				acceptInput = false;

				weirdFlag = 0;
				return checkGameStatus();
			}
		}


		//no special events happened. Carry on within the same round.
		return 2;

	}
	bool readyForGeneration(){return !sequenceGenerated;}
	bool playingCues(){return playCues;}
	void startAcceptingInputs(){lastResponseTime = millis();}
	void setDebug(bool b){debug = b;}
	int getScore(){return gamesWon;}




};

