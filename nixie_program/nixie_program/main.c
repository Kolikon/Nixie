#include <avr/io.h>
#define F_CPU 16000000UL
#include "util/delay.h"

const int PRZERWA = 500;


void T1delay(int ile)
{
	TCNT1= 65536 - ile;
	while( (TIFR1 & 1) == 0);
	TIFR1= 1;
}


int main(void)
{
	// 1-out 0-in
	DDRA= 0b00000000;   //in
	DDRB= 0b11111111;   //out min
	DDRC= 0b11111111;   //out sec
	DDRD= 0b11111111;   //out hour

	PORTA= 0b11111111;  //pull-up
	
	TCCR1B= 0b00000101; //prescaler= 1024
	

	unsigned char h, m, s;
	unsigned char ust[3] = {23, 59, 59};

	// ustawienie godziny
	unsigned char wybrany= 0;

	while (1)
	{
		while(1)
		{
			_delay_ms(PRZERWA);
			PORTD= ust[0]%10 + (ust[0]/10)*16;
			PORTB= ust[1]%10 + (ust[1]/10)*16;
			PORTC= ust[2]%10 + (ust[2]/10)*16;

			switch(wybrany)
			{
				case 0 :
				PORTD= 0;
				_delay_ms(PRZERWA);
				break;
				case 1 :
				PORTB= 0;
				_delay_ms(PRZERWA);
				break;
				case 2 :
				PORTC= 0;
				_delay_ms(PRZERWA);
				break;
			}

			if(PINA == 0b11111100)
			{
				_delay_ms(PRZERWA);
				break;
			}

			if(PINA == 0b11111110)
			{
				wybrany= (wybrany + 1)  % 3;
				_delay_ms(PRZERWA);
			}

			if(PINA == 0b11111101)
			{
				if(!wybrany)
				{
					ust[wybrany]= (ust[wybrany] + 1) % 24;
				} else
				{
					ust[wybrany]= (ust[wybrany] + 1) % 60;
				}
				_delay_ms(PRZERWA);
			}
		}

		// odtad leci
		while(1)
		{
			if(PINA == 0b11111100)
			{
				_delay_ms(PRZERWA);
				break;
			}
			
			for(h= ust[0]; h < 24; h++)
			{
				PORTD= h%10 + (h/10)*16;
				for(m= ust[1]; m < 60; m++)
				{
					PORTB= m%10 + (m/10)*16;
					for(s= ust[2]; s < 60; s++)
					{
						PORTC= s%10 + (s/10)*16;
						T1delay(15624);
					}
					ust[2]= 0;
				}
				ust[1]= 0;
			}
			ust[0]= 0;
			s=m=h=0;
		}
	}
}
