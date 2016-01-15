
#include <EEPROM.h>

void setupConfigDefault() {
  for(int i=0; i<MAX_CHANCOUNT; i++) {
    stickConfig[i].enabled=false;
    stickConfig[i].pinIndex=0;
    stickConfig[i].enablePinIndex = -1;
    stickConfig[i].isDigital=false;
    stickConfig[i].isReversed=false;
    stickConfig[i].isSpringCentered=true;
    stickConfig[i].calibLow=0;
    stickConfig[i].calibMid=511;
    stickConfig[i].calibHigh=1023;
    stickConfig[i].defaultPos=0.5f;
  }
}

void setupCurrentConfig() {
  for(int i=0; i<MAX_CHANCOUNT; i++) {
    if (stickConfig[i].enabled) {
      if (stickConfig[i].enablePinIndex>=0) {
        pinMode(stickConfig[i].enablePinIndex, OUTPUT);
      }
      if (stickConfig[i].isDigital) {
        pinMode(stickConfig[i].pinIndex, INPUT_PULLUP);
      }
    }
  }
}

void addChecksum(int& checksum, void* atAddress, int howMany) {
  byte* ptr = (byte*)atAddress;
  for(int i=0; i<howMany; i++) {
    checksum = (checksum << 1) ^ *ptr++;
  }
}

void eepromPush(int& address, int& checksum, int intValue) {
  EEPROM.put(address, intValue);
  addChecksum(checksum, &intValue, sizeof(intValue));
  address += sizeof(intValue);
}

void eepromPush(int& address, int& checksum, StickConfig stickConfig) {
  EEPROM.put(address, stickConfig);
  addChecksum(checksum, &stickConfig, sizeof(StickConfig));
  address += sizeof(StickConfig);
}

void saveConfig() {
  int checksum = 0;
  int address = 0;
  eepromPush(address, checksum, 0xcc01); //version
  eepromPush(address, checksum, MAX_CHANCOUNT); // channel count
  
  for(int i=0; i<MAX_CHANCOUNT; i++) {
    eepromPush(address, checksum, stickConfig[i]);
  }
  eepromPush(address, checksum, checksum);
}

bool loadConfig() {
 
  int checksum = 0;
  int address = 0;
  int dataInt = 0;
  byte dataByte = 0;
  
  EEPROM.get(address, dataInt);
  address += sizeof(int);
  if (dataInt != 0xcc01) {
    return false;
  }
  addChecksum(checksum, &dataInt, sizeof(int));
  
  EEPROM.get(address, dataInt);
  address += sizeof(int);
  if (dataInt != MAX_CHANCOUNT) {
    return false;
  }
  addChecksum(checksum, &dataInt, sizeof(int));
  
  for(int i=0; i<(MAX_CHANCOUNT * sizeof(StickConfig)); i++) {
    EEPROM.get(address++, dataByte);
    addChecksum(checksum, &dataByte, 1);
  }

  EEPROM.get(address, dataInt);
  if (checksum!=dataInt) {
    return false;
  }

  address = 2 * sizeof(int);
  for(int i=0; i<MAX_CHANCOUNT; i++) {
    EEPROM.get(address, stickConfig[i]);
    address += sizeof(StickConfig);
  }
  return true;  
}

