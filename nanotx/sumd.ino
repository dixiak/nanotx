
#define SUMD_HEAD 0xa8
#define SUMD_FRAMESIZE (3 + 32 + 2)

byte sumdFrame[SUMD_FRAMESIZE];

volatile bool serialDataReady = false;

void CRC16_add(uint8_t c, uint16_t& CRC16_value)
{
  uint8_t i;
  CRC16_value ^= (uint16_t)c << 8;
  for (i = 0; i < 8; i++) {
    if (CRC16_value & 0x8000) {
      CRC16_value = (CRC16_value << 1) ^ 0x1021;
    } else {
      CRC16_value = (CRC16_value << 1);
    }
  }
}

void fillSumd()
{
  if (SUMD_ENABLE) {
    if (!serialDataReady) {
      int channelCount = min(16, MAX_CHANCOUNT);
      uint16_t checksum = 0;
      bool inDefaultPosition = true;
      sumdFrame[0] = SUMD_HEAD;
      sumdFrame[1] = 0x01;
      sumdFrame[2] = channelCount;
      for (int i = 0; i < channelCount; i++) {
        setSumdChannel(i, readStick(i, inDefaultPosition));
      }
      for (int i = 0; i < (3 + (channelCount * 2)); i++) {
        CRC16_add(sumdFrame[i], checksum);
      }
      sumdFrame[3 + (channelCount * 2)] = checksum >> 8;
      sumdFrame[3 + (channelCount * 2) + 1] = checksum & 0xff;
      serialDataReady = true;
    }
  }
}

void setSumdChannel(int channelIndex, float channelValue) {
  // 7200 = extended low 900us
  // 16800 = extended high 2100us
  // 12000 = mid 1500us
  // 8000 = norm low 1000us
  // 16000 = norm high 2000us

  channelValue = 0.5f + ((channelValue - 0.5f) * (1.0f + SUMD_EDGE_OVERSHOOT));
  int chanIntValue = 8000 + toInt(channelValue * 8000.0f);
  sumdFrame[(channelIndex * 2) + 3] = 0xff & (chanIntValue >> 8);
  sumdFrame[(channelIndex * 2) + 4] = 0xff & (chanIntValue);
}

int getSumdChannel(int channelIndex) {
  int chanValue = 0;
  chanValue += sumdFrame[(channelIndex * 2) + 3] << 8;
  chanValue += sumdFrame[(channelIndex * 2) + 4];
  return chanValue;
}

void sumdAsDebug() {
//    for(int i=0; i<32; i++) {
//      Serial.print(ibusFrame[i], HEX);
//    }

  for (int i=0; i<16; i++) {
    Serial.print(getSumdChannel(i));
    Serial.print(" ");
  }
  Serial.println();
}

void sendSumdFrameIfReady() {
  if (SUMD_ENABLE) {
    if (serialDataReady) {  
      if (SUMD_AS_DEBUG) {
        sumdAsDebug();
      } else {
        int bytesToSend =  3 + 2 + (min(16, MAX_CHANCOUNT) * 2);
        for(int i=0; i<bytesToSend; i++) {
          Serial.write(sumdFrame[i]);
        }
      }
      serialDataReady = false;
    }
  }
}

