/*
 * @author Jaden Foldesi <flodesi57@gmail.com>
 * 
 * This is code for a program that will find the temperature
 * and humidity in a room and output it on an LCD Screen
 * 
 * Needed supplies:
 * 
 * Raspberry Pi (rPi3 was used)
 * DHT11 sensor
 * LCD Screen (I used a 1602 display)
 * Potentiometer
 * Wires 
 */

#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define rs  25
#define e   24
#define d4  23
#define d5  22
#define d6  21
#define d7  14
#define max 85
#define dhtPin 3

int lcd;
int data[5] = {0, 0, 0, 0, 0};

void dht11Sensor()
{
	uint8_t counter = 0;
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
		counter = 0;
		while (digitalRead(dhtPin) == lstate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
			{
				break;
			}
		}
		lstate = digitalRead(dhtPin);
		if (counter == 255)
			break;

		if ((i >= 4) && (i % 2 == 0))
		{
			data[j / 8] <<= 1;
			if (counter > 16)
				data[j / 8] |= 1;
				j++;
		}
	}

	if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)))
	{
		f = data[2] * 9. / 5. + 32;
		lcdPosition(lcd, 0, 0);
		lcdPrintf(lcd, "Humidity: %d.%d %%\n", data[0], data[1]);
		lcdPosition(lcd, 0, 1);
		lcdPrintf(lcd, "Temp: %d.0 C", data[2]);

	}
}

int main(void)
{
	int lcd;
	wiringPiSetup();
	lcd = lcdInit (2, 16, 4, rs, e, d4, d5, d6, d7, 0, 0, 0, 0);

	while (1)
	{
		dht11Sensor();
		delay(1000); 
	}

	return(0);
}
