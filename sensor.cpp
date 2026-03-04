#include "sensor.h"
#include "globale.h"

const int sampleInterval = 5;

float readTemperature(int pin) {

  long sum = 0;

  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    unsigned long currentTime = millis();
    while (millis() - currentTime < sampleInterval) {}
  }

  float avg = sum / 10.0;
  float voltage = avg * (3.3 / 4095.0);
  return voltage * 100.0;
}

void cardiacValue() {
  const int threshold = 3100;
  const int timeMeasure = 15000;
  const int sampleInterval = 5;

  int beatCount = 0;
  unsigned long timeStart = millis();
  long sum = 0;
  float avgCardiacValue = 0;
  unsigned long currentTime = millis();

  while(millis() - timeStart < timeMeasure) {
    for (int i = 0; i < 10; i++) {
      sum += analogRead(33);
      currentTime = millis();
      while (millis() - currentTime < sampleInterval) {}
    }
    avgCardiacValue = sum / 10.0;
    /**Serial.print("Avg Cardiac Value: ");
    Serial.println(avgCardiacValue);**/
    if (avgCardiacValue > threshold) {
      beatCount++;
      while(analogRead(33) > threshold) {}
    }
    sum = 0;
    avgCardiacValue = 0;
  };
  globalBPM = (beatCount * 4 );
  Serial.print("Heart Rate: ");
  Serial.println(globalBPM);
}

void cardiacTask(void * parameter) {
  for (;;) {
    cardiacValue();
    vTaskDelay(5 / portTICK_PERIOD_MS);  // 5ms delay
  }
}

float accelerometerMagnitude(MPU6050 mpu) {
  int16_t ax, ay, az, gx, gy, gz;
  long sum = 0;

  for (int i = 0; i < 10; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    float ax_g = ax / 16384.0;
    float ay_g = ay / 16384.0;
    float az_g = az / 16384.0;
    sum += sqrt(ax_g*ax_g + ay_g*ay_g + az_g*az_g);
    unsigned long currentTime = millis();
    while (millis() - currentTime < sampleInterval) {}
  }

  sum /= 10.0;
  Serial.println("Accelerometer Magnitude: " + String(sum));
  return sum;
}