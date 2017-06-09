/*
be nam _e_ khoda


Project Description : We are going to make a Carrier Signal on the Frequency of 1MHz
					  and modulate the song of EY IRAN on this signal and broadcast it 
					  on the AM Radio on Frequency 1 MHz

Platform : AVR Atmega328P

Fuse Bits : ## Mega 48, 88, 168, 328 default values
			LFUSE = 0x62
			HFUSE = 0xdf
			EFUSE = 0x00
					  
					  
CopyRight : In this project, I have used the AM Radio project from the book "Make : AVR Programming" by Elliot Williams
			and I have also used the EY IRAN song project by Mr.Keynejad (www.knowledgeplus.ir)

			Combining these two projects, I have made this project that broadcasts the song of EY IRAN on AM Radio (1MHz)

*/

// ------- Preamble -------- //
#include <avr/io.h>                        /* Defines pins, ports, etc */
#include <util/delay.h>                     /* Functions to waste time */
#include <avr/power.h>
#include <avr/interrupt.h>
#include "notes.h"

#define ANTENNA                 PD5                            /* OC0B */
#define ANTENNA_PORT            PORTD
#define ANTENNA_PIN             PIND
#define ANTENNA_DDR             DDRD

#define COUNTER_VALUE   3              /* determines carrier frequency */

volatile unsigned char duration;

unsigned char notes[]={re2,d4,la2,d1,sil,d1,la2,d1,sil,d1,la2,d4,
re2,d3,sil,d1,re2,d4,la2,d1,sil,d1,la2,d1,sil,d1,la2,d4,re2,d4,la2,d4,
la2_,d2,do3,d2,la2,d8,re3,d2,do3,d1,sil,d1,do3,d1,sil,d1,la2_,d1,sil,d1,
la2_,d1,sil,d1,la2,d1,sil,d1,la2,d1,sil,d1,sol2,d2,fa2,d4,sol2,d2,do3,d2,
la2,d8,re2,d2,do2,d2,re2,d2,re2_,d2,fa2,d2,re2_,d2,fa2,d2,sol2,d2,la2,d2,
do3,d2,la2_,d2,la2,d2,sol2,d2,fa2,d2,re2_,d2,re2,d2,do2,d4,sol2,d1,sil,d1,
sol2,d1,sil,d1,sol2,d4,la2,d4,la2_,d4,la2,d2,sol2,d2,fa2,d4,sol2,d4,la2,d4,
sol2,d2,fa2,d2,re2_,d4,fa2,d4,sol2,d4,fa2,d2,re2_,d2,re2,d8,la2_,d2,la2,d1,
sil,d1,la2,d1,sil,d1,sol2,d1,sil,d1,sol2,d1,sil,d1,fa2,d1,sil,d1,fa2,d1,
sil,d1,re2_,d2,la2,d2,sol2,d1,sil,d1,sol2,d1,sil,d1,fa2,d1,sil,d1,fa2,d1,
sil,d1,re2_,d1,sil,d1,re2_,d1,sil,d1,re2,d2,fa2,d2,sil,d2,fa2,d2,sol2,d2,
la2,d8,re2,d4,la2,d1,sil,d1,la2,d1,sil,d1,la2,d4,re2,d3,sil,d1,re2,d4,
la2,d1,sil,d1,la2,d1,sil,d1,la2,d4,re2,d4,la2,d4,la2_,d2,do3,d2,la2,d8,
re3,d2,do3,d1,sil,d1,do3,d1,sil,d1,la2_,d1,sil,d1,la2_,d1,sil,d1,la2,d1,
sil,d1,la2,d1,sil,d1,sol2,d2,fa2,d4,sol2,d2,do3,d2,la2,d8,re2,d2,do2,d2,
re2,d2,re2_,d2,fa2,d2,re2_,d2,fa2,d2,sol2,d2,la2,d2,do3,d2,la2_,d2,la2,d2,
sol2,d2,fa2,d2,re2_,d2,re2,d2,do2,d4,sol2,d1,sil,d1,sol2,d1,sil,d1,sol2,d4,
la2,d4,la2_,d4,la2,d2,sol2,d2,fa2,d4,sol2,d4,la2,d4,sol2,d2,fa2,d2,re2_,d4,
fa2,d4,sol2,d4,fa2,d2,re2_,d2,re2,d8,la2_,d2,la2,d1,sil,d1,la2,d1,sil,d1,
sol2,d1,sil,d1,sol2,d1,sil,d1,fa2,d1,sil,d1,fa2,d1,sil,d1,re2_,d2,la2,d2,
sol2,d1,sil,d1,sol2,d1,sil,d1,fa2,d1,sil,d1,fa2,d1,sil,d1,re2_,d1,sil,d1,
re2_,d1,sil,d1,re2,d2,re2,d4,re2_,d2,fa2,d2,re2,d6,sil,d2,re2,d8,la2,d8,
re2,d8,la2,d7,sil,d1,la2,d6,la2_,d2,do3,d6,la2_,d2,la2,d8,sil,d8,re3,d8,
do3,d8,la2_,d8,la2,d8,sol2,d6,fa2,d2,sol2,d6,do3,d2,la2,d8,sil,d8,re2,d8,
la2_,d8,la2,d8,sol2,d8,fa2,d6,re2_,d2,re2,d6,re2_,d2,do2,d8,sil,d8,fa2,d8,
re2_,d8,re2,d6,sol2,d2,fa2,d8,re2_,d6,re2,d2,do2,d6,re2_,d2,re2,d4,la1,d4,
la1_,d4,do2,d4,re2,d4,sol1,d4,do2,d4,sol1,d4,la1_,d4,re2,d4,la1,d4,re2,d4,
sol1,d4,re2,d4,la1,d4,re2,d4,la1_,d4,re2,d4,do2,d4,re2,d4,re3,d32,do3,d6,
re3_,d2,re3,d6,do3,d2,la2_,d6,re3,d2,do3,d6,la2_,d2,la2,d6,do3,d2,la2_,d6,
la2,d2,sol2,d8,sil,d8,sol1,d6,la1,d2,la1_,d6,do2,d2,re2,d6,re2_,d2,do2,d6,
re2,d2,re2_,d6,do2,d2,re2,d6,re2_,d2,re2,d8,sil,d8,do3,d8,si2,d6,re3,d2,
do3,d16,la2_,d6,la2,d2,sol2,d6,la2_,d2,la2,d16,do3,d8,si2,d6,re3,d2,
do3,d16,la2_,d6,la2,d2,sol2,d6,la2_,d2,la2,d16,fa2,d6,re2_,d2,fa2,d6,
sol2,d2,la2,d16,la2_,d6,la2,d2,sol2,d6,fa2,d2,la2,d6,sol2,d2,fa2,d6,
re2_,d2,sol2,d6,fa2,d2,re2_,d6,fa2,d2,sol2,d16,la2,d6,sol2,d2,fa2,d6,
re2_,d2,sol2,d6,fa2,d2,re2_,d6,re2,d2,fa2,d6,re2_,d2,sol2,d6,re2_,d2,
re2,d8,sil,d8};        

