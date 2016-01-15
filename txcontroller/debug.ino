
void debugPrintConfig() {
  for(int i=0; i<MAX_CHANCOUNT; i++) {    
    Serial.print(i);
    Serial.print(" ");
    Serial.print((stickConfig[i].enabled)? "E ":"D ");
    Serial.print(stickConfig[i].pinIndex);
    Serial.print(" ");
    Serial.print(stickConfig[i].enablePinIndex);
    Serial.print(" ");
    Serial.print((stickConfig[i].isDigital)? "DIG ":"ANA ");
    Serial.print((stickConfig[i].isReversed)? "REV ":"___ ");
    Serial.print((stickConfig[i].isSpringCentered)? "norm ":"thrt ");
    Serial.print(stickConfig[i].calibLow);
    Serial.print(" ");
    Serial.print(stickConfig[i].calibMid);
    Serial.print(" ");
    Serial.print(stickConfig[i].calibHigh);
    Serial.print(" ");
    Serial.println(stickConfig[i].defaultPos);
  }
}

void debugPrintSticks() {
  for(int i=0; i<MAX_CHANCOUNT; i++) {    
    bool inDefaultPosition = false;
    float f = readStick(i, inDefaultPosition);
    Serial.print(f);
    Serial.print(inDefaultPosition? "d ":"_ ");
  }
  Serial.println();
}
