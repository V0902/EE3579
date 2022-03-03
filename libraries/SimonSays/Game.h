
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include <time.h>
#include <stdlib.h>
#include "SSControlUnit.h"
#include "SSInputUnit.h"
#include "SSOutputUnit.h"
#include "Basic_Input.h"
#include "Buzzer.h"


//this class is literally derived from my testing sketch.
class game{
protected:
    //pins.
    int buttonPins[5];
    int outputPin;
    int difficultyPin;
    //size of the game.
    int userSize;
    //systems of the game.
    outputUnit outputControl;
    inputUnit inputControl;
    controlUnit control;
    //flags controlling the flow of the game.
    bool difficultySelected;
    bool gameFinished;
    bool countdown1;
    bool countdown2;
    bool debugging;

public:
	game(){
		bool difficultySelected = false;
		bool gameFinished = false;
		bool countdown1 = false;
		bool countdown2 = false;
	}
	void setupGame(int buttonPs[5], int outP, int difficultyP, int userSiz, bool debug = false){
		*buttonPins = buttonPs;
		outputPin = outP;
		difficultyPin = difficultyP;
		userSize = userSiz;
		outputControl.setPin(outputPin);
		inputControl.setSize(userSize);
		inputControl.setDigitalPins(buttonPs);
		inputControl.setPotentiometer(difficultyP);
        outputControl.startGameMessage();
        debugging = debug;
        control.setDebug(debugging);
	}
    void setDifficulty(){
        outputControl.playSound(inputControl.setDifficultyPin()-1);
        inputControl.readBinaryInputs();
        if(inputControl.returnPressedButton()>=0){
          Serial.println("Difficulty chosen:");
          Serial.println(inputControl.setDifficultyPin());
          control.setDifficulty(inputControl.setDifficultyPin());
          difficultySelected = true;
          delay(1000);
        };
    }
	void gameLoop(){
	  int buttonPressed;
      //Do nothing after game is finished.
      if(gameFinished){
        return;
      }
      //update clocks
      inputControl.update();
      outputControl.update(); 
      //Select difficulty.
      if(!difficultySelected){
        setDifficulty();
      }

      //difficulty set.
      else{
        //update control unit clock.
        int outcome = control.update();
    
        //Play this round?
        if(outcome == 2){
          //if about to play cues, countdown first for prep.
          if(!countdown1){
            outputControl.startCountdown(3);
            countdown1 = true;
            control.startAcceptingInputs();
          }     
          //Check whether the sounds are being played right now.
          if(control.playingCues()){
            int thisCue = control.accessCurrentCue();
            outputControl.playSound(thisCue);
            return;
          }
          //We are now in the state of accepting input.
          //Play countdown first.
          if(!countdown2){
            outputControl.startCountdown(3);
            countdown2 = true;
            control.startAcceptingInputs();
          }
          //countdown finished. Read button presses on every iteration.

          inputControl.readBinaryInputs();
          buttonPressed = inputControl.returnPressedButton();
          //Play feedback and record the sound  
          if(buttonPressed >=0){
             outputControl.playFeedbackSound(buttonPressed);
             control.getPlayerInput(buttonPressed);
          }
        }
        //Play next round?
        else if(outcome == 0){
          outputControl.printGameStatus(control.getScore());
          control.generateCueSequence(userSize);
          countdown1 = false;
          countdown2 = false;
        }
        //Game won?
        else if(outcome == 1){
          outputControl.printGameStatus(control.getScore());
          outputControl.gameWonSound();
          gameFinished = true;
        }


        //Game lost?
        else if (outcome == -1){
          outputControl.printGameStatus(control.getScore());
          outputControl.gameLostSound();
          gameFinished = true;
          }
      }
	}

};