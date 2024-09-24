/*
https://github.com/panStamp/thermistor
https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/
*/

const int NOMINAL_RESISTANCE = 10000;
const int SERIAL_RESISTANCE = 10000;
const int BETA_COEFFICIENT = 3435;
const int ADC_RESOLUTION = 1023;
const int TEMPERATURE_NOMINAL = 25;
const int NUMBER_OF_TEMPERATURE_SAMPLES = 10;

int temperatureSamples[NUMBER_OF_TEMPERATURE_SAMPLES];
int temperatureIndex = 0;

void handleTemperature() {
  if (millis() > nextTemperatureCheck) {
    checkTemperature();
    nextTemperatureCheck = millis() + 977;
  }
}

void checkTemperature() {
  Serial.println(analogRead(TEMP_V));
  digitalWrite(TEMP_GND, LOW);
  temperatureSamples[temperatureIndex] = analogRead(TEMP_V);
  digitalWrite(TEMP_GND, HIGH);
  temperatureIndex++;

  if (temperatureIndex == NUMBER_OF_TEMPERATURE_SAMPLES) {
    float averegaTemperature = 0;

    for (int i = 0; i < NUMBER_OF_TEMPERATURE_SAMPLES; i++) {
      averegaTemperature += temperatureSamples[i];
    }
    averegaTemperature /= NUMBER_OF_TEMPERATURE_SAMPLES;
    convertThermistorReading(averegaTemperature);
    temperatureIndex = 0;
  }
}

void convertThermistorReading(float thermistorReading) {
  Serial.print("Thermistor reading ");
  Serial.println(thermistorReading);

  // convert the value to resistance
  float thermistorResistance = ADC_RESOLUTION / thermistorReading - 1;
  thermistorResistance = SERIAL_RESISTANCE * thermistorResistance;

  Serial.print("Thermistor resistance ");
  Serial.println(thermistorResistance);

  float steinhart;
  steinhart = thermistorResistance / NOMINAL_RESISTANCE;  // (R/Ro)
  steinhart = log(steinhart);                             // ln(R/Ro)
  steinhart /= BETA_COEFFICIENT;                          // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15);      // + (1/To)
  steinhart = 1.0 / steinhart;                            // Invert
  steinhart -= 273.15;                                    // convert to C
  steinhart *= 100;                                       // make in 10ths of degrees

  Serial.print("Temperature ");
  Serial.print(steinhart);
  Serial.println(" 10ths of degrees C°");

  int steinhartInt = static_cast<int>(round(steinhart));

  tempMostSignificant = steinhartInt / 100;
  tempLeastSignificant = steinhartInt % 100;
}