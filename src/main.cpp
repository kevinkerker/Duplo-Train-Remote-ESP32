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

void duplo_speed(void *parameter) {
  Serial.print("Init duplo_speed\n");
  while (1) {
    duplo.loop();
    delay(100);
  }
}
void duplo_buttons(void *parameter) {
  Serial.print("Init duplo_color\n");
  while (1) {
    // duplo.buttons();
    delay(2000);
  }
}
void duplo_display(void *parameter) {
  Serial.print("Init duplo_display\n");
  while (1) {
    duplo.display();
    delay(1000);
  }
}
void setup() {
  Serial.begin(115200);
  display_init();
  duplo.init();

  xTaskCreate(duplo_speed,         // Function name of the task
              "Duplo Loop Speed",  // Name of the task (e.g. for debugging)
              2048,                // Stack size (bytes)
              NULL,                // Parameter to pass
              1,                   // Task priority
              NULL                 // Task handle
  );

  xTaskCreate(duplo_buttons,  // Function name of the task
              "Duplo Color",  // Name of the task (e.g. for debugging)
              2048,           // Stack size (bytes)
              NULL,           // Parameter to pass
              1,              // Task priority
              NULL            // Task handle
  );
  xTaskCreate(duplo_display,  // Function name of the task
              "Duplo Color",  // Name of the task (e.g. for debugging)
              2048,           // Stack size (bytes)
              NULL,           // Parameter to pass
              1,              // Task priority
              NULL            // Task handle
  );
}

// main loop
void loop() {
  // connect flow

}  // End of loop
