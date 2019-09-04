#ifndef GPIO_H_
#define GPIO_H_


#define INPUT   0
#define OUTPUT  1

#define LOW  0
#define HIGH 1

#define PIN  4 /* physical pin 7 */
#define POUT 3  /* physical pin 5  */

int analogRead();
int GPIOExport(int pin);
int GPIOUnexport(int pin);
int pinMode(int pin, int dir);
int digitalRead(int pin);
int digitalWrite(int pin, int value);
int analogWrite(int pin, int value);
long int micros();
long int millis();
int map(int number, int fromLow, int fromHigh, int toLow, int toHigh);
#endif
