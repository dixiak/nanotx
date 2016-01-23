#include "nanotx.h"

bool armed;
bool noConfig = true;

void configure() {
  setupConfigDefault();
  
  stickConfig[0].enabled=true;
  stickConfig[0].pinIndex=3;
  stickConfig[0].enablePinIndex=3;
  stickConfig[0].isDigital=false;
  stickConfig[0].isReversed=false;
  stickConfig[0].isSpringCentered=true;

  stickConfig[1].enabled=true;
  stickConfig[1].pinIndex=2;
  stickConfig[1].enablePinIndex=2;
  stickConfig[1].isDigital=false;
  stickConfig[1].isReversed=false;
  stickConfig[1].isSpringCentered=true;

  stickConfig[2].enabled=true;
  stickConfig[2].pinIndex=4;
  stickConfig[2].enablePinIndex=4;
  stickConfig[2].isDigital=false;
  stickConfig[2].isReversed=false;
  stickConfig[2].isSpringCentered=false;
  stickConfig[2].defaultPos=0.0f;

  stickConfig[3].enabled=true;
  stickConfig[3].pinIndex=5;
  stickConfig[3].enablePinIndex=5;
  stickConfig[3].isDigital=false;
  stickConfig[3].isReversed=false;
  stickConfig[3].isSpringCentered=true;

  stickConfig[4].enabled=true;
  stickConfig[4].pinIndex=8;
  stickConfig[4].isDigital=true;
  stickConfig[4].isReversed=true;
  stickConfig[4].defaultPos=1.0f;
  
  stickConfig[5].enabled=true;
  stickConfig[5].pinIndex=9;
  stickConfig[5].isDigital=true;
  stickConfig[5].isReversed=true;
  stickConfig[5].defaultPos=1.0f;

  stickConfig[6].enabled=true;
  stickConfig[6].pinIndex=10;
  stickConfig[6].isDigital=true;
  stickConfig[6].isReversed=true;
  stickConfig[6].defaultPos=1.0f;
}

void xsetup() {
  Serial.begin(115200);
  configure();
  setupCurrentConfig();
  calibrate();
  saveConfig();
  debugPrintConfig();
  Serial.println("Saved");
  if (loadConfig()) {
    Serial.println("Loaded OK!");
    debugPrintConfig();
  }
}

void setup() {
  armed = false;
  if (!loadConfig()) {
    noConfig = true;
    setupConfigDefault();
    mainLedModeFastBlink();
  } else {    
    mainLedModeSlowBlink();
  }  
  setupCurrentConfig();
  pinMode(MAIN_LED_PIN, OUTPUT);  
  Serial.begin(115200);
  ppmSetup();
}

void loop() {
  sendSumdFrameIfReady();
  mainLedService();
  if (!armed && canArm()) {
    armed = true;
    mainLedModeOn();
  }
  if (Serial.available()) {
    debugPrintConfig();
    Serial.read();
  }
  //debugPrintSticks();
}

