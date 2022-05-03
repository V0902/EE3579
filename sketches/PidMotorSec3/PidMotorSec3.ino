#include <Arduino.h>
#include <IntervalCheckTimer.h>
#include <InterruptBasedSpeedMeasure.h>
#include <Motor.h>
#include <basic_speed_PID.h>
#include <stepResponse.h>

InterruptSpeedMeasure_SMA rotation_counter;
IntervalCheckTimer speed_check;
basic_speed_PID pid;
stepResponse measurements;
pidMotor motor;

int temp_pwm;
double RPM;
int targetValue = 3000;
bool mode;

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
  rotation_counter.setupSMAarray(5);
  
  // timer to perform speed measurement and control at given interval:
  // set the time between speed measurements/control)
  int speed_control_ms=500;  
  int pid_check_ms = 500;
  
  speed_check.setInterCheck(speed_control_ms);  


  //setting the PID variables.
  //kp - proportional 0.3525 unstable, f = 2Hz
  //ki - integral
  //kd - derivative
  double kp = 0.05;
  double ki = 0.0015;
  double kd = 0.006;
  pid.set_gainvals(kp, ki, kd);

  //setup parameters for the 
  int percentageBrackets = 10;
  int stableTimeMs = 2000;
  Serial.begin(9600); 
  measurements.setParameters(targetValue, percentageBrackets, stableTimeMs);
  
  
  
    
   

} 

void loop()
{
  motor.update();

  //check revs
  if(speed_check.isMinChekTimeElapsedAndUpdate() && motor.on())
  {
    measurements.turnOn();

    RPM=rotation_counter.getRPMandUpdate();

    //graphing mode or not.
    mode = 1;

    if(RPM>=0)
    {

      if(mode == 1){
        measurements.takeMeasurement((int)RPM);
        Serial.print("Target RPM is "); Serial.println(targetValue);
        Serial.print("pwm assigned by the PID: "); Serial.print(temp_pwm);
        Serial.print("    |   revs per min (AVG)  = ");
        Serial.println(RPM);
      }
      else{
        Serial.print(targetValue);Serial.print("\t");
        Serial.println(RPM);
      }
      motor.setPower(temp_pwm);
      temp_pwm = (int)pid.ComputePID_output(targetValue, (double)RPM);

    }
    else
    {
      Serial.println("Reading speed failed");
    } 
  }        

  //Show measurements if user inputs a 1.
  if(Serial.available() == 1){
    if (Serial.parseInt() == 1){
      measurements.printParameters();
    }
  }

}
