#include "Duplo/duplo.h"

#include "Display/display.h"

DUPLO duplo;
byte motorPort = (byte)DuploTrainHubPort::MOTOR;

void colorSensorCallback(void *hub, byte portNumber, DeviceType deviceType, uint8_t *pData) {
  Lpf2Hub *myHub = (Lpf2Hub *)hub;

  if (deviceType == DeviceType::DUPLO_TRAIN_BASE_COLOR_SENSOR) {
    int color = myHub->parseColor(pData);
    Serial.print("Color: ");
    Serial.println(COLOR_STRING[color]);
    // myHub->setLedColor((Color)color);

    if (color == (byte)RED) {
      myHub->playSound((byte)DuploTrainBaseSound::BRAKE);
      delay(200);
    } else if (color == (byte)BLUE) {
      myHub->playSound((byte)DuploTrainBaseSound::WATER_REFILL);
      delay(200);
    } else if (color == (byte)YELLOW) {
      myHub->playSound((byte)DuploTrainBaseSound::HORN);
      delay(200);
    }
  }
}

void speedometerSensorCallback(void *hub, byte portNumber, DeviceType deviceType, uint8_t *pData) {
  Lpf2Hub *myHub = (Lpf2Hub *)hub;

  if (deviceType == DeviceType::DUPLO_TRAIN_BASE_SPEEDOMETER) {
    duplo.actual_speed = myHub->parseSpeedometer(pData);
  }
}

void DUPLO::connecting() {
  if (myHub.isConnecting()) {
    myHub.connectHub();
    if (myHub.isConnected()) {
      Serial.println("Connected to Duplo Hub");

      delay(500);
      if (myHub.isConnected()) {
        // myHub.activatePortDevice((byte)DuploTrainHubPort::COLOR, colorSensorCallback);
        myHub.activatePortDevice((byte)DuploTrainHubPort::SPEEDOMETER, speedometerSensorCallback);
        delay(1000);
        myHub.setLedColor(GREEN);
        delay(1000);
      }

    } else {
      Serial.println("Failed to connect to Duplo Hub");
    }
  } else {
    if (!myHub.isConnected()) {
      delay(2000);
      myHub.init();
    }
  }
}
void DUPLO::init() {
  myHub.init();
  analogSetAttenuation(ADC_11db);
  if (zeroY == 0) {
    zeroY = analogRead(VRY_PIN);
  }
}
void DUPLO::display() {
  display_print("Duplo Hub", 0);
  if (myHub.isConnected()) {
    display_print("Connected", 1);
  }
  display_display();
}
void DUPLO::safety() {
  if (actual_speed < 20 && actual_speed > -20) {
    if (motor_running && millis() - millis_running > 1000) {
      myHub.stopBasicMotor(motorPort);
      motor_running = false;
      safety_stop = true;
    }
  }
  if (speed > 0) {
    myHub.setLedColor(Color::WHITE);
  }
  if (speed < 0) {
    myHub.setLedColor(Color::RED);
  }
}
Color DUPLO::cycleColor() {
  static Color currentColor = Color::PINK;  // Static variable to store the current color

  int currentIndex = static_cast<int>(currentColor);

  if (currentIndex == static_cast<int>(Color::NONE)) {
    currentIndex = 0;  // Reset to 0 when it's Color::NONE
  } else {
    currentIndex = (currentIndex + 1) % static_cast<int>(Color::NUM_COLORS);

    if (currentIndex >= static_cast<int>(Color::NUM_COLORS)) {
      currentIndex = static_cast<int>(Color::NONE);  // Reset to None after the modulo operation
    }
  }

  currentColor = static_cast<Color>(currentIndex);

  return currentColor;
}
void DUPLO::loop() {
  connecting();
  //   read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // // print data to Serial Monitor
  // Serial.print("x = ");
  // Serial.print(valueX);
  // Serial.print(", y = ");
  // Serial.println(valueY);

  speed = (float(valueY) - zeroY) * (float(50) / float(2048));
  // Serial.print(", y = ");
  // Serial.print(valueY);
  // Serial.print(", zeroY = ");
  // Serial.print(zeroY);
  // Serial.print(", Speed = ");
  // Serial.println(speed);
  // Serial.print(", actual_speed = ");
  // Serial.println(actual_speed);
  // if (safety_stop) {
  //   Serial.println("safety_stop = true");
  // } else {
  //   Serial.println("safety_stop = false");
  // }

  if (speed < 20 && speed > -20) {
    speed = 0;
  }
  if (myHub.isConnected()) {
    if (speed != 0) {
      if (!safety_stop) {
        myHub.setBasicMotorSpeed(motorPort, int(speed));
        motor_running = true;
        if (millis_running == 0) {
          millis_running = millis();
        }
      }
    } else {
      if (motor_running) {
        myHub.stopBasicMotor(motorPort);
      }
      motor_running = false;
      safety_stop = false;
      millis_running = 0;
    }
  }
}