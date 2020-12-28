/*
 * main.c
 *
 *  Created on: Nov 12, 2020
 *      Author: Przmeyslaw Grabowsky
 */ //notka 22.12.20 - jedyne co mi teraz przychodzi do pustej glowy to wstawic wyswietlanie cyfr do osbnego przerwania
//#include <stdlib.h>
//#include <stdio.h
#include <avr/interrupt.h>
#include <avr/io.h>
//#define F_CPU 1000000
#include <util/delay.h>

// declaratons of interrupts
//SR( TIMER1_OVF_vect);

ISR( PCINT2_vect );

ISR (BADISR_vect ) {}

// variables and flags
volatile uint8_t buffor = 0xC0;
volatile uint8_t changeFlag = 0;
volatile static uint8_t prev = 0x00;

volatile uint8_t prevStateFlag = 0;

void main(void){

    // variables of displayed digits
	uint8_t firstDigit = 0x7F;
	uint8_t secondDigit = 0x7F;
	uint8_t forthDigit = 0x7F;
	uint8_t thirdDigit = 0x7F;

	// cli();//stop interrupts; dont know if usaful

    // this likely useless with PCICR and PCMSK2
	DDRD = 0xF0; // lines of a port D on a keyboard (pins 0-3 inputs;4-7 outputs)
	PORTD = 0xDF;//0xFF; // setting lines of port D

	// those two are rather useful
	DDRB = 0xFF; 	// setting port B as an output
	DDRC = 0xFF; 	// setting port C as an output

	// konfiguracja PCINT2i
	PCICR |= (1<<PCIE2);
	//PCMSK2 |= (1<<PD7)|(1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3)|(1<<PD1)|(1<<PD2)|(1<<PD0);
	PCMSK2 |= (1<<PD3)|(1<<PD1)|(1<<PD2)|(1<<PD0);

    sei(); // switch on global interrupts

	int cnt_4 = 0; 	// set a counter to switch between displays

	for(;;){
		//  set a default digit to show
		cli();
		if(1 == changeFlag && 0 == prevStateFlag){
			forthDigit = thirdDigit;			//_delay_ms(50);
			thirdDigit = secondDigit;			//_delay_ms(50);
			secondDigit = firstDigit;			//_delay_ms(50);
			firstDigit = buffor; _delay_ms(100);
			PORTB ^= 0x80; //_delay_ms(50);

			changeFlag = 0;
			prevStateFlag = 1;
		}
			//cli();https://electronics.stackexchange.com/questions/165044/understanding-interrupts-and-software-button-debounce
			sei();
			//  counter to iterate thou digits
			cnt_4++;
			if(cnt_4>3) cnt_4=0;

			switch(cnt_4){
			  case 0:
			    {
			       PORTC = 0x01;		//  set display of a forth digit
				   PORTB = forthDigit;  //  show a forth digit
				   PORTD = 0xEF;
			       break;
			    }
			  case 1:
			    {
			       PORTC = 0x02;        //  set display of a third digit
				   PORTB = thirdDigit;  // show a third digit
				   PORTD = 0xDF;
			       break;
			    }
			  case 2:
			    {
			       PORTC = 0x04;  //  set display of a second digit
				   PORTB = secondDigit;  //  show a second digit
				   PORTD = 0xBF;
			       break;
			    }
			  case 3:
			    {
			       PORTC = 0x08; 	  //  set di splay of a first digit
				   PORTB = firstDigit;   //  show a first digit
				   PORTD = 0x7F;
			       break;
			    }
			}
			//sei();

		if(1==changeFlag && 1==prevStateFlag) changeFlag = 0;
		else if(0==changeFlag && 1==prevStateFlag) prevStateFlag = 0;
	}
}

//ISR( TIMER1_OVF_vect) {}

ISR( PCINT2_vect ){

	uint8_t current, changed;
	current = PIND; // get input state of portD as it has now changed
	changed = current ^ prev; // use XOR to find out which bit(s) have changed
  	//check buttons of first line on a keyboard
	//if(PIND & (0<<PD4)){
	if(0xDF == PIND){
		asm volatile("nop");

		if((changed & (1<<PIND0))){
			buffor = 0xF9; //1
			changeFlag = 1;
		}
		else if((changed & (1<<PIND1))){
			buffor = 0xA4; //2
			changeFlag = 1;
		}
		else if((changed & (1<<PIND2))){
			buffor = 0xB0; //3
			changeFlag = 1;
		}
		else if((changed & (1<<PIND3))){
			buffor = 0x88; //A
			changeFlag = 1;
		}
	}
		//check buttons of second line on a keyboard
	else if(0xBF == PIND){

		asm volatile("nop");

		if((changed & (1<<PIND0))){
			buffor = 0x99; //4
			changeFlag = 1;
		}
		else if((changed & (1<<PIND1))){
			buffor = 0x92; //5
			changeFlag = 1;
		}
		else if(changed & (1<<PIND2)){
			buffor = 0x82; //6
			changeFlag = 1;
		}
		else if((changed & (1<<PIND3))){
			buffor = 0x83; //B
			changeFlag = 1;
		}
	}
		//checwerk buttons of third line on a keyboard
	else if(0x7F == PIND){

		asm volatile("nop");

		if((changed & (1<<PIND0))){
			buffor = 0xF8; //7
			changeFlag = 1;
		}
		else if((changed & (1<<PIND1))){
			buffor = 0x80; //8
			changeFlag = 1;
		}
		else if((changed & (1<<PIND2))){
			buffor = 0x90; //9
			changeFlag = 1;
		}
		else if((changed & (1<<PIND3))){
			buffor = 0xC6; //C
			changeFlag = 1;
		}
	}
		//check buttons of forth line on a keyboard
	else if(0xEF == PIND){


		if((changed & (1<<PIND0))){
			buffor = 0x8F; //star
			changeFlag = 1;
		}
		else if((changed & (1<<PIND1))){
			buffor = 0xC0; //0
			changeFlag = 1;
		}
		else if((changed & (1<<PIND2))){
			buffor = 0x89; //#
			changeFlag = 1;
		}
		else if((changed & (1<<PIND3))){
			buffor = 0xA1; //D
			changeFlag = 1;
		}
	}
	prev = current;
}
