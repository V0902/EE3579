#include <IntervalCheckTimer.h>
#include <ProperLED.h>

an_LED fan_control;
IntervalCheckTimer timer;
bool increasing;
int increments_to_print, curr_increm;
int min_val, max_val;

void setup() {
  // put your setup code here, to run once:
  int pwmpin_fancontrol = 6;
  unsigned long inp_inter_check_ms = 750;
  unsigned long jumpstart_ms = 500;
  int jumpstart_val = 94;
  //
  min_val = 54;
  max_val = 200;
  //
  fan_control.setup_LED(pwmpin_fancontrol);
  fan_control.switch_on();
  // jumpstart 
  fan_control.set_brightness(jumpstart_val);
  delay(jumpstart_ms);
  //
  fan_control.set_brightness(min_val);
  //
  timer.setInterCheck(inp_inter_check_ms);
  //
  increasing = true;
  increments_to_print = 2;
  curr_increm = 0;
  Serial.begin(9600);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if(timer.isMinChekTimeElapsedAndUpdate())
  {
    if(increasing)
      fan_control.brighter();
    else
      fan_control.dimmer();

    curr_increm++;

    if(curr_increm >= increments_to_print)
    {
      Serial.print("Current pwm ");
      Serial.println(fan_control.get_brightness());
      curr_increm = 0;
    }
    
    if(fan_control.get_brightness()>max_val)
      increasing=false;
    if(fan_control.get_brightness()<min_val)
      increasing=true;
    
  }
 
}