unsigned int timer1_values[]={1516,1431,1350,1275,1203,1135,1072,1011,955,901,850,803,757,
715,675,637,601,567,535,505,477}; //...

unsigned int current_note = sizeof(notes) - 1;

unsigned char note_f;


static inline void initTimer0(void) {
  TCCR0A |= (1 << WGM01);                                  /* CTC mode */
  TCCR0A |= (1 << COM0B0);            /* Toggles pin each time through */
  TCCR0B |= (1 << CS00);              /* Clock at CPU frequency, ~8MHz */
  OCR0A = COUNTER_VALUE;                          /* carrier frequency */
}

static inline void initTimer1(void) {
  TCCR1B |= (1 << WGM12);                                  /* CTC mode */
  TCCR1B |= (1 << CS11);            /* Clock at CPU/8 frequency, ~1MHz */
  TIMSK1 |= (1 << OCIE1A);          /* enable output compare interrupt */
}

ISR(TIMER1_COMPA_vect) {                 /* ISR for audio-rate Timer 1 */
	//if(note_f != 0){ 
		ANTENNA_DDR ^= (1 << ANTENNA); /* toggle carrier on and off */
	//}
}

static inline void transmitBeep(uint16_t pitch, uint16_t duration) {
  OCR1A = pitch;                               /* set pitch for timer1 */
  
  if(pitch == sil){
	  note_f = 0;
  }
  
  else{
	  note_f = 1;
  }
  
  sei();                                         /* turn on interrupts */
  do {
    _delay_ms(1);                            /* delay for pitch cycles */
    duration--;
  } while (duration > 0);
  cli();                  /* and disable ISR so that it stops toggling */
  
  ANTENNA_DDR |= (1 << ANTENNA);               /* back on full carrier */
}

int main(void) {
  // -------- Inits --------- //

  clock_prescale_set(clock_div_1);                  /* CPU clock 8 MHz */
  initTimer0();
  initTimer1();

  // ------ Event loop ------ //
  while (1) {
    
	current_note = current_note + 2;
	
	if(current_note >= sizeof(notes)){
		current_note = 0;
		_delay_ms(2500);
	}
	
    transmitBeep(timer1_values[notes[current_note]-1], notes[current_note+1] * 50);
	_delay_ms(100);
	
  }                                                  /* End event loop */
  return 0;                            /* This line is never reached */
}

