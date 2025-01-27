// Interrupcion PCINT


#include <avr/io.h>
#define F_CPU 1000000UL
#include <avr/interrupt.h>
#include <util/delay.h>

#define BTN_INT0 PIND2 

#define LED0 PORTB0
#define LED1 PORTB1
#define LED2 PORTB2
#define LED3 PORTB3

#define LED0_ON		PORTB |=  _BV(LED0)
#define LED0_OFF	PORTB &= ~_BV(LED0)

#define LED1_ON		PORTB |=  _BV(LED1)
#define LED1_OFF	PORTB &= ~_BV(LED1)

#define LED2_ON		PORTB |=  _BV(LED2)
#define LED2_OFF	PORTB &= ~_BV(LED2)

#define LED3_ON		PORTB |=  _BV(LED3)
#define LED3_OFF	PORTB &= ~_BV(LED3)

void init_ports();
void init_int();

volatile uint8_t flag = 0;

const int TIME = 500;

int main(void)
{
	cli();
	init_ports();
	init_int();
	flag = 0;
	sei();

	while (1)
	{
		
		if ( flag == 1 )
		{
			LED0_ON;
			LED2_ON;
			LED1_OFF;
			LED3_OFF;
			_delay_ms(TIME);
			LED0_OFF;
			LED2_OFF;
			LED1_ON;
			LED3_ON;
			_delay_ms(TIME);
		}
		else if ( flag == 0 )
		{
			LED0_ON;
			LED1_ON;
			LED2_ON;
			LED3_ON;
			_delay_ms(TIME);
			LED0_OFF;
			LED1_OFF;
			LED2_OFF;
			LED3_OFF;
			_delay_ms(TIME);
		}
	}
}

// Inicializar puertos
void init_ports()
{
	// Entradas
	DDRD  &= ~( _BV(BTN_INT0) ); // Entrada - 0
	PORTD |=  ( _BV(BTN_INT0) ); // Pull up - HIGH No es necesario por el debounce en hardware
	
	// Salidas
	DDRB  |=  ( _BV(LED0) | _BV(LED1) | _BV(LED2) | _BV(LED3) );	// Salidas - 1
	PORTB &= ~( _BV(LED0) | _BV(LED1) | _BV(LED2) | _BV(LED3) );	// Salidas - Source - LOW
	
}

// Inicializar interrupciones
void init_int()
{
	// Nota: las PCINT s�lo distinguen cambios de tipo Change
	// Si queremos detectar flancos de subida o de bajada deberemos 
	// guardar el estado del registro en una variable y realizar la 
	// comparaci�n con el estado anterior en la ISR.
	
	// Inicializar PCICR
	PCICR |=  ( _BV(PCIE2) ); // Habilitar interrupci?n
	// PCICR - Pin Change Interrupt Control Register
		
	// Inicializar PCMSK2
	PCMSK2 |=  ( _BV(PCINT16) ); // Habilitar PCINT16
	// PCMSK2 - Pinc Change Mask 2

}

ISR(PCINT2_vect)
{

	flag = 1 - flag; // Toggle 1 y 0, cada vez que se presiona cambia el valor entre 1 y 0

}
// ISR	- Interruption Service Routine - Maneja el desbordamiento
// INTO_vect - External Interrupt Request 0