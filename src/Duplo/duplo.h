#ifndef DUPLO_H
#define DUPLO_H

#include <Lpf2Hub.h>

#define VRX_PIN 39  // ESP32 pin GPIO39 (ADC3) connected to VRX pin
#define VRY_PIN 36  // ESP32 pin GPIO36 (ADC0) connected to VRY pin

void colorSensorCallback(void *hub, byte portNumber, DeviceType deviceType, uint8_t *pData);

class DUPLO {  // The class
 public:       // Access specifier
  Lpf2Hub myHub;
  byte motorPort = (byte)DuploTrainHubPort::MOTOR;
  void init();
  void connecting();
  void loop();
  void display();
  void buttons();

 private:
  int valueX = 0;  // to store the X-axis value
  int valueY = 0;  // to store the Y-axis value
  bool motor_running = false;
  // bool forward = false;
  Color cycleColor();
  int zeroY = 0;
};

// create a hub instance
extern DUPLO duplo;

#endif /* DUPLO_H */
