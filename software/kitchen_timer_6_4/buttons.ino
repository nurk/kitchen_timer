void handleButtons() {
  startStopButton.read();
  timeResetButton.read();
  minutesButton.read();
  secondsButton.read();

  handleStarStoptButton();
  handleTimeResetButton();
  handleTimeButtons();
}

void handleStarStoptButton() {
  if (startStopButton.wasPressed()) {
    buttonTone();
  }
  if (startStopButton.pressedFor(1000)) {
    setupMenu = true;
    menuItem = -1;
  }
  if (startStopButton.wasReleased()) {
    if (setupMenu) {
      menuItem = (menuItem + 1) % 5;
    } else {
      if (buzzing) {
        resetBuzzing();
      } else {
        if (pauzed && counting) {
          pauzed = false;
        } else {
          if (counting) {
            pauzed = true;
          } else if (!(minutes == 0 && seconds == 0)) {
            counting = true;
            pauzed = false;
          }
        }
      }
    }
  }
}

void resetBuzzing() {
  stopBleeps();
  counting = false;
  minutes = previousMinutes;
  seconds = previousSeconds;
}

void handleTimeResetButton() {
  if (timeResetButton.wasPressed()) {
    buttonTone();
  }
  if (timeResetButton.pressedFor(1000)) {
    if (!resetLongPressed) {
      buttonTone();
    }
    resetLongPressed = true;
  }
  if (timeResetButton.wasReleased()) {
    if (resetLongPressed) {
      EEPROM.put(eepromMinutesAddress, previousMinutes);
      EEPROM.put(eepromSecondsAddress, previousSeconds);
    } else if (setupMenu) {
      setupMenu = false;
    } else {
      counting = false;
      pauzed = false;
      stopBleeps();
      minutes = 0;
      seconds = 0;
      previousSeconds = seconds;
      previousMinutes = minutes;
    }
    resetLongPressed = false;
  }
}

void handleTimeButtons() {
  switch (STATE) {
    case WAIT:
      if (minutesButton.wasPressed()) {
        STATE = MINUTES_UP;
      } else if (minutesButton.wasReleased()) {
        rpt = REPEAT_FIRST;
      } else if (minutesButton.pressedFor(rpt)) {
        rpt += REPEAT_INCR;
        STATE = MINUTES_UP;
      } else if (secondsButton.wasPressed()) {
        STATE = SECONDS_UP;
      } else if (secondsButton.wasReleased()) {
        rpt = REPEAT_FIRST;
      } else if (secondsButton.pressedFor(rpt)) {
        rpt += REPEAT_INCR;
        STATE = SECONDS_UP;
      }
      break;
    case MINUTES_UP:
      buttonTone();
      if (setupMenu) {
        alarmUp();
      } else {
        minutes++;
        if (minutes > 99) {
          minutes = 0;
        }
        previousMinutes = minutes;
      }
      STATE = WAIT;
      break;
    case SECONDS_UP:
      buttonTone();
      if (setupMenu) {
        alarmDown();
      } else {
        seconds++;
        if (seconds > 59) {
          seconds = 0;
        }
        previousSeconds = seconds;
      }
      STATE = WAIT;
      break;
  }
}

void alarmUp() {
  switch (menuItem) {
    case 0:
      alarm5MinutesBleeps = min(9, alarm5MinutesBleeps + 1);
      EEPROM.put(alarm5MinutesAddress, alarm5MinutesBleeps);
      break;
    case 1:
      alarm10MinutesBleeps = min(9, alarm10MinutesBleeps + 1);
      EEPROM.put(alarm10MinutesAddress, alarm10MinutesBleeps);
      break;
    case 2:
      alarm15MinutesBleeps = min(9, alarm15MinutesBleeps + 1);
      EEPROM.put(alarm15MinutesAddress, alarm15MinutesBleeps);
      break;
    case 3:
      alarm20MinutesBleeps = min(9, alarm20MinutesBleeps + 1);
      EEPROM.put(alarm20MinutesAddress, alarm20MinutesBleeps);
      break;
    case 4:
      tempOffset = fmin(9.9, tempOffset + 0.1);
      EEPROM.put(tempOffsetAddress, tempOffset);
      break;
  }
}

void alarmDown() {
  switch (menuItem) {
    case 0:
      alarm5MinutesBleeps = max(0, alarm5MinutesBleeps - 1);
      EEPROM.put(alarm5MinutesAddress, alarm5MinutesBleeps);
      break;
    case 1:
      alarm10MinutesBleeps = max(0, alarm10MinutesBleeps - 1);
      EEPROM.put(alarm10MinutesAddress, alarm10MinutesBleeps);
      break;
    case 2:
      alarm15MinutesBleeps = max(0, alarm15MinutesBleeps - 1);
      EEPROM.put(alarm15MinutesAddress, alarm15MinutesBleeps);
      break;
    case 3:
      alarm20MinutesBleeps = max(0, alarm20MinutesBleeps - 1);
      EEPROM.put(alarm20MinutesAddress, alarm20MinutesBleeps);
      break;
    case 4:
      tempOffset = fmax(-9.9, tempOffset - 0.1);
      EEPROM.put(tempOffsetAddress, tempOffset);
      break;
  }
}

void buttonTone() {
  digitalWrite(BUZZER, LOW);
  tone(BUZZER, beepTone, 60);
}
