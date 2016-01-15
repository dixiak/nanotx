
float readStick(int index, bool &inDefaultPos) {
  inDefaultPos = true;
  if (index>=MAX_CHANCOUNT) {
    return 0.5f;
  }
  float defaultPosition = stickConfig[index].defaultPos;
  if (!stickConfig[index].enabled) {
    return defaultPosition;
  }
  float result = (stickConfig[index].isDigital)? readStickDigital(index):readStickAnalog(index);
  if ((max(result, defaultPosition)-min(result, defaultPosition)) > ARM_POS_THRESHOLD) {
    inDefaultPos = false;
  }
  if (!armed) {
    return defaultPosition;
  }
  return (stickConfig[index].isReversed)? 1.0f-result:result;
}

float readStickAnalog(int index) {
  if (stickConfig[index].enablePinIndex>=0) {
    digitalWrite(stickConfig[index].enablePinIndex, HIGH);
  }
  int nowRead = analogRead(stickConfig[index].pinIndex);
  if (stickConfig[index].enablePinIndex>=0) {
    digitalWrite(stickConfig[index].enablePinIndex, LOW);
  }  
  int calLow = stickConfig[index].calibLow;
  int calMid = stickConfig[index].calibMid;
  int calHigh = stickConfig[index].calibHigh;

  if (nowRead<=calLow) {
    return 0.0f;    
  }
  if (nowRead>=calHigh) {
    return 1.0f;
  }

  if (stickConfig[index].isSpringCentered) {
    if (nowRead<=calMid) {
      return ((float)(nowRead-calLow)) / (2.0f * ((float)(calMid-calLow)));
    }
    return 0.5f + ((float)(nowRead-calMid)) / (2.0f * ((float)(calHigh-calMid)));
  }
  return ((float)(nowRead-calLow)) / ((float)(calHigh-calLow));
}

float readStickDigital(int index) {
  bool nowRead = digitalRead(stickConfig[index].pinIndex)==HIGH;
  return nowRead? 1.0f:0.0f;
}

