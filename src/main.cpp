#include <Arduino.h>
#include "layers.h"
#include "frames.h"
#include <IRremote.h>

const int remotePin = 12;
IRrecv ir(remotePin);
decode_results adresa;

bool cubeState = true;
long long frameTime = 0;
long long stateTime = 0;

long delayTime;

int currentAnim;
int maxAnims;

int currentFrame;
// max frame is when it hits 0 or maximum number of frames

int repeatCount;
int maxRepeatCount;

void nextAnim(){
	currentAnim++;
	if(currentAnim > maxAnims)
		currentAnim = 0;

	currentFrame = 1;

	repeatCount = 0;
	maxRepeatCount = anims[currentAnim][0] & 0b11111111;
	delayTime = anims[currentAnim][0] >> 8;
}

void setup() {
	Serial.begin(9600);
	initLayers();
	ir.enableIRIn();

	maxAnims = sizeof(anims)/sizeof(anims[0]);

	currentAnim = -1;
	nextAnim(); // initialize anims
}

void loop() {
	if(cubeState)
		if(millis() - frameTime > delayTime) { // 50 ms between frames
			frameTime = millis();

			currentFrame++;
			if(currentFrame >= 40 || anims[currentAnim][currentFrame] == 0) {
				currentFrame = 1;
				repeatCount++;
			}
			if(repeatCount >= maxRepeatCount)
				nextAnim();
		}

	if (ir.decode(&adresa)) { //
		Serial.print("Received: ");
		Serial.println(adresa.value,HEX);

		switch(adresa.value%16){

			case 12: // on/off
				if(millis()-stateTime < 500) break;
				stateTime = millis();
				cubeState = !cubeState;
				currentFrame = 1;
				if(cubeState==false) setFrame(0);
			break;

			default:
				cubeState = true;
				currentAnim = (adresa.value-1)%16 - 1;
				nextAnim();
			break;
		}    
		ir.resume();
	}

	if(cubeState) setFrame(anims[currentAnim][currentFrame]);
}