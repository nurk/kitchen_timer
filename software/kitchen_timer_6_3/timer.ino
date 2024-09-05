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
  if (counting && !pauzed && seconds == 0) {
    if (minutes == 20) {
      bleepBlocksToDo = alarm20MinutesBleeps;
    } else if (minutes == 15) {
      bleepBlocksToDo = alarm15MinutesBleeps;
    } else if (minutes == 10) {
      bleepBlocksToDo = alarm10MinutesBleeps;
    } else if (minutes == 5) {
      bleepBlocksToDo = alarm5MinutesBleeps;
    }
  }
}

void handleAlarm() {
  bleepBlocksToDo = 60;
}