#include <Basic_Input.h>
#include <Beginner_LED.h>




// global-scope variable

dig_LED detect_led, check_led;
in_digital dig_inp1, dig_inp2;
int choice;

void setup()
{  
  //1- OR sense magnet; 2 - AND sense magnet, 3 - XOR sense magnet.
  choice = 3;
  // initialize as output pin (only needed once).
  int detect_led_pin = 6;
  detect_led.setup_LED(detect_led_pin);
 
  int Check_led_pin = 4;
  check_led.setup_LED(Check_led_pin);

 //Initialize input as hall effect sensors.
  int dig_inp_pin=2;
  dig_inp1.setup_in_digital(dig_inp_pin, true);
  int dig_inp_pin2 = 3;
  dig_inp2.setup_in_digital(dig_inp_pin2, true);
  Serial.begin(9600);
 
}

void loop()
{
  int waiting_ms=1000;
  int blinking_ms=300;
  int ledON_to_inpcheck_interval_ms=250;

  int inp_val1;
  int inp_val2;
  bool check_inp_val1;
  bool check_inp_val2;
 
  check_led.switch_on();  
  delay(ledON_to_inpcheck_interval_ms);
  check_inp_val1=dig_inp1.read_input(inp_val1);
  check_inp_val2=dig_inp2.read_input(inp_val2);
  if(inp_val1==LOW)
     Serial.println("Detection on inp1");
  else
    Serial.println("No detection on inp1");
  
  //Serial.print(dig_inp1.read_input(inp_val2));
   
    if( check_inp_val1 || check_inp_val2){ // if either sensor is enabled
      if(choice ==1){ //if we're doing an OR
         if(inp_val1 == LOW || inp_val2 == LOW) // the pin is set to ground if the magnet is present
         {
            //check_led.switch_off();
            detect_led.blink(blinking_ms);
         }
      }
      else if(choice ==2){ //doing an AND
        if(inp_val1==LOW && inp_val2 ==LOW){
            //check_led.switch_off();
            detect_led.blink(blinking_ms);
        }
      }
      else if(choice == 3){ //doing an XOR
        if((inp_val1 == LOW || inp_val2 == LOW) &&!(inp_val1 == LOW && inp_val2 == LOW)){
            //check_led.switch_off();
            detect_led.blink(blinking_ms);
        }
      }
    }
   
   
    check_led.switch_off();  
    delay(waiting_ms);
}
