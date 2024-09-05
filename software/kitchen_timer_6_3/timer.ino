void timerHandler() {
  if (counting && !pauzed && !(seconds == 1 && minutes == 0)) {
    seconds--;
    if (seconds < 0) {
      seconds = 59;
      minutes--;
      if (minutes < 0) {
        seconds = 0;
        minutes = 0;
      }
    }
    handleIntermediateAlarms();
  } else {
    if (counting && !pauzed) {
      seconds = 0;
      buzzing = true;
      handleAlarm();
    }
    counting = false;
  }
}

void handleIntermediateAlarms() {
  if (counting && !pauzed && minutes == 20 && seconds == 0) {
    bleepBlocksToDo = alarm20MinutesBleeps;
  } else if (counting && !pauzed && minutes == 15 && seconds == 0) {
    bleepBlocksToDo = alarm15MinutesBleeps;
  } else if (counting && !pauzed && minutes == 10 && seconds == 0) {
    bleepBlocksToDo = alarm10MinutesBleeps;
  } else if (counting && !pauzed && minutes == 5 && seconds == 0) {
    bleepBlocksToDo = alarm5MinutesBleeps;
  }
}

void handleAlarm() {
  bleepBlocksToDo = 60;
}