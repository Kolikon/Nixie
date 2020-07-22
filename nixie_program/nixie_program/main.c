#include <avr/io.h>
#define F_CPU 16000000UL
#include "util/delay.h"

const int PRZERWA = 200;

void T1delay()
{
	while( (TIFR1 & 1) == 0);
	TIFR1= 1;
	TCNT1= 49912;
	
	return;
}

void wybor(unsigned char ust[3])
{
	unsigned char wybrany= 0;
	_delay_ms(PRZERWA);
	while(1)
		{
			switch(wybrany)
			{
				case 0 :
				{
					PORTC= 0xff;
					PORTA= 0xff;
					break;
				}
				case 1 :
				{
					PORTB= 0xff;
					break;
				}
				case 2 :
				{
					PORTD= 0xff;
					break;
				}
			}
			_delay_ms(PRZERWA/2);
			
			if( (PINA & 3) == 0)
			{
				_delay_ms(PRZERWA);
				return;
			}

			if( (PINA & 3) == 1)
			{
				wybrany= (wybrany + 1)  % 3;
			}

			if( (PINA & 3) == 2)
			{
				if(!wybrany)
				{
					ust[wybrany]= (ust[wybrany] + 1) % 24;
				} else
				{
					ust[wybrany]= (ust[wybrany] + 1) % 60;
				}
			}
			
			PORTD= ust[2]/10 + (ust[2]%10)*16;
			PORTB= ust[1]/10 + (ust[1]%10)*16;
			PORTC= ust[0]/10 + (ust[0]%10)*16;
			PORTA= ( (PORTC & 0b00111100) << 2 ) + 0b1111;
			_delay_ms(PRZERWA/2);
		}
	return;
}


int main(void)
{
	// 1-out 0-in
	DDRA= 0b11110000;   //in
	DDRC= 0b11111111;   //out hour
	DDRB= 0b11111111;   //out min
	DDRD= 0b11111111;   //out sec

	PORTA= 0b11111111;  //pull-up
	
	TCCR1B= 0b00000101; //prescaler= 1024
	
	unsigned char ust[3] = {12, 34, 56};
	
	PORTD= ust[2]/10 + (ust[2]%10)*16;
	PORTB= ust[1]/10 + (ust[1]%10)*16;
	PORTC= ust[0]/10 + (ust[0]%10)*16;
	PORTA= ( (PORTC & 0b00111100) << 2 ) + 0b1111;
	
	while(1)
	{
		if( (PINA & 3) == 0)
		{
			_delay_ms(PRZERWA);
			wybor(ust);
			PORTD= ust[2]/10 + (ust[2]%10)*16;
			PORTB= ust[1]/10 + (ust[1]%10)*16;
			PORTC= ust[0]/10 + (ust[0]%10)*16;
			PORTA= ( (PORTC & 0b00111100) << 2 ) + 0b1111;
		}
		
		ust[2]= (ust[2]+1)%60;
		PORTD= ust[2]/10 + (ust[2]%10)*16;
		
		if(ust[2] == 0)
		{
			ust[1]= (ust[1]+1) % 60;
			PORTB= ust[1]/10 + (ust[1]%10)*16;
		}
		
		if(ust[1] == 0 && ust[2] == 0)
		{
			ust[0]= (ust[0]+1) % 24;
			PORTC= ust[0]/10 + (ust[0]%10)*16;
			PORTA= ( (PORTC & 0b00111100) << 2 ) + 0b1111;
		}
		
		T1delay();
	}
}
