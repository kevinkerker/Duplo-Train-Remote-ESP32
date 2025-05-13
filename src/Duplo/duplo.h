#ifndef DUPLO_H
#define DUPLO_H

#include <Lpf2Hub.h>

#define VRX_PIN 39  // ESP32 pin GPIO39 (ADC3) connected to VRX pin
#define VRY_PIN 36  // ESP32 pin GPIO36 (ADC0) connected to VRY pin

extern byte motorPort;

void colorSensorCallback(void *hub, byte portNumber, DeviceType deviceType, uint8_t *pData);
void speedometerSensorCallback(void *hub, byte portNumber, DeviceType deviceType, uint8_t *pData);

class DUPLO {  // The class
 public:       // Access specifier
  Lpf2Hub myHub;
  void init();
  void connecting();
  void loop();
  void display();
  void lights();
  int actual_speed = 0;

 private:
  int valueX = 0;  // to store the X-axis value
  int valueY = 0;  // to store the Y-axis value
  bool motor_running = false;
  // bool forward = false;
  Color cycleColor();
  int zeroY = 0;
  bool safety_stop = false;
  unsigned long millis_running = 0;
  float speed = 0;
};

// create a hub instance
extern DUPLO duplo;

#endif /* DUPLO_H */
