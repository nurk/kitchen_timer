void handleBleeps() {
  doBleeps();
  doBleepBlocks();
}

void doBleepBlocks() {
  if (bleepBlocksToDo > 0 && millis() > nextBleepBlockTime) {
    bleepBlocksToDo--;
    bleepsToDo = 4;
    nextBleepBlockTime = millis() + 1000;

    if (buzzing && bleepBlocksToDo == 0) {
      resetBuzzing();
    }
  }
}

void doBleeps() {
  if (bleepsToDo > 0 && millis() > nextBleepTime) {
    bleepsToDo--;
    nextBleepTime = millis() + 120;
    tone(BUZZER, beepTone, 60);
  }
}

void stopBleeps() {
  bleepsToDo = 0;
  bleepBlocksToDo = 0;
  buzzing = false;
  digitalWrite(BUZZER, LOW);
}