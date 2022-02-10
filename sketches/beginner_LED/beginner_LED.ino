#include <Beginner_LED.h>


// global-scope variable
dig_LED Cpp_led;
//Note: use globals sparingly (they are easily misued causing bugs)

void setup()
{  
  // initialize as output pin (only needed once).
  int Cpp_led_pin = 13;
  Cpp_led.setup_LED(Cpp_led_pin);  

}

void loop()
{
  int waiting_ms=1000;
  Cpp_led.blink(waiting_ms);
  
}
