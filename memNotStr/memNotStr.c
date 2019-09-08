#include "gpio.h"
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>


void loop();

char setMe[200] = "splash";
char usefulString[20] = "/bin/sh";

void __attribute__ ((noinline)) pwnme(char *input, int size){
    char buf[8];
    memcpy(buf, input, size);
    return;
}

// Pins
int greenPin = 12; //physical 32 PWM
int bluePin = 13;  //physical 33 PWM
int aPin = 0;      //physical 27 GPIO
int bPin = 5;      //physical 29 GPIO
int cPin = 6;      //physical 31 GPIO
int dPin = 19;     //physical 35 GPIO
int ePin = 26;     //physical 37 GPIO
int sensorPin = 4; //physical 7 ADC

// Vars
int leds = 5;
int colors = 2;
int activeLED = 0;

// Times
int intervalTime = 1; // ms
int frameTime = 33; // ms
int lastIntTime;
int elapsedIntTime;
int lastFrameTime;
int elapsedFrameTime;
int thisTime;
int thisFrameTime;
bool firstIntTime = HIGH;
bool firstFrameTime = HIGH;

// Frame Array
int frame[5][3];

// Frame Vars
int onLED = 0;
int frameCount = 0;
int frameNumber = 15;
bool LEDdirection = HIGH;
int input;

int main() {

  // Define Initial Array
  frame[0][0] = aPin;
  frame[0][1] = 0;
  frame[0][2] = 0;
  frame[1][0] = bPin;
  frame[1][1] = 0;
  frame[1][2] = 0;
  frame[2][0] = cPin;
  frame[2][1] = 0;
  frame[2][2] = 0;
  frame[3][0] = dPin;
  frame[3][1] = 0;
  frame[3][2] = 0;
  frame[4][0] = ePin;
  frame[4][1] = 0;
  frame[4][2] = 0;

  // Configuration
  digitalWrite(aPin, HIGH);
  digitalWrite(bPin, HIGH);
  digitalWrite(cPin, HIGH);
  digitalWrite(dPin, HIGH);
  digitalWrite(ePin, HIGH);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);

  // Times
  lastIntTime = millis();
  lastFrameTime = millis();
  loop();
  return 0;
}

void loop() {
	char buffer[100], tooSmall[8];
    char c;
    int rv, bufSize = 32, count;
    char buf[bufSize];
    memset(buf, 0, 0x20); 
    struct pollfd ufds[1];
    ufds[0].fd = STDIN_FILENO;
    ufds[0].events = POLLIN | POLLRDNORM;
	while(1){
		rv = poll(ufds, 1, 0);
		if(rv == 1)
		{
			fgets(buffer, sizeof(buffer) - 1, stdin);
			count = 0;        
			c = buffer[count++]; //get flag and increment count
			if(c == '1')
			{
				memset(setMe, 0, sizeof(setMe)); //clear setMe
				c = buffer[++count]; //increment count and get beggining of setMe string
			
				while(c != '\n')
				{
					setMe[count - 2] = c;
					c = buffer[++count];
				}

				pwnme(setMe, count - 1);
			}
			
			if(c == '2')
			{
				printf("%s\n", setMe);
			}
		}

		memset(buffer, 0, sizeof(buffer)); //clear buffer

		input = analogRead(sensorPin);
		
		// Update Clock
		thisTime = millis();
		thisFrameTime = millis();
		elapsedIntTime = thisTime - lastIntTime;
		elapsedFrameTime = thisFrameTime - lastFrameTime;

		// Update Frame
		frameTime = map(input, 0, 1023, 20, 3);
		if (elapsedFrameTime > frameTime) {
		frameCount++;
		if(frameCount >= frameNumber) {
			frameCount = 0;
			if (LEDdirection == HIGH) {
		onLED++;
		if (onLED > 4) {
			onLED = 4;
			LEDdirection = LOW;
		}
			}
			if (LEDdirection == LOW) {
		onLED--;
		if (onLED < 0) {
			onLED = 1;
			LEDdirection = HIGH;
		}
			}
			for (int i = 0; i < leds; i++) {
		if (i == onLED) {
			
			int greenVal;
			int blueVal;
			if (input < 150) {
			blueVal = 255;
			} else {
			blueVal = map(input, 150, 255, 255, 0);
			}
			if (input > 100) {
			greenVal = 255;
			} else {
			greenVal = map(input, 0, 100, 0, 255);
			}
			
			frame[i][1] = greenVal;
			frame[i][2] = blueVal;
		} else {
			frame[i][1] = 0;
			frame[i][2] = 0;
		}
			}
		}
		
		lastFrameTime = thisFrameTime;
		}

		// Drive Active LED
		if (firstIntTime) {
		analogWrite(greenPin, frame[activeLED][1]);
		analogWrite(bluePin, frame[activeLED][2]);
		
		for (int i = 0; i < leds; i++) {
			if (i == activeLED) {
		digitalWrite(frame[i][0], LOW);
			} else {
		digitalWrite(frame[i][0], HIGH);
			}
		}

		firstIntTime = LOW;
		}

		// LED Cycle
		if (elapsedIntTime > intervalTime) {
		activeLED++;
		if (activeLED > 4) {
			activeLED = 0;
		}

		for (int i = 0; i < leds; i++) {
			digitalWrite(frame[i][0], HIGH);
		}
		
		lastIntTime = thisTime;
		firstIntTime = HIGH;
		}
  }
}

void usefulFunction(){
	system("/bin/ls");
	return;
}
