Hello,
my name is Przemyslaw Grabowski and this is my first project on AVR microcontroller.

As it's name may suggest it is a simple calculator, which receives some numbers and makes proper calculations. 

For this project I am using Atmega8A, 16-key keyboard, 4x seven-segment display and few LEDs with resitors.

The aim is to make a simple tool and learn a lot.

UPDATE Jan-07-2021
-> Since i tried to use PCINT interrupts for all PORT D pins I took a decision to replace a microcontoller with Atmega88PA.
It turns out interrupts are not supposed to be used for to check wich pin is pressed. It is due to debouncing of buttons and
others. Polling is better so I revert my code, by I will stay with the Atmega88PA just in case.

