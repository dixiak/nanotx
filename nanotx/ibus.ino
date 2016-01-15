
byte ibusFrame[32] = { 0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0 };

volatile bool ibusDataReady = false;

void fillIbus() {
  if (IBUS_ENABLE) {
    if (!ibusDataReady) {
      bool inDefaultPosition = true;
      for(int i=0; i<MAX_CHANCOUNT; i++) {
        setIbusChannel(i, readStick(i, inDefaultPosition));
      }
      uint16_t checksum = 0xffff;
      for(int i=0; i<30; i++) {
        checksum-=ibusFrame[i];
      }
      ibusFrame[31] = 0xff & (checksum >> 8);
      ibusFrame[30] = 0xff & (checksum);    
      ibusDataReady = true;
    }
  }
}

void setIbusChannel(int channelIndex, float channelValue) {
  int chanIntValue = 1000 + toInt(1000.0 * channelValue);
  ibusFrame[(channelIndex * 2) + 3] = 0xff & (chanIntValue >> 8);
  ibusFrame[(channelIndex * 2) + 2] = 0xff & (chanIntValue);
}

int getIbusChannel(int channelIndex) {
  int chanValue = 0;
  chanValue += ibusFrame[(channelIndex * 2) + 3] << 8;
  chanValue += ibusFrame[(channelIndex * 2) + 2];
  return chanValue;
}

void ibusAsDebug() {
//    for(int i=0; i<32; i++) {
//      Serial.print(ibusFrame[i], HEX);
//    }

  for (int i=0; i<14; i++) {
    Serial.print(getIbusChannel(i));
    Serial.print(" ");
  }
  Serial.println();
}

void sendIbusFrameIfReady() {
  if (IBUS_ENABLE) {
    if (ibusDataReady) {  
      if (IBUS_AS_DEBUG) {
        ibusAsDebug();
      } else {
        for(int i=0; i<32; i++) {
          Serial.write(ibusFrame[i]);
        }
      }
      ibusDataReady = false;
    }
  }
}

