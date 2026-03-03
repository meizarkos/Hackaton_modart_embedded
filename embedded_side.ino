const int lm35Pin = 32;
const int resistPin = 34;

void setup() {
  Serial.begin(9600);

  // Configuration ADC ESP32
  analogReadResolution(12);                 // 0–4095
  analogSetPinAttenuation(lm35Pin, ADC_11db); // ~0–3.3V
  analogSetPinAttenuation(resistPin, ADC_11db);
}

void loop() {
  int adcValue = analogRead(lm35Pin);

  // Conversion ADC -> tension
  float voltage = adcValue * (3.3 / 4095.0);

  // LM35: 10mV par °C
  float temperatureC = voltage * 100.0;

  Serial.print("ADC: ");
  Serial.print(adcValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | Température: ");
  Serial.print(temperatureC, 1);
  Serial.println(" °C");


  int resistValue = analogRead(resistPin);
  Serial.print("Bande resistante :");
  Serial.println(resistValue);

  delay(1000);
}