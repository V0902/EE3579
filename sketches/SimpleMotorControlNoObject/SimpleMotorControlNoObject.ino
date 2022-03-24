#include <Arduino.h>
#include <IntervalCheckTimer.h>
#include <InterruptBasedSpeedMeasure.h>


InterruptSpeedMeasure rotation_counter;
IntervalCheckTimer speed_check;

//FV
int target_index, consecutive_count;
//
bool switched;

void setup()
{
  //set enable pin
  pinMode(13, OUTPUT);
  //set direction pin!
  pinMode(4, OUTPUT);
  digitalWrite(13, LOW);
  // enable the interrupt (int_0 works via pin2)
  // enable the interrupt (int_1 works via pin3)
  rotation_counter.setupSpeedMeasure(int_1);
  
  // timer to perform speed measurement and control at given interval:
  // set the time between speed measurements/control)
  int speed_control_ms=500;  
  speed_check.setInterCheck(speed_control_ms);
  
    
  Serial.begin(9600);  
  
  
  
}

void loop()
{
    Serial.println("Turned off");
    digitalWrite(13, LOW);
    delay(5000);
    digitalWrite(13, HIGH);
    Serial.println("Turned on");
    Serial.println("Oop, I'm switching direction now!");
    digitalWrite(4, switched);
    switched = !switched;
    delay(5000);
    digitalWrite(13, LOW);
    Serial.println("Turned off again");
    
  
  
  if(speed_check.isMinChekTimeElapsedAndUpdate())
  {
    double RPM=rotation_counter.getRPMandUpdate();
    if(RPM>=0)
    {
      Serial.print("revs per min  = ");
      Serial.println(RPM);
    }
    else
    {
      Serial.println("Reading speed failed");
    } 
  }
  
}
