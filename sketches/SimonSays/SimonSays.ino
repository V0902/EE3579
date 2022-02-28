//#include <SSControlUnit.h>
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
bool initialTest = false;
bool difficultySelected = false;


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
  /*
  //testing whether the pins can make a sound.
  if(!initialTest){
    for(int i =0; i < userSize; i++){
       outputControl.playSound(i);
       delay(1000);
    }
    initialTest = !initialTest;
    Serial.println("Audio tests finished");
  }
  */
  //update clocks
  inputControl.update();
  outputControl.update();
  //check if any button has been pressed
  if(!difficultySelected){
    outputControl.playSound(inputControl.setDifficultyPin()-1);
    inputControl.readBinaryInputs();
    if(inputControl.returnPressedButton()>=0){
      Serial.println("Difficulty chosen.");
      difficultySelected = true;
      delay(1000);
    };
  }

  else{
    inputControl.readBinaryInputs();
    //assign it a variable
    buttonPressed = inputControl.returnPressedButton();
    //play sound based on which button was pressed.
    
    if(buttonPressed >=0){
       outputControl.playFeedbackSound(buttonPressed);

    }
  }

  
}



/*
buzzer myBuzzer;
int buzzerPin = 3;
int pitchIncrement = 100;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  myBuzzer.setup_buzzer(buzzerPin);

}

void loop() {
  myBuzzer.switch_on();
  delay(1000);
  myBuzzer.switch_off();
  //myBuzzer.set_pitch(myBuzzer.get_pitch()+pitchIncrement);
  delay(1000);
  

}
*/
