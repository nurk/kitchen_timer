/**
  SW: 6.3
  HW: 6.3

  ATmega4808
  Internal 16 MHz
  EEPROM retained
  32 pin standard
  Reset pin GPIO
  Serial UPDI
**/

#define DEBUG false
#define Serial \
  if (DEBUG) Serial

#include "HystFilter.h"
#include <JC_Button.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

#define USE_TIMER_0 true
#define TIMER_INTERVAL_MS 1000L
#include <megaAVR_TimerInterrupt.h>

#define A_SEGMENT PIN_PD4
#define B_SEGMENT PIN_PC3
#define C_SEGMENT PIN_PD1
#define D_SEGMENT PIN_PD5
#define E_SEGMENT PIN_PD6
#define F_SEGMENT PIN_PD2
#define G_SEGMENT PIN_PD0
#define DP_SEGMENT PIN_PD3

#define DIGIT_1 PIN_PC2
#define DIGIT_2 PIN_PC1
#define DIGIT_3 PIN_PC0
#define DIGIT_4 PIN_PA7

#define START_STOP PIN_PA0
#define TIME_RESET PIN_PA1
#define MINUTES PIN_PA2
#define SECONDS PIN_PA3

#define BUZZER PIN_PA6

#define BATTERY_LEVEL_PIN PIN_PF2 // needs bodge wire from pf0 to pf2. pf0 cannot do analog read
#define BATTERY_LEVEL_LED_PIN PIN_PD7

#define TEMP_V PIN_PF4
#define TEMP_GND PIN_PF1
#define FUNCTION PIN_PF5

const int segments[] = { A_SEGMENT, B_SEGMENT, C_SEGMENT, D_SEGMENT, E_SEGMENT, F_SEGMENT, G_SEGMENT, DP_SEGMENT };
const int digits[] = { DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4 };

// Dp G F E D C B A
const byte numbers[] = {
  0X3F,  // 0
  0X06,  // 1
  0X5B,  // 2
  0X4F,  // 3
  0X66,  // 4
  0X6D,  // 5
  0X7C,  // 6
  0X07,  // 7
  0X7F,  // 8
  0X6F   // 9
};
const byte blankDigit = 0X00;
const byte aDigit = 0X77;

int currentDigit = 0;
unsigned long digitMillis = millis();

const Button startStopButton(START_STOP);
const Button timeResetButton(TIME_RESET);
const Button minutesButton(MINUTES);
const Button secondsButton(SECONDS);
boolean resetLongPressed = false;
// for state machine used by mintues and seconds button
// see https://github.com/JChristensen/JC_Button/blob/master/examples/UpDown/UpDown.ino
const unsigned long REPEAT_FIRST(500);
const unsigned long REPEAT_INCR(100);
unsigned long rpt(REPEAT_FIRST);
enum states_t { WAIT,
                MINUTES_UP,
                SECONDS_UP };
states_t STATE;

int seconds = 0;
int minutes = 0;
int previousSeconds = 0;
int previousMinutes = 0;
const int eepromMinutesAddress = 0;
const int eepromSecondsAddress = 2;
bool counting = false;
bool buzzing = false;
bool pauzed = false;

const int beepTone = 1800;

int bleepsToDo = 0;
unsigned long nextBleepTime = millis();

int bleepBlocksToDo = 0;
unsigned long nextBleepBlockTime = millis();

boolean setupMenu = false;
int menuItem = 0;
int alarm5MinutesBleeps = 0;
int alarm10MinutesBleeps = 0;
int alarm15MinutesBleeps = 0;
int alarm20MinutesBleeps = 0;
const int alarm5MinutesAddress = 4;
const int alarm10MinutesAddress = 6;
const int alarm15MinutesAddress = 8;
const int alarm20MinutesAddress = 10;


Adafruit_NeoPixel pixels(1, BATTERY_LEVEL_LED_PIN, NEO_GRB + NEO_KHZ800);
unsigned long nextBatteryLevelCheck = 0;

unsigned long nextFunctionCheck = millis();
boolean isTimerDisplayed = true;

unsigned long nextTemperatureCheck = millis();
int tempMostSignificant = 0;
int tempLeastSignificant = 0;

void setup() {
  Serial.swap(1);
  Serial.begin(9600);
  pinMode(A_SEGMENT, OUTPUT);
  pinMode(B_SEGMENT, OUTPUT);
  pinMode(C_SEGMENT, OUTPUT);
  pinMode(D_SEGMENT, OUTPUT);
  pinMode(E_SEGMENT, OUTPUT);
  pinMode(F_SEGMENT, OUTPUT);
  pinMode(G_SEGMENT, OUTPUT);
  pinMode(DP_SEGMENT, OUTPUT);

  pinMode(DIGIT_1, OUTPUT);
  pinMode(DIGIT_2, OUTPUT);
  pinMode(DIGIT_3, OUTPUT);
  pinMode(DIGIT_4, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(FUNCTION, INPUT_PULLUP);
  pinMode(TEMP_GND, OUTPUT);

  pixels.begin();
  pixels.setBrightness(10);
  pixels.setPixelColor(0, pixels.Color(255, 255, 255));
  pixels.show();

  digitalWrite(TEMP_GND, LOW);
  convertThermistorReading(analogRead(TEMP_V));
  digitalWrite(TEMP_GND, HIGH);

  isTimerDisplayed = digitalRead(FUNCTION) == HIGH;

  startStopButton.begin();
  timeResetButton.begin();
  minutesButton.begin();
  secondsButton.begin();

  ITimer0.init();
  ITimer0.attachInterruptInterval(TIMER_INTERVAL_MS, timerHandler);

  loadEEPROM();
}

void loop() {
  handleFunction();
  handleBatteryLevel();
  handleTemperature();
  multiplexDigits();
  handleButtons();
  handleBleeps();
}

void loadEEPROM() {
  int eepromSeconds = 0;
  int eepromMinutes = 0;
  EEPROM.get(eepromSecondsAddress, eepromSeconds);
  EEPROM.get(eepromMinutesAddress, eepromMinutes);
  EEPROM.get(alarm5MinutesAddress, alarm5MinutesBleeps);
  EEPROM.get(alarm10MinutesAddress, alarm10MinutesBleeps);
  EEPROM.get(alarm15MinutesAddress, alarm15MinutesBleeps);
  EEPROM.get(alarm20MinutesAddress, alarm20MinutesBleeps);

  if (eepromSeconds <= 59 && eepromSeconds >= 0) {
    seconds = eepromSeconds;
    previousSeconds = eepromSeconds;
  }
  if (eepromMinutes <= 99 && eepromMinutes >= 0) {
    minutes = eepromMinutes;
    previousMinutes = eepromMinutes;
  }

  alarm5MinutesBleeps = min(9, max(0, alarm5MinutesBleeps));
  alarm10MinutesBleeps = min(9, max(0, alarm10MinutesBleeps));
  alarm15MinutesBleeps = min(9, max(0, alarm15MinutesBleeps));
  alarm20MinutesBleeps = min(9, max(0, alarm20MinutesBleeps));
}