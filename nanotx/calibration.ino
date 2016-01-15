
#include <EEPROM.h>

#define CALIB_TIME_EDGES 16000
#define CALIB_TIME_CENTRE 2000
#define CALIB_READEVERY 10

int calibReading(int i, int j) {
    if (stickConfig[j].enablePinIndex>=0) {
      digitalWrite(stickConfig[j].enablePinIndex, HIGH);
    }
    int nowRead = analogRead(stickConfig[j].pinIndex);
    if (stickConfig[j].enablePinIndex>=0) {
      digitalWrite(stickConfig[j].enablePinIndex, LOW);
    }  
    
    if ((0==i) || (nowRead<stickConfig[j].calibLow)) {
      stickConfig[j].calibLow = nowRead;
    }    
    if ((0==i) || (nowRead>stickConfig[j].calibHigh)) {
      stickConfig[j].calibHigh = nowRead;
    }    
    return nowRead;
}

void calibrate() {
  
  long midTotals[MAX_CHANCOUNT];
  memset(midTotals, 0, sizeof(midTotals));

  Serial.println("Waiting.\n");
  Serial.read();
  
  Serial.println("Calibrating...\n");
  Serial.println("Measuring stick centre positions. Centre the sticks!\n");
  delay(1000);
  
  for (int i=0; i<CALIB_TIME_CENTRE/CALIB_READEVERY; i++) {
    for(int j=0; j<MAX_CHANCOUNT; j++) {
      if ((stickConfig[j].enabled) && (!stickConfig[j].isDigital) && stickConfig[j].isSpringCentered) {
        int nowRead = calibReading(i, j);
        midTotals[j] += nowRead;
      }
    }
    delay(CALIB_READEVERY);
  }

  for(int j=0; j<MAX_CHANCOUNT; j++) {
    if ((stickConfig[j].enabled) && (!stickConfig[j].isDigital) && (stickConfig[j].isSpringCentered)) {
      stickConfig[j].calibMid = midTotals[j]/(CALIB_TIME_CENTRE/CALIB_READEVERY);
      Serial.print(" ");
      Serial.print(j);
      Serial.print(" range ");
      Serial.print(stickConfig[j].calibLow);
      Serial.print("-");
      Serial.print(stickConfig[j].calibHigh);
      Serial.print(" final mid: ");
      Serial.println(stickConfig[j].calibMid);
    }
  }
  Serial.println("Measuring stick edges. Waggle the sticks!\n");
  
  for (int i=0; i<CALIB_TIME_EDGES/CALIB_READEVERY; i++) {
    for(int j=0; j<MAX_CHANCOUNT; j++) {
      if ((stickConfig[j].enabled) && (!stickConfig[j].isDigital)) {
        calibReading(i, j);
      }
    }
    delay(CALIB_READEVERY);
  }

  for(int j=0; j<MAX_CHANCOUNT; j++) {
    if ((stickConfig[j].enabled) && (!stickConfig[j].isDigital)) {
      int minSep = min(stickConfig[j].calibHigh - stickConfig[j].calibMid, stickConfig[j].calibMid - stickConfig[j].calibLow);
      int totalSep = stickConfig[j].calibHigh - stickConfig[j].calibLow;
      Serial.print(" ");
      Serial.print(j);
      Serial.print(" low: ");
      Serial.print(stickConfig[j].calibLow);
      if (stickConfig[j].isSpringCentered) {
        Serial.print(" mid: ");
        Serial.print(stickConfig[j].calibMid);
      } else {
        Serial.print(" no mid  ");
      }
      Serial.print(" high: ");
      Serial.print(stickConfig[j].calibHigh);
      if (totalSep<512) {
        Serial.print(" POOR RANGE");
      }
      if (stickConfig[j].isSpringCentered) {
        if (minSep<128) {
          Serial.print(" BAD SEPARATION");
        } else {
          if (minSep<256) {
            Serial.print(" POOR SEPARATION");
          }
        }
      }
      Serial.print("\n");
    }
  }
}

