#include <IntervalCheckTimer.h>
#include <ProperLED.h>
#include <Basic_Input.h>

an_LED fan_control;
in_digital stopButton;
in_digital startButton;
IntervalCheckTimer timer;

bool working;

int userPressedStop, userPressedStart;
int lastFanValue;
int min_val, max_val;
int jumpstart_val, jumpstop_val;
int potentiometerPin;
int potentiometerRead;


void setup() {
  working = false;
  // put your setup code here, to run once:
  potentiometerPin = A0;
  int stopPin = 3;
  int startPin = 4;
  int pwmpin_fancontrol = 6;
  unsigned long inp_inter_check_ms = 250;
  jumpstart_val = 94;
  jumpstop_val = 0;
  //
  min_val = 58;
  max_val = 200;
  //
  timer.setInterCheck(inp_inter_check_ms);
  fan_control.setup_LED(pwmpin_fancontrol);
  //set internal pullup
  startButton.setup_in_digital(startPin, true);
  stopButton.setup_in_digital(stopPin, true);
  Serial.begin(9600);
  

}

void loop() {
  //Check every x ms for input
  if(timer.isMinChekTimeElapsedAndUpdate())
  {
    //read input from buttons
    stopButton.read_input(userPressedStop);
    startButton.read_input(userPressedStart);

    potentiometerRead = analogRead(potentiometerPin);
    
    //user pressed button to turn it off.
    if(userPressedStop == LOW && working){
      lastFanValue = fan_control.get_brightness();
      fan_control.set_brightness(jumpstop_val);
      working = false;
      Serial.println("Fan stopped");
      fan_control.switch_off();
    }
    //user pressed button to turn it on.
    else if(userPressedStart == LOW && !working){
      fan_control.switch_on();
      if(lastFanValue < jumpstart_val){
        lastFanValue = jumpstart_val;
      }
      fan_control.set_brightness(lastFanValue);
      working = true;
      Serial.println("Fan started");
    }
    
    //user might or might not have span the potentiometer - we don't care.
    else if(working){
       potentiometerRead = analogRead(potentiometerPin);
       //determining the scaling factor for the potentiometer (that has 1024 distinct values)
       int scalingFactor = 1024/(max_val-min_val); 
       //setting the brightness between min_val and max_val
       lastFanValue = potentiometerRead/scalingFactor+min_val;
       fan_control.set_brightness(lastFanValue);

    }
      Serial.print("Current pwm ");
      Serial.println(fan_control.get_brightness());
  }
 
}
