#include <SSControlUnit.h>
#include <SSInputUnit.h>
#include <SSOutputUnit.h>
#include <Basic_Input.h>
#include <Buzzer.h>

buzzer myBuzzer;
int buzzerPin = 3;
int pitchIncrement = 100;

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  myBuzzer.setup_buzzer(buzzerPin);

}

void loop() {
  myBuzzer.switch_on();
  delay(1000);
  myBuzzer.switch_off();
  //myBuzzer.set_pitch(myBuzzer.get_pitch()+pitchIncrement);
  delay(1000);
  

}
