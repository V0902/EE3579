#include "ProximityGame.h"
#include <time.h>
#include <stdlib.h>

//all other dependencies defined inside Game.h :)

//Pin 0 - button, Pin 1 - echo, pin 2 - trigger. Check documentation for more info.
int buttonSensorPins[3] = {5,12,13};
int outputPin = 7;
int difficultyPin = A0;
int userSize = 4;
proximityGame thegame;


void setup() {
  Serial.begin(9600);
  thegame.setupGame(buttonSensorPins, outputPin, difficultyPin, userSize, true);
}

void loop() {
  thegame.gameLoop();
}
