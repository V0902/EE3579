#include <Arduino.h>
#include <IntervalCheckTimer.h>
#include <InterruptBasedSpeedMeasure.h>
#include <Motor.h>

InterruptSpeedMeasure rotation_counter;
IntervalCheckTimer speed_check;

Motor motor;

void setup()
{
  motor.setStartButton(8);
  motor.setStopButton(9);
  motor.setReverseButton(10);
  motor.setPotentiometerPin(A0);
  motor.setDirectionPin(4);
  motor.setPowerPin(5);
  
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
  motor.update();

  //check revs
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
