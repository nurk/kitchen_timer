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
          if (menuItem != 4) {
            writeDigit(aDigit, false);
          } else {
            writeDigit(cDigit, false);
          }
        } else {
          writeDigit(numbers[left / 10], false);
        }
        break;
      case DIGIT_2:
        {
          if (setupMenu) {
            switch (menuItem) {
              case -1:
                writeDigit(blankDigit, false);
                break;
              case 0:
                writeDigit(numbers[5], true);
                break;
              case 1:
                writeDigit(numbers[1], false);
                break;
              case 2:
                writeDigit(numbers[1], false);
                break;
              case 3:
                writeDigit(numbers[2], false);
                break;
              case 4:
                if (tempOffset < -0.01) {
                  writeDigit(minusDigit, false);
                } else {
                  writeDigit(blankDigit, false);
                }
                break;
            }
          } else {
            boolean withDp = true;
            if ((counting && !pauzed) || buzzing) {
              if ((millis() / 500) % 2 != 0) {
                withDp = false;
              }
            }
            writeDigit(numbers[left % 10], withDp);
          }
          break;
        }
      case DIGIT_3:
        if (setupMenu) {
          switch (menuItem) {
            case -1:
              writeDigit(blankDigit, false);
              break;
            case 0:
              writeDigit(blankDigit, false);
              break;
            case 1:
              writeDigit(numbers[0], true);
              break;
            case 2:
              writeDigit(numbers[5], true);
              break;
            case 3:
              writeDigit(numbers[0], true);
              break;
            case 4:
              writeDigit(numbers[static_cast<int>((fabs(tempOffset) * 10) / 10)], true);
              break;
          }
        } else {
          writeDigit(numbers[right / 10], false);
        }
        break;
      case DIGIT_4:
        if (setupMenu) {
          switch (menuItem) {
            case -1:
              writeDigit(blankDigit, false);
              break;
            case 0:
              writeDigit(numbers[alarm5MinutesBleeps], false);
              break;
            case 1:
              writeDigit(numbers[alarm10MinutesBleeps], false);
              break;
            case 2:
              writeDigit(numbers[alarm15MinutesBleeps], false);
              break;
            case 3:
              writeDigit(numbers[alarm20MinutesBleeps], false);
              break;
            case 4:
              writeDigit(numbers[static_cast<int>(round(fabs(tempOffset) * 10)) % 10], false);
              break;
          }
        } else {
          writeDigit(numbers[right % 10], false);
        }
        break;
    }

    digitalWrite(digits[currentDigit], HIGH);
    digitMillis = millis();
  }
}

void writeDigit(byte number, boolean withDp) {
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