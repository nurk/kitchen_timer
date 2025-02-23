void handleFunction() {
  if (millis() > nextFunctionCheck) {
    isTimerDisplayed = digitalRead(FUNCTION) == HIGH;
    nextFunctionCheck = millis() + 153;
  }
}