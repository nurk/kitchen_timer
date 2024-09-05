void multiplexDigits() {
  if (millis() > digitMillis + 1) {
    int left = 0;
    int right = 0;

    if (isTimerDisplayed) {
      left = minutes;
      right = seconds;
    } else {
      left = tempMostSignificant;
      right = tempLeastSignificant;
    }

    digitalWrite(digits[currentDigit], LOW);
    currentDigit = (currentDigit + 1) % 4;

    switch (digits[currentDigit]) {
      case DIGIT_1:
        if (setupMenu) {
          writeNumber(aDigit, false);
        } else {
          writeNumber(numbers[left / 10], false);
        }
        break;
      case DIGIT_2:
        {
          if (setupMenu) {
            switch (menuItem) {
              case -1:
                writeNumber(blankDigit, false);
                break;
              case 0:
                writeNumber(numbers[5], true);
                break;
              case 1:
                writeNumber(numbers[1], false);
                break;
              case 2:
                writeNumber(numbers[1], false);
                break;
              case 3:
                writeNumber(numbers[2], false);
                break;
            }
          } else {
            boolean withDp = true;
            if ((counting && !pauzed) || buzzing) {
              if ((millis() / 500) % 2 != 0) {
                withDp = false;
              }
            }
            writeNumber(numbers[left % 10], withDp);
          }
          break;
        }
      case DIGIT_3:
        if (setupMenu) {
          switch (menuItem) {
            case -1:
                writeNumber(blankDigit, false);
                break;
            case 0:
              writeNumber(blankDigit, false);
              break;
            case 1:
              writeNumber(numbers[0], true);
              break;
            case 2:
              writeNumber(numbers[5], true);
              break;
            case 3:
              writeNumber(numbers[0], true);
              break;
          }
        } else {
          writeNumber(numbers[right / 10], false);
        }
        break;
      case DIGIT_4:
        if (setupMenu) {
          switch (menuItem) {
            case -1:
                writeNumber(blankDigit, false);
                break;
              case 0:
              writeNumber(numbers[alarm5MinutesBleeps], false);
              break;
            case 1:
              writeNumber(numbers[alarm10MinutesBleeps], false);
              break;
            case 2:
              writeNumber(numbers[alarm15MinutesBleeps], false);
              break;
            case 3:
              writeNumber(numbers[alarm20MinutesBleeps], false);
              break;
          }
        } else {
          writeNumber(numbers[right % 10], false);
        }
        break;
    }

    digitalWrite(digits[currentDigit], HIGH);
    digitMillis = millis();
  }
}

void writeNumber(byte number, boolean withDp) {
  for (int i = 0, mask = 1; i < 8; i++, mask = mask << 1) {
    if (number & mask) {
      digitalWrite(segments[i], HIGH);
    } else {
      digitalWrite(segments[i], LOW);
    }
  }

  if (withDp) {
    digitalWrite(DP_SEGMENT, HIGH);
  }
}