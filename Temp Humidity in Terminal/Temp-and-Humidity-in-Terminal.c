/*
 * @author Jaden Foldesi <flodesi57@gmail.com>
 * 
 * This is code for a program that will find the temperature
 * and humidity in a room and display it in a terminal window
 * 
 * Needed supplies:
 * 
 * Raspberry Pi (rPi3 was used)
 * DHT11 sensor
 * Potentiometer
 * Wires 
 */

#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


#define max 85
#define dhtPin 3

int lcd;
int data[5] = {0, 0, 0, 0, 0};

void dht11Sensor()
{
	uint8_t count = 0;
	uint8_t lstate = HIGH;
	uint8_t j = 0, i;
	float f; 

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	pinMode(dhtPin, OUTPUT);
	digitalWrite(dhtPin, LOW);
	delay(18);

	digitalWrite(dhtPin, HIGH);
	delayMicroseconds(40);

	pinMode(dhtPin, INPUT);

	for (i = 0; i < max; i++)
	{
		count = 0;
		while (digitalRead(dhtPin) == lstate)
		{
			count++;
			delayMicroseconds(1);
			if (count == 255)
			{
				break;
			}
		}
		lstate = digitalRead(dhtPin);
		if (count == 255)
			break;

		if ((i >= 4) && (i % 2 == 0))
		{
			data[j / 8] <<= 1;
			if (count > 16)
				data[j / 8] |= 1;
				j++;
		}
	}

	if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
	{
		printf("Temperature: %d.0 C Humidity: %d.%d %%\n", data[2], data[0], data[1]);

	}
}

int main(void)
{
	wiringPiSetup();

	while (1)
	{
		dht11Sensor();
		delay(1000); 
	}

	return(0);
}
