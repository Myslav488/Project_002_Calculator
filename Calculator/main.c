/*
 * main.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Przmeyslaw Grabowsky
 */
//#include <stdlib.h>
//#include <stdio.h>
#include <avr/io.h>
//#define F_CPU 1000000
//#include <util/delay.h>

int main(void){
	//ustawienie lini 0 i 1 wporcie B jako wyjsciowe
	//DDRB = 0x03;
	//DDRB = (1 << DDB0) | (1 << DDB1);

	uint8_t myDigit = 0x7F;

	//lines of a port D on a keyboard
	DDRD = 0xF0;
	//setting lines of port D
	PORTD = 0xFF;

	//setting port B as an output
	DDRB = 0xFF;

	//setting port C as an output
	DDRC = 0xFF;
	//set on the first part of a display
	PORTC = 0x01;

	for(;;){
		//show a digit
		PORTB = myDigit;
		//set a default digit to show
		myDigit = 0x7F;

		//check buttons of first line on a keyboard
		PORTD = 0xEF;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)) myDigit = 0xF9; //1
		if(bit_is_clear(PIND, 1)) myDigit = 0xA4; //2
		if(bit_is_clear(PIND, 2)) myDigit = 0xB0; //3
		if(bit_is_clear(PIND, 3)) myDigit = 0x88; //A

		//check buttons of second line on a keyboard
		PORTD = 0xDF;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)) myDigit = 0x99; //4
		if(bit_is_clear(PIND, 1)) myDigit = 0x92; //5
		if(bit_is_clear(PIND, 2)) myDigit = 0x82; //6
		if(bit_is_clear(PIND, 3)) myDigit = 0x83; //B

		//check buttons of third line on a keyboard
		PORTD = 0xBF;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)) myDigit = 0xF8; //7
		if(bit_is_clear(PIND, 1)) myDigit = 0x80; //8
		if(bit_is_clear(PIND, 2)) myDigit = 0x90; //9
		if(bit_is_clear(PIND, 3)) myDigit = 0xC6; //C

		//check buttons of forth line on a keyboard
		PORTD = 0x7F;

		asm volatile("nop");

		if(bit_is_clear(PIND, 0)) myDigit = 0x8F; //*
		if(bit_is_clear(PIND, 1)) myDigit = 0xC0; //0
		if(bit_is_clear(PIND, 2)) myDigit = 0x89; //#
		if(bit_is_clear(PIND, 3)) myDigit = 0xA1; //D

	}
	return 0;
}

