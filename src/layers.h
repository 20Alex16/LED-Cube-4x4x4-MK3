#include <Arduino.h>
#define d delay(1)

int latchPin = 5;
int clockPin = 4;
int dataPin = 3;

static void initLayers(){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  digitalWrite(latchPin, HIGH);

  pinMode(A0, OUTPUT); // layer 1
  pinMode(A1, OUTPUT); // layer 2
  pinMode(A2, OUTPUT); // layer 3
  pinMode(A3, OUTPUT); // layer 4
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
}

static void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
    int i=0;
    int pinState;
    // pinMode(myClockPin, OUTPUT);
    // pinMode(myDataPin, OUTPUT);
  
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);
  
    for (i=7; i>=0; i--)  {
      digitalWrite(myClockPin, 0);
  
      if ( myDataOut & (1<<i) ) {
        pinState= 1;
      }
      else {  
        pinState= 0;
      }
  
      digitalWrite(myDataPin, pinState);
      digitalWrite(myClockPin, 1);
      digitalWrite(myDataPin, 0);
    }
  
    digitalWrite(myClockPin, 0);
}

static void setLayer(byte b1, byte b2){
  digitalWrite(latchPin,LOW);
  shiftOut(3,4,b1); shiftOut(3,4,b2);
  digitalWrite(latchPin,HIGH);
}

static void activateLayer(int layer){
  digitalWrite(A0,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(A2,LOW);
  digitalWrite(A3,LOW);
  
  switch(layer){
    case 1: digitalWrite(A0, HIGH); break;
    case 2: digitalWrite(A1, HIGH); break;
    case 3: digitalWrite(A2, HIGH); break;
    case 4: digitalWrite(A3, HIGH); break;
    default: break; // do nothing
  };
}

static void setFrame(uint64_t frame){
  byte byte1 = frame >> 56;
  byte byte2 = frame >> 48 & 0xFF;
  byte byte3 = frame >> 40 & 0xFF;
  byte byte4 = frame >> 32 & 0xFF;
  byte byte5 = frame >> 24 & 0xFF;
  byte byte6 = frame >> 16 & 0xFF;
  byte byte7 = frame >> 8 & 0xFF;
  byte byte8 = frame & 0xFF;

  // byte1 = byte2 = byte3 = byte4 = byte5 = byte6 = byte7 = byte8 = 255;


  activateLayer(1);
  d;
  setLayer(byte1,byte2);
  d;
  setLayer(0,0);

  activateLayer(2);
  d;
  setLayer(byte3,byte4);
  d;
  setLayer(0,0);

  activateLayer(3);
  d;
  setLayer(byte5,byte6);
  d;
  setLayer(0,0);

  activateLayer(4);
  d;
  setLayer(byte7,byte8);
  d;
  setLayer(0,0);
}

static void testSetFrame(){
  digitalWrite(A0,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(A2,HIGH);
  digitalWrite(A3,HIGH);

  setLayer(255,255);
}