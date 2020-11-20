/*
 * main.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Przmeyslaw Grabowsky
 */
//#include <stdlib.h>
//#include <stdio.h
#include <avr/io.h>
//#define F_CPU 1000000
#include <util/delay.h>

int main(void){
	//ustawienie lini 0 i 1 wporcie B jako wyjsciowe
	//DDRB = 0x03;
	//DDRB = (1 << DDB0) | (1 << DDB1);

	uint8_t firstDigit = 0x7F;
	uint8_t secondDigit = 0x7F;
	uint8_t forthDigit = 0x7F;
	uint8_t thirdDigit = 0x7F;
	uint8_t buffor = 0xC0;
	uint8_t changeFlag = 0;
	uint8_t prevStateFlag = 0;

	//lines of a port D on a keyboard
	DDRD = 0xF0;
	//setting lines of port D
	PORTD = 0xFF;

	//setting port B as an output
	DDRB = 0xFF;

	//setting port C as an output
	DDRC = 0xFF;

	//set a counter to switch between displays
	int cnt_4 = 0;

	for(;;){

		//set a default digit to show
		if(0 != changeFlag && 0 == prevStateFlag){
			forthDigit = thirdDigit;
			//_delay_ms(50);
			thirdDigit = secondDigit;
			//_delay_ms(50);
			secondDigit = firstDigit;
			//_delay_ms(50);
			firstDigit = buffor;
			changeFlag = 0;
			prevStateFlag = 1;
		}

		//counter to iterate thou digits
		cnt_4++;
		if(cnt_4>3) cnt_4=0;

		switch(cnt_4){
		  case 0:
		    {
		       //set display of a forth digit
		       PORTC = 0x01;
			   //show a forth digit
			   PORTB = forthDigit;
		       break;
		    }
		  case 1:
		    {
		       //set display of a third digit
		       PORTC = 0x02;
			   //show a third digit
			   PORTB = thirdDigit;
		       break;
		    }
		  case 2:
		    {
			   //set display of a second digit
		       PORTC = 0x04;
			   //show a second digit
			   PORTB = secondDigit;
		       break;
		    }
		  case 3:
		    {
			   //set di splay of a first digit
		       PORTC = 0x08;
			   //show a first digit
			   PORTB = firstDigit;
		       break;
		    }
		}

		//check buttons of first line on a keyboard
		PORTD = 0xEF;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)){
			buffor = 0xF9; //1
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 1)){
			buffor = 0xA4; //2
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 2)){
			buffor = 0xB0; //3
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 3)){
			buffor = 0x88; //A
			changeFlag = 1;
		}

		//check buttons of second line on a keyboard
		PORTD = 0xDF;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)){
			buffor = 0x99; //4
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 1)){
			buffor = 0x92; //5
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 2)){
			buffor = 0x82; //6
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 3)){
			buffor = 0x83; //B
			changeFlag = 1;
		}

		//checwerk buttons of third line on a keyboard
		PORTD = 0xBF;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)){
			buffor = 0xF8; //7
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 1)){
			buffor = 0x80; //8
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 2)){
			buffor = 0x90; //9
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 3)){
			buffor = 0xC6; //C
			changeFlag = 1;
		}

		//check buttons of forth line on a keyboard
		PORTD = 0x7F;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)){
			buffor = 0x8F; //*
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 1)){
			buffor = 0xC0; //0
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 2)){
			buffor = 0x89; //#
			changeFlag = 1;
		}
		if(bit_is_clear(PIND, 3)){
			buffor = 0xA1; //D
			changeFlag = 1;
		}

		if(1==changeFlag && 1==prevStateFlag) changeFlag = 0;
		else if(0==changeFlag && 1==prevStateFlag) prevStateFlag = 0;
	}
	return 0;
}
