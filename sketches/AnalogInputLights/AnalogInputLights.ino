#include <Basic_Input.h>
#include <Beginner_LED.h>

// global-scope variable
in_analog an_inp;
//array of LED lights
dig_LED arrayOfLed[5];

//Put this function in a separate .h file

//void groupSwitchOn(indices, dig_LED[5] myLeds){
//  for(i = 0; i<5; i++){
//    if(i<=indices){
//      myLeds[i].switch_on();
//    }
//    else{
//      myLeds[i].switch_off();
//    }
//  }
//}

void setup()
{  
  // initialize as input pin (only needed once).
  int an_inp_pin = A0;
  an_inp.setup_in_analog(an_inp_pin);
  // initialize array of LEDs
  int index = 0;
  for(int i = 9; i < 14; i++){
    arrayOfLed[index].setup_LED(i); 
    index +=1;
  }
  Serial.begin(9600);
}



void loop()
{
  int waiting_ms=200;
  int inp_val;
  
  if( an_inp.read_input(inp_val)== true)
  {
    Serial.println();
    Serial.print(inp_val);
    Serial.println();
    if(inp_val < 171){
      arrayOfLed[0].switch_off();
      arrayOfLed[1].switch_off();
      arrayOfLed[2].switch_off();
      arrayOfLed[3].switch_off();
      arrayOfLed[4].switch_off();
                  
    }
    else if(inp_val < 341){
      arrayOfLed[0].switch_on();
      arrayOfLed[1].switch_off();
      arrayOfLed[2].switch_off();
      arrayOfLed[3].switch_off();
      arrayOfLed[4].switch_off();
                  
    }
    else if(inp_val < 512){
      arrayOfLed[0].switch_on();
      arrayOfLed[1].switch_on();
      arrayOfLed[2].switch_off();
      arrayOfLed[3].switch_off();
      arrayOfLed[4].switch_off();
                  
    }
    else if(inp_val < 683){
      arrayOfLed[0].switch_on();
      arrayOfLed[1].switch_on();
      arrayOfLed[2].switch_on();
      arrayOfLed[3].switch_off();
      arrayOfLed[4].switch_off();
                  
    }
    else if(inp_val < 853){
      arrayOfLed[0].switch_on();
      arrayOfLed[1].switch_on();
      arrayOfLed[2].switch_on();
      arrayOfLed[3].switch_on();
      arrayOfLed[4].switch_off();
                  
    }
    else{
      arrayOfLed[0].switch_on();
      arrayOfLed[1].switch_on();
      arrayOfLed[2].switch_on();
      arrayOfLed[3].switch_on();
      arrayOfLed[4].switch_on();
                  
    }
    delay(waiting_ms);
  }
     
}
