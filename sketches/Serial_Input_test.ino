#include <Basic_Input.h>

in_serial serialinp;

void setup()
{  
  Serial.begin(9600);
}

void loop()
{
  int val;
  bool success=serialinp.read_input(val);
  if(success)
    Serial.println(val);
  
}


