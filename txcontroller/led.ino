
unsigned long ledHighStateLength = 0;
unsigned long ledLowStateLength = 0;
bool mainLedState = false;
unsigned long stateSwitchAfter = 0;

void mainLedModeOn() {
  ledLowStateLength = 0;
  ledHighStateLength = 1;
  stateSwitchAfter = 0;
}

void mainLedModeFastBlink() {
  ledLowStateLength = 200;
  ledHighStateLength = 200;
  stateSwitchAfter = 0;
}

void mainLedModeSlowBlink() {
  ledLowStateLength = 800;
  ledHighStateLength = 800;
  stateSwitchAfter = 0;
}

void mainLedFlicker() {
  ledLowStateLength = 800;
  ledHighStateLength = 200;
  stateSwitchAfter = 0;
}

bool setLED(bool newState) {
  mainLedState = newState;
  digitalWrite(MAIN_LED_PIN, mainLedState? HIGH:LOW);
  return newState;
}

void mainLedService() {

  if (0==ledLowStateLength) {
    if (!mainLedState) {
      setLED(true);
    }
    return;
  }

  unsigned long now = millis();
  if (now>stateSwitchAfter) {
    if (setLED(!mainLedState)) {
      stateSwitchAfter = now + ledHighStateLength;
    } else {
      stateSwitchAfter = now + ledLowStateLength;
    }
  }
}
