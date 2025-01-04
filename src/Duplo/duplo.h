#ifndef DUPLO_H
#define DUPLO_H

#include <Lpf2Hub.h>

#define VRX_PIN 39  // ESP32 pin GPIO39 (ADC3) connected to VRX pin
#define VRY_PIN 36  // ESP32 pin GPIO36 (ADC0) connected to VRY pin

class DUPLO {  // The class
 public:       // Access specifier
  Lpf2Hub myHub;
  byte motorPort = (byte)DuploTrainHubPort::MOTOR;
  void colorSensorCallback(byte portNumber, DeviceType deviceType, uint8_t *pData);
  void speedometerSensorCallback(byte portNumber, DeviceType deviceType, uint8_t *pData);
  void init();
  void connecting();
  void loop();

 private:
  int valueX = 0;  // to store the X-axis value
  int valueY = 0;  // to store the Y-axis value
};

// create a hub instance
extern DUPLO duplo;

#endif /* DUPLO_H */
