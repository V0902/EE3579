#include <SSControlUnit.h>
#include <SSInputUnit.h>
#include <SSOutputUnit.h>
#include <Basic_Input.h>
#include <Buzzer.h>


int buttonPins[5] = {2,3,4,5,6};
int outputPin = 7;
int difficultyPin = A0;
int userSize = 4;
outputUnit outputControl(outputPin);
inputUnit inputControl(userSize);
controlUnit control;
bool initialTest = false;
bool difficultySelected = false;
bool gameFinished = false;
bool countdown = false;


void setup() {
  Serial.begin(9600);
  //setting all the digital pins.
  inputControl.setDigitalPins(buttonPins);
  inputControl.setPotentiometer(difficultyPin);
  //now all the pins have been set.
  outputControl.startGameMessage();
}

void loop() {
  int buttonPressed;
  if(gameFinished){
    return;
  }
  //update clocks
  inputControl.update();
  outputControl.update(); 
  //Select difficulty.
  if(!difficultySelected){
    outputControl.playSound(inputControl.setDifficultyPin()-1);
    inputControl.readBinaryInputs();
    if(inputControl.returnPressedButton()>=0){
      Serial.println("Difficulty chosen.");
      control.setDifficulty(inputControl.returnPressedButton());
      difficultySelected = true;
      delay(1000);
    };
  }

  //difficulty set.
  else{
    //update control unit clock.
    int outcome = control.update();
    
    //Play this round?
    if(outcome == 2){     
      //Check whether the sounds are being played right now.
      if(control.playingCues()){
        int thisCue = control.accessCurrentCue();
        outputControl.playSound(thisCue);
        return;
      }
      //We are now in the state of accepting input.
      //Play countdown first.
      if(!countdown){
        outputControl.startCountdown(3000);
        countdown = true;
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
      countdown = false;
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
