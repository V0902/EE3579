
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include <Game.h>
#include "SSAdvancedControl.h"

class advancedGame: public game{
protected:
    advancedControlUnit advcontrol;
    //the only thing that is different is the control unit, which also influences changes in the outputUnit.

    //I have no idea how to use inheritance here, I give up 
public:
    void setDifficulty(){
        outputControl.playSound(inputControl.setDifficultyPin()-1);
        inputControl.readBinaryInputs();
        if(inputControl.returnPressedButton()>=0){
            advcontrol.setDebug(debugging);
            Serial.println("Difficulty chosen:");
            Serial.println(inputControl.setDifficultyPin());
            //changes from inheritance. modifier passed by reference
            float modifier;
            advcontrol.setDifficulty(inputControl.setDifficultyPin(), modifier);
            outputControl.setAdvanced(true);
            outputControl.setRespTime(modifier);
            difficultySelected = true;
            delay(1000);
        };
    }
    	void gameLoop(){
	  int buttonPressed;
      //Do nothing after game is finished. There is absolutely no difference here whatsoever compared to the base class :( inheritance help
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
        int outcome = advcontrol.update();
    
        //Play this round?
        if(outcome == 2){
          //if about to play cues, countdown first for prep.
          if(!countdown1){
            outputControl.startCountdown(3);
            countdown1 = true;
            advcontrol.startAcceptingInputs();
          }     
          //Check whether the sounds are being played right now.
          if(advcontrol.playingCues()){
            int thisCue = advcontrol.accessCurrentCue();
            outputControl.playSound(thisCue);
            return;
          }
          //We are now in the state of accepting input.
          //Play countdown first.
          if(!countdown2){
            outputControl.startCountdown(3);
            countdown2 = true;
            advcontrol.startAcceptingInputs();
          }
          //countdown finished. Read button presses on every iteration.

          inputControl.readBinaryInputs();
          buttonPressed = inputControl.returnPressedButton();
          //Play feedback and record the sound  
          if(buttonPressed >=0){
             outputControl.playFeedbackSound(buttonPressed);
             advcontrol.getPlayerInput(buttonPressed);
          }
        }
        //Play next round?
        else if(outcome == 0){
          outputControl.printGameStatus(advcontrol.getScore());
          advcontrol.generateCueSequence(userSize);
          countdown1 = false;
          countdown2 = false;
        }
        //Game won?
        else if(outcome == 1){
          outputControl.printGameStatus(advcontrol.getScore());
          outputControl.gameWonSound();
          gameFinished = true;
        }


        //Game lost?
        else if (outcome == -1){
          outputControl.printGameStatus(advcontrol.getScore());
          outputControl.gameLostSound();
          gameFinished = true;
          }
      }
	}

};