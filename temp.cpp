#include "temp.h"

float readTemperature(int pin) {
  long sum = 0;

  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(5);
  }

  float avg = sum / 10.0;
  float voltage = avg * (3.3 / 4095.0);
  return voltage * 100.0;
}