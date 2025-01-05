#include "Duplo/duplo.h"

#include "Display/display.h"

DUPLO duplo;

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

void DUPLO::connecting() {
  if (myHub.isConnecting()) {
    myHub.connectHub();
    if (myHub.isConnected()) {
      Serial.println("Connected to Duplo Hub");

      delay(500);
      if (myHub.isConnected()) {
        // myHub.activatePortDevice((byte)DuploTrainHubPort::COLOR, colorSensorCallback);
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
void DUPLO::buttons() {
  // uint8_t action_color = 0;
  // myHub.parseColor(&action_color);
  if (myHub.isConnected()) {
    Color currentColor = cycleColor();
    Serial.print("Current Color: ");
    Serial.println(static_cast<int>(currentColor));
    myHub.setLedColor(currentColor);
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

  float speed = (float(valueY) - zeroY) * (float(50) / float(2048));
  // Serial.print(", y = ");
  // Serial.print(valueY);
  // Serial.print(", zeroY = ");
  // Serial.print(zeroY);
  // Serial.print(", Speed = ");
  // Serial.println(speed);

  if (speed < 20 && speed > -20) {
    speed = 0;
  }
  if (myHub.isConnected()) {
    if (speed != 0) {
      myHub.setBasicMotorSpeed(motorPort, int(speed));
      motor_running = true;
    } else {
      if (motor_running) {
        myHub.stopBasicMotor(motorPort);
      }
      motor_running = false;
    }
  }
  if (speed > 0) {
    myHub.setLedColor(Color::WHITE);
  }
  if (speed < 0) {
    myHub.setLedColor(Color::RED);
  }
}