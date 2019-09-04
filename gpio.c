#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "gpio.h"

int analogRead(){
        char filename[] = "/sys/devices/pci0000:00/808622C1:02/i2c-0/i2c-ADC081C:00/iio:device0/in_voltage_raw\0";
        FILE *fp;
        char c[5];
	fp = fopen(filename, "r");
        if(fp == NULL)
        {
                perror("Error when opening file\n");
                exit(EXIT_FAILURE);
        }
	
	fgets(c, sizeof(c), fp);
        
	fclose(fp);

	return atoi(c);
}

int GPIOExport(int pin)
{
#define BUFFER_MAX 3
        char buffer[BUFFER_MAX];
        ssize_t bytes_written;
        int fd;

        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (-1 == fd) {
                fprintf(stderr, "Failed to open export for writing!\n");
                return(-1);
        }

        bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
        write(fd, buffer, bytes_written);
        close(fd);
        return(0);
}

int GPIOUnexport(int pin)
{
        char buffer[BUFFER_MAX];
        ssize_t bytes_written;
        int fd;

        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (-1 == fd) {
                fprintf(stderr, "Failed to open unexport for writing!\n");
                return(-1);
        }

        bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
        write(fd, buffer, bytes_written);
        close(fd);
        return(0);
}

int pinMode(int pin, int dir)
{
        static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
        char path[DIRECTION_MAX];
        int fd;

        snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
        fd = open(path, O_WRONLY);
        if (-1 == fd) {
                fprintf(stderr, "Failed to open gpio direction for writing!\n");
                return(-1);
        }

        if (-1 == write(fd, &s_directions_str[INPUT == dir ? 0 : 3], INPUT == dir ? 2 : 3)) {
                fprintf(stderr, "Failed to set direction!\n");
                return(-1);
        }

        close(fd);
        return(0);
}


int digitalRead(int pin)
{
#define VALUE_MAX 30
        char path[VALUE_MAX];
        char value_str[8];
        int fd;

        snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
        fd = open(path, O_RDONLY);
        if (-1 == fd) {
                fprintf(stderr, "Failed to open gpio value for reading!\n");
                return(-1);
        }

        int nbytes = sizeof(value_str);
        if (-1 == read(fd, value_str, nbytes)) {
                fprintf(stderr, "Failed to read value!\n");
                return(-1);
        }

        close(fd);
        printf("value: %x\n", *value_str);
        printf("String: %s\n", value_str);
        printf("Float: %f\n", atof(value_str));
        printf("Int: %d\n", atoi(value_str));

        return(atoi(value_str));
}


int digitalWrite(int pin, int value)
{
        static const char s_values_str[] = "01";

        char path[VALUE_MAX];
        int fd;

        snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
        fd = open(path, O_WRONLY);
        if (-1 == fd) {
                fprintf(stderr, "Failed to open gpio value for writing!\n");
                return(-1);
        }

        if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
                fprintf(stderr, "Failed to write value!\n");
                return(-1);
        }

        close(fd);
        return(0);
}

//returns 1 on successful operation -1 on failure
//calling function should check return value
int analogWrite(int pin, int value)
{

	if(pin != 12)
	{
		if(pin != 13)
		{
			perror("incorrect pin: not a PWM");
			return -1;
		}
	}

	if(value < 0 || value > 255)
	{
		perror("inccorect value out of range");
		return -1;
	}

	char *filename;

	if(pin == 12)
	{
		filename = "/sys/class/pwm/pwmchip0/pwm0/duty_cycle";
	}
	else
	{	
		filename = "/sys/class/pwm/pwmchip1/pwm0/duty_cycle";
	}

	float period = 3413333; //up board pwm example period
	
	float percentage = ((float)value / 255.0);
		
	float dutyCycle = period * percentage;

	int roundDC = round(dutyCycle);
	
	FILE *fp;
	fp = fopen(filename, "w");
	if(fp == NULL)
	{
		perror("Error opening file");
		return -1;
	}

	fprintf(fp, "%d", roundDC);

	fclose(fp);

	return 1;
}

long int micros(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double total = ((double)tv.tv_usec / 1000000 + (double)tv.tv_sec) * 1000000;
	long int final = total;
	return final;
}

long int millis(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double total = ((double)tv.tv_usec / 1000000 + (double)tv.tv_sec) * 1000;
	long int final = total;
	return final;

}


int map(int number, int fromLow, int fromHigh, int toLow, int toHigh) {
  number = (int)( ( (float)(number - fromLow) * ( (float)(toHigh - toLow) / (float)(fromHigh - fromLow) ) ) + (float)(toLow) );
  return number;
}

