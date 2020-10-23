/*
 * main.c
 *
 *  Created on: 19 Oct 2020
 *      Author: kenawy7
 */


#include "avr/io.h"
#include <util/delay.h>

int main(void)
{
	//PWM
	TCCR1A |= 0xA1;
	TCCR1B |= 0x0C;
	OCR1A = 100;
	OCR1B = 100;
	DDRD |= 0x30;

	DDRC |= 0x0F;
	PORTC |= 0x09;

	//IR
	DDRB &= ~0x03;

	//US
	DDRA |= 0x01;
	DDRA &= ~0x02;

	while(1)
	{
		unsigned char x = 0;
		unsigned char y = 0;

		if(PINB & 0x01)
		{
			PORTC = 0x01;
			OCR1A = 0;
			OCR1B = 100;
		}
		else if(PINB & 0x02)
		{
			PORTC = 0x08;
			OCR1A = 100;
			OCR1B = 0;
		}
		else
		{
			PORTA |= 0x01;
			_delay_us(10);
			PORTA &= ~0x01;
			while(!(PINA & 0x02));
			TCNT2 = 0;
			TIFR |= 0x40;
			TCCR2 |= 0x02;
			while(PINA & 0x02)
			{
				if(TIFR & 0x40)
				{
					x++;
					TIFR |= 0x40;
				}
			}
			TCCR2 &= ~0x02;
			y = TCNT2;
			y = ((y + (x*256)) / 2) / 58;

			if(y<30)
			{
				OCR1A = 0;
				OCR1B = 0;
			}
			else
			{
				OCR1A = 100;
				OCR1B = 100;
			}
		}
	}
	return 0;
}
