#include <Basic_Input.h>
#include <Beginner_LED.h>




// global-scope variable

dig_LED detect_led, check_led;
//in_digital dig_inp;
in_digital dig_inp;
//Note: use globals sparingly (they are easily misued causing bugs)

// variable brightness led
//ana_LED variable_LED;

void setup()
{  
  // initialize as output pin (only needed once).
  int detect_led_pin = 6;
  detect_led.setup_LED(detect_led_pin);
  
  int Check_led_pin = 4;
  check_led.setup_LED(Check_led_pin);
  // variable brightness led
  //int var_led_pin = 10;
  //variable_LED.setup_LED(var_led_pin);
  //
  
  int dig_inp_pin=2;
  dig_inp.setup_in_digital(dig_inp_pin, true);
  
}

void loop()
{
  int waiting_ms=1000;
  int blinking_ms=300;
  int ledON_to_inpcheck_interval_ms=250;

  int inp_val;
  
  check_led.switch_on();  
  delay(ledON_to_inpcheck_interval_ms);
  
  if( dig_inp.read_input(inp_val)== true) // if the sensor is enabled
     if(inp_val == LOW) // the pin is set to ground if the magnet is present
     {
        check_led.switch_off();
        detect_led.blink(blinking_ms);
     }
  
  check_led.switch_off();  
  delay(waiting_ms);
  
  
  
}
