const int NUMBER_OF_BATTERY_SAMPLES = 5;

int batterySamples[NUMBER_OF_BATTERY_SAMPLES];
int batteryIndex = 0;

void handleBatteryLevel() {
  if (millis() > nextBatteryLevelCheck) {
    checkBatteryLevel();
    nextBatteryLevelCheck = millis() + 976;
  }
}

void checkBatteryLevel() {
  batterySamples[batteryIndex] = analogRead(BATTERY_LEVEL_PIN);
  batteryIndex++;

  if (batteryIndex == NUMBER_OF_BATTERY_SAMPLES) {
    float averageBattery = 0;

    for (int i = 0; i < NUMBER_OF_BATTERY_SAMPLES; i++) {
      averageBattery += batterySamples[i];
    }
    averageBattery /= NUMBER_OF_BATTERY_SAMPLES;
    setBatteryIndicator(averageBattery);
    batteryIndex = 0;
  }
}

void setBatteryIndicator(float value) {
  float voltage = value * 5.0 / 1023;
  float percentage = mapf(voltage, 3.0, 4.2, 0, 100);

  Serial.print("voltage:");
  Serial.print(voltage);
  Serial.print(",percentage:");
  Serial.println(percentage);

  if (percentage > 75) {
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  } else if (percentage > 50) {
    pixels.setPixelColor(0, pixels.Color(255, 255, 0));
  } else if (percentage > 25) {
    pixels.setPixelColor(0, pixels.Color(255, 140, 0));
  } else {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  }

  pixels.show();
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}