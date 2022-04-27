#include <Arduino.h>
#include <IntervalCheckTimer.h>
#include <InterruptBasedSpeedMeasure.h>
#include <Motor.h>
#include <basic_speed_PID.h>

InterruptSpeedMeasure_SMA rotation_counter;
IntervalCheckTimer speed_check;
basic_speed_PID pid;

pidMotor motor;

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
  //rotation_counter.setupSMAarray(10);
  
  // timer to perform speed measurement and control at given interval:
  // set the time between speed measurements/control)
  int speed_control_ms=1000;  
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
    int temp_pwm = (int)pid.ComputePID_output((double)motor.getTargetRpm(), (double)RPM);
    if(RPM>=0)
    {
      Serial.print("Target pwm is "); Serial.println(motor.getTargetRpm());
      Serial.print("pwm assigned by the PID: "); Serial.print(temp_pwm);
      Serial.print("    |   revs per min (AVG)  = ");
      Serial.println(RPM);
      motor.setPower(temp_pwm);
    }
    else
    {
      Serial.println("Reading speed failed");
    } 
  }
  
}
