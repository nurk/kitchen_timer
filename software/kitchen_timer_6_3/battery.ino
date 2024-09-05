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
    int averageBattery = 0;

    for (int i = 0; i < NUMBER_OF_BATTERY_SAMPLES; i++) {
      averageBattery += batterySamples[i];
    }
    averageBattery /= NUMBER_OF_BATTERY_SAMPLES;
    setBatteryIndicator(averageBattery);
    batteryIndex = 0;
  }
}

void setBatteryIndicator(int value) {
  Serial.print("Set battery level ");
  Serial.println(value);

  if (value > 825) {
    Serial.println("Green");
    pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  } else if (value > 735) {
    Serial.println("Yellow");
    pixels.setPixelColor(0, pixels.Color(255, 255, 0));
  } else if (value > 655) {
    Serial.println("Orange");
    pixels.setPixelColor(0, pixels.Color(255, 140, 0));
  } else {
    Serial.println("Red");
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  }

  pixels.show();
}