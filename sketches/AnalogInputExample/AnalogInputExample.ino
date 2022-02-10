#include <Basic_Input.h>
//#include <Beginner_LED.h>

// global-scope variable
in_analog an_inp;

// variable brightness led
//ana_LED variable_LED;

void setup()
{  
  // initialize as input pin (only needed once).
  int an_inp_pin = A0;
  an_inp.setup_in_analog(an_inp_pin);
  
  Serial.begin(9600);
}

void loop()
{
  int waiting_ms=1000;
  int inp_val;
  
  if( an_inp.read_input(inp_val)== true)
  {
    Serial.println();
    Serial.print(inp_val);
    Serial.println();
    delay(waiting_ms);
  }
     
}
