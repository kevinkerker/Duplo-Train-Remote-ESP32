#include "Duplo/duplo.h"

#include "Display/display.h"

DUPLO duplo;

void DUPLO::colorSensorCallback(byte portNumber, DeviceType deviceType, uint8_t *pData) {
  if (deviceType == DeviceType::DUPLO_TRAIN_BASE_COLOR_SENSOR) {
    int color = myHub.parseColor(pData);
    Serial.print("Color: ");
    Serial.println(COLOR_STRING[color]);
    if (color == (byte)RED) {
      myHub.stopBasicMotor(motorPort);
      delay(100);
      myHub.playSound((byte)DuploTrainBaseSound::BRAKE);
    } else if (color == (byte)BLUE) {
      myHub.playSound((byte)DuploTrainBaseSound::WATER_REFILL);
    } else if (color == (byte)YELLOW) {
      myHub.playSound((byte)DuploTrainBaseSound::HORN);
    }
  }
}

void DUPLO::speedometerSensorCallback(byte portNumber, DeviceType deviceType, uint8_t *pData) {
  if (deviceType == DeviceType::DUPLO_TRAIN_BASE_SPEEDOMETER) {
    int speed = myHub.parseSpeedometer(pData);
    Serial.print("Speed: ");
    Serial.println(speed);
    if (speed > 10) {
      Serial.println("Forward");
      myHub.setBasicMotorSpeed(motorPort, 40);
    } else if (speed < -10) {
      Serial.println("Back");
      myHub.setBasicMotorSpeed(motorPort, -40);
    } else {
      Serial.println("Stop");
      myHub.stopBasicMotor(motorPort);
    }
  }
}
void DUPLO::connecting() {
  if (myHub.isConnecting()) {
    myHub.connectHub();
    if (myHub.isConnected()) {
      Serial.println("Connected to Duplo Hub");

      display_print("Connected", 1);

      delay(500);
      // connect color sensor and activate it for updates
      Serial.println("Init speedometer");
      // myHub.activatePortDevice((byte)DuploTrainHubPort::SPEEDOMETER, speedometerSensorCallback);
      delay(500);
      // connect speed sensor and activate it for updates
      Serial.println("Init color sensor");
      // myHub.activatePortDevice((byte)DuploTrainHubPort::COLOR, colorSensorCallback);
      if (myHub.isConnected()) {
        delay(500);
        myHub.setLedColor(GREEN);
        delay(1000);
        myHub.setBasicMotorSpeed(motorPort, 50);
      }

    } else {
      Serial.println("Failed to connect to Duplo Hub");
    }
  }
}
void DUPLO::init() {
  myHub.init();
  analogSetAttenuation(ADC_11db);
}
void DUPLO::loop() {
  connecting();
  //   read X and Y analog values
  valueX = analogRead(VRX_PIN);
  valueY = analogRead(VRY_PIN);

  // print data to Serial Monitor
  Serial.print("x = ");
  Serial.print(valueX);
  Serial.print(", y = ");
  Serial.println(valueY);

  if (myHub.isConnected()) {
    if (valueY > 3000) {
      Serial.println("Forward");
      myHub.setBasicMotorSpeed(motorPort, 50);
    } else if (valueY < 1000) {
      Serial.println("Back");
      myHub.setBasicMotorSpeed(motorPort, -50);
    } else {
      Serial.println("Stop");
      myHub.stopBasicMotor(motorPort);
    }
  }
  delay(100);
}