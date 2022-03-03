#include "AdvancedGame.h"
#include <time.h>
#include <stdlib.h>

//all other dependencies defined inside Game.h :)

int buttonPins[5] = {2,3,4,5,6};
int outputPin = 7;
int difficultyPin = A0;
int userSize = 4;
advancedGame thegame;


void setup() {
  Serial.begin(9600);
  thegame.setupGame(buttonPins, outputPin, difficultyPin, userSize, true);
}

void loop() {
  thegame.gameLoop();
}
