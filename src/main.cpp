#include <Arduino.h>

/**
 * A Legoino example to connect a Duplo Train hub and read out the color and speed
 * sensor values
 *
 * Just put your Train on the Rails and push it a littlebit forward or backward.
 * If you lift it from the track or hold it, it will stop. The color sensor readings
 * should be directly visible in the Hub LED and dependent on the color a sound
 * will be played.
 *
 * (c) Copyright 2020 - Cornelius Munz
 * Released under MIT License
 *
 */

#include <Duplo/duplo.h>
#include <Wire.h>

#include "Display/display.h"

unsigned long currentMillis = 0;

void setup() {
  Serial.begin(115200);
  display_init();
  duplo.init();
}

// main loop
void loop() {
  // connect flow
  duplo.loop();

}  // End of loop
