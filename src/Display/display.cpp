#include "Display/display.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void display_init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("Duplo Hub"));
  display.display();
}

void display_print(String message, uint8_t line) {
  switch (line) {
    case 0:
      display.setCursor(10, 0);
      display.println(message);
      break;
    case 1:
      display.setCursor(10, 16);
      display.println(message);
      break;
    default:
      break;
  }
}

void display_display() {
  display.display();
  display.clearDisplay();
  display.setTextSize(2);  // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
}
