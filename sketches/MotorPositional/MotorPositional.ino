#include <Arduino.h>
#include <IntervalCheckTimer.h>
#include <InterruptBasedSpeedMeasure.h>
#include <Motor.h>
#include <basic_speed_PID.h>

InterruptSpeedMeasure_SMA rotation_counter;
IntervalCheckTimer speed_check;
IntervalCheckTimer pid_check;
basic_speed_PID pid;

PositionalMotor motor;

int temp_pwm;
double RPM;

void setup()
{
  motor.setStartButton(8);
  motor.setStopButton(9);
  motor.setReverseButton(10);
  motor.setPotentiometerPin(A0);
  motor.setDirectionPin(4);
  motor.setPowerPin(5);  

  int rotations = 5961;
  //based on the test results. See report
  motor.inputRotations(264);
  motor.inputTargetRotations(5961);
  Serial.begin(9600);
  Serial.setTimeout(10); 
  Serial.print("The total rotations we're looking to achieve are: "); Serial.println(rotations); 

} 

void loop()
{
  bool test = 0;
  //script for finding the falloff time at pwm = 255.
  if(test){
    //type 1 to start test   
    motor.test();   
  }
  //normal operation
  else{
    motor.update();
  }

  
  
}
