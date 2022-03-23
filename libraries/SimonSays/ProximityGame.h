
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include <Game.h>
#include "SSAdvancedControl.h"
#include "ProximityInput.h"

class proximityGame: public game{
protected:
    advancedControlUnit advcontrol;
    proximityInput pinputControl;
    //keeping the advanced control unit and swapping input unit to the proximity one.
    //I've tried some pointer manipulation to no avail. Therefore I have reverted the changes.
public:
    //overloading the setup to include new component.
    void setupGame(int buttonPs[3], int outP, int difficultyP, int userSiz, bool debug = false){
		*buttonPins = buttonPs;
		outputPin = outP;
		difficultyPin = difficultyP;
		userSize = userSiz;
		outputControl.setPin(outputPin);
		pinputControl.setSize(userSize);
        //overloaded with a smaller array.
		pinputControl.setDigitalPins(buttonPs);
		pinputControl.setPotentiometer(difficultyP);
        outputControl.startGameMessage();
        debugging = debug;
        control.setDebug(debugging);
	}
    //no change here besides swapping inputContro lto pinputControl and increasing response time.
    void setDifficulty(){
        outputControl.playSound(pinputControl.setDifficultyPin()-1);
        pinputControl.readBinaryInputs();
        if(pinputControl.returnPressedButton()>=0){
            advcontrol.setDebug(debugging);
            Serial.println("Difficulty chosen:");
            Serial.println(pinputControl.setDifficultyPin());
            //changes from inheritance. modifier passed by reference
            float modifier;
            advcontrol.setDifficulty(pinputControl.setDifficultyPin(), modifier);
            outputControl.setAdvanced(true);
            //user is given 4 times as much time to respond due to the nature of the game not being just pressing buttons anymore.
            outputControl.setRespTime(modifier*4);
            difficultySelected = true;
            delay(1000);
        };
    }
   //Only overloading inputUnit with proximityInput. 
   void gameLoop(){
	  int buttonPressed;
      //Do nothing after game is finished. There is absolutely no difference here whatsoever compared to the base class :( inheritance help
      if(gameFinished){
        return;
      }
      //update clocks
      pinputControl.update();
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

          pinputControl.readBinaryInputs();
          buttonPressed = pinputControl.returnPressedButton();
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