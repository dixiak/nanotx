

long meterLastTime = 0;
float movingAverage = 0.0f;
int rawVoltage = 0;

void voltageRead() {
  if (METER_ENABLE) {
    rawVoltage = analogRead(VOLTAGE_IN_PIN);
  }
}

void meterService() {  
  if (METER_ENABLE) {
    long now = millis();
    if (now-meterLastTime>50) {

      meterLastTime = now;
      float actualVoltage = 1.0427f * 3.0f * 5.0f * ((float)rawVoltage / 1023.0f);
      float diff = (min(abs(actualVoltage-movingAverage), 2.0f)) / 2.0f;
      movingAverage = (diff * actualVoltage) + ((1.0f-diff) * movingAverage);
      
      float scale = (float)(METER_HIGH - METER_LOW);
      float voltageScale = (movingAverage - VOLTAGE_LOW)/(VOLTAGE_HIGH - VOLTAGE_LOW);
      int channelValue = METER_LOW + toInt(voltageScale * scale);
      if (channelValue<METER_LOW) {
        channelValue = METER_LOW;
      }
      if (channelValue>METER_HIGH) {
        channelValue = METER_HIGH;
      }
      analogWrite(METER_OUT_PIN, channelValue);
      
/*      Serial.write(0xa8);
      Serial.write(1);
      Serial.write(4);
      Serial.write(0); Serial.write(toInt(actualVoltage * 10.0f));
      Serial.write(0); Serial.write(toInt(movingAverage * 10.0f));
      Serial.write(0); Serial.write(channelValue);
      Serial.write(0); Serial.write(channelValue);
      Serial.write(0x42); Serial.write(0x42); */
    }
  }
}

