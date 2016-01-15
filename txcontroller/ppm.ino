
#define PPM_FRAMELENGTH 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PULSELENGTH 300  //set the pulse length
#define PPM_HEADERLENGTH 500  //set the signal header

#define STATE_HI_HEADER 0
#define STATE_HI_SIGNAL 1
#define STATE_LO_STOP 2

int highState = STATE_HI_HEADER;
int channelIndex = -1;
int fillerPulseLength = PPM_FRAMELENGTH - (2000 * PPM_CHANCOUNT);
int channelValue;
int channelsNotInDefaultPosition;
int framesInDefaultPosition;

bool canArm() {  
  return framesInDefaultPosition > 10;
}

void ppmTimerSetup() {
  framesInDefaultPosition = 0;
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();  
}

int getChannelValue(int index) {
  int result = fillerPulseLength;
  bool inDefaultPos = true;
  if (-1 == index) {
    channelsNotInDefaultPosition = 0;
    fillerPulseLength = PPM_FRAMELENGTH - (PPM_HEADERLENGTH + PPM_PULSELENGTH);
  } else {
    result = 200 + toInt(readStick(index, inDefaultPos) * 1000.0f);    
    channelsNotInDefaultPosition += inDefaultPos? 0:1;
    fillerPulseLength -= PPM_HEADERLENGTH + result + PPM_PULSELENGTH;
  }
  return result;
}

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0;

  switch(highState) {
    
    // this segment is 500 microseconds long
    case STATE_HI_HEADER:
      if (PPM_ENABLE) {
        digitalWrite(PPM_PIN, PPM_POLARITY);
      }
      OCR1A = PPM_HEADERLENGTH * 2;
      highState = STATE_HI_SIGNAL;
      channelValue = getChannelValue(channelIndex);
      break;
      
    // this segment is between 200 microseconds and (22-8) millis long
    case STATE_HI_SIGNAL:
      //digitalWrite(PPM_PIN, PPM_POLARITY);
      OCR1A = channelValue * 2;
      highState = STATE_LO_STOP;
      if (channelIndex<0) {
        fillIbus();
      }
      break;    

    // this segment is 300 microseconds long
    case STATE_LO_STOP:
      if (PPM_ENABLE) {
        digitalWrite(PPM_PIN, !PPM_POLARITY);
      }
      OCR1A = PPM_PULSELENGTH * 2;
      highState = STATE_HI_HEADER;
      if ((++channelIndex)==PPM_CHANCOUNT) {
        channelIndex=-1;
        if (channelsNotInDefaultPosition>0) {
          framesInDefaultPosition = 0;
        } else {
          framesInDefaultPosition++;
        }
      }
      break;

    default:
      break;
  }
}

void ppmSetup(){  
  if (PPM_ENABLE) {
    pinMode(PPM_PIN, OUTPUT);
    digitalWrite(PPM_PIN, !PPM_POLARITY);  //set the PPM signal pin to the default state (off)
  }
  ppmTimerSetup();
}

