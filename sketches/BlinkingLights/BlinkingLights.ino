#include <Beginner_LED.h>


// global-scope variable
dig_LED arrayOfLed[5];
//Note: use globals sparingly (they are easily misued causing bugs)

void setup()
{  
  // initialize as output pin (only needed once).
  int index = 4;
  for(int i = 9; i < 14; i++){
  arrayOfLed[index].setup_LED(i); 
  index -=1;
  }

}

void loop()
{
  int w = 1000;
  arrayOfLed[4].switch_off();
  arrayOfLed[0].switch_on();
  arrayOfLed[1].switch_on();
  delay(w);
  arrayOfLed[0].switch_off();
  arrayOfLed[2].switch_on();
  delay(w);
  arrayOfLed[1].switch_off();
  arrayOfLed[3].switch_on();
  delay(w);
  arrayOfLed[2].switch_off();
  arrayOfLed[4].switch_on();
  delay(w);
  arrayOfLed[3].switch_off();
  arrayOfLed[0].switch_on();
  delay(w);
  
}
