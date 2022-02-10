#include <Beginner_LED.h>

ana_LED variable_LED;
void setup() {
  // put your setup code here, to run once:
  int pwm_pin=11;
  variable_LED.setup_LED(pwm_pin);
  
}

void loop() {
  
  ////
  // variable brightness led
  int tot_steps=150;
  int waiting_ms=1000;
  
  variable_LED.switch_on();
  variable_LED.set_max_bright();
  
  for(int i=0; i<tot_steps; i++)
  {
      variable_LED.dimmer();
      delay(waiting_ms/20);
  }
  Serial.print("Reached minimum dimness");
  delay(waiting_ms);
  //*
  for(int i=0; i<tot_steps; i++)
  {
      variable_LED.brighter();
      delay(waiting_ms/20);
  }  
  //*/

}
