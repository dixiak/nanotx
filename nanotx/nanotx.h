#include "config.h"

void debugPrintConfig();
void debugPrintSticks();

struct StickConfig {
  bool enabled;
  int pinIndex;
  int enablePinIndex;
  bool isDigital;
  bool isReversed;
  bool isSpringCentered;
  int calibLow;
  int calibMid;
  int calibHigh;
  float defaultPos;
};

StickConfig stickConfig[MAX_CHANCOUNT];

int toInt(float fractional) {
  return (int)(fractional + 0.5f);
}

void ppmSetup();
float readStick(int index, bool &inDefaultPos);
float readStickLastPosition(int index);
bool canArm();

void fillSumd();
void sendSbusFrameIfReady();

void mainLedService();
void mainLedModeOn();
void mainLedModeFastBlink();
void mainLedModeSlowBlink();
void mainLedFlicker();

void saveConfig();
bool loadConfig();
void setupConfigDefault();

void meterService();
void voltageRead();
