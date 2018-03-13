/*
 * Toilet door.c
 *
 * Created: 3/5/2018 5:26:17 PM
 * Author : Dominik
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR (SPI_STC_vect);

enum evnt //Possible events
{
	E_NOTHING,
	E_CLOSED,
	E_OPEN,
	E_LOCK,
};

enum state //Possible states
{
	S_OPEN,
	S_CLOSED,
	S_LOCKED,
};

enum dataSend //Possible informations for RPi
{
	DOOR_OPEN,
	DOOR_CLOSED,
	DOOR_LOCKED
};
	
uint8_t doorStatus;

int event_change();
uint8_t stateMachine_change(unsigned int State, unsigned int Event);
unsigned int state_change(unsigned int State, unsigned int Event);
void pin_setup();
void spiSlave_init ();

int main(void)
{
	unsigned int Event;
	unsigned int State;
	//Initializes the pins
	pin_setup();
	//Initialize AVR as SPI slave
	spiSlave_init ();
	//Set the initial state as open
	State = S_OPEN;
	//Allow global interrupts. The AVR will be waiting for a request from RPi. 
	sei();
		
    while (1) 
    {
		Event = event_change(); //What event has occurred?
		doorStatus = stateMachine_change(Event, State); //Set the doorStaus with the current status of the door. 
		State = state_change(Event, State);	//Set the State to the current state of the door.
    }
}

uint8_t stateMachine_change(unsigned int Event, unsigned int State)
{
	uint8_t sendData = doorStatus;
	switch(State)
	{
		case S_OPEN:
		{
			switch(Event)
			{	
				case E_CLOSED:
				{
					//Door was open and somebody closed it
					PORTD |= (1<<PIND5); //Turn on the closed-door LED
					sendData = DOOR_CLOSED;
					break;
				}	
				case E_OPEN:
				{
					//Do nothing, as nothing has changed
					break;
				}
				case E_LOCK:
				{
					//Do nothing
					break;
				}
			}
		}
		case S_CLOSED:
		{
			switch(Event)
			{
				case E_OPEN:
				{
					//Doors were closed and somebody opened it
					PORTD &= ~(1<<PIND5); //Turn the closed-door LED off
					sendData = DOOR_OPEN;
					break;
				}
				case E_CLOSED:
				{
					//Do nothing as nothing has changed
					break;
				}
				case E_LOCK:
				{
					//Doors were closed and locked
					PORTD |= (1<<PIND4); // Turn on the lock-door LED
					sendData = DOOR_LOCKED;
					
				}
			}
		}
		case (S_LOCKED):
		{
			switch (Event)
			{
				case E_OPEN:
				{
					//do nothing
					break;
				}
				case E_CLOSED:
				{
					//Doors were unlocked => go back to closed
					PORTD &= ~(1<<PIND4); //Turn off the door-locked LED
					sendData = DOOR_CLOSED;
					break;
				}
				case E_LOCK:
				{
					//do nothing
					break;
				}
			}
		}
		
		default:{
			break;
		}
		
	}
	//return the data that will be sent to RPi over the SPI
	return sendData;
}

unsigned int state_change(unsigned int Event, unsigned int State)
{
	unsigned int newState = State;
	switch(State)
	{
		case S_OPEN:
		{
			switch(Event)
			{
				case(E_CLOSED):
				{
					//The doors were open and their state was changed to closed
					newState = S_CLOSED;
					break;
				}
				case(E_OPEN):
				{
					//The state does not change
					break;
				}
				case (E_LOCK):
				{
					//do nothing
					break;
				}
			}
			
		}
		case S_CLOSED:
		{
			switch(Event)
			{
				case(E_OPEN):
				{
					//The doors were closed and their state changed to opened
					newState = S_OPEN;
					break;
				}
				case (E_CLOSED):
				{
					//State did not change
					break;	
				}
				case (E_LOCK):
				{
					//The doors were closed and are now locked
					newState = S_LOCKED;
					break;
				}
			}
		}
		case (S_LOCKED):
			{
				switch (Event)
				{
					case E_OPEN:
					{
						//do nothing
						break;
					}
					case E_CLOSED:
					{
						//doors were unlocked => go back to closed
						newState = S_CLOSED;
						break;
					}
					case E_LOCK:
					{
						//do nothing
						break;
					}
				}
			}
		default:{
			break;
		}
	}
	return newState;
	
}

int event_change()
{
	int Event = E_NOTHING;
	while (Event == E_NOTHING)
	{
		if (PIND & (1<<PIND7)) //If the doors close
		{
			Event = E_CLOSED;
			if (PIND & (1<<PIND6)){ //if the doors are locked
				Event = E_LOCK;
			}
			//when unlocked
			else Event = E_CLOSED;
		}
		else//If the doors open
		{
			Event = E_OPEN;
		}
	}
	return Event;
}

void spiSlave_init (){
	//Allow interrupt and SPI
	SPCR |= (1<<SPIE) | (1<<SPE);
	//Set MISO as output
	DDRB |= (1<<PINB4);
}

void pin_setup(){
	// set digital pin 5 and 4 as output and set it to low
	DDRD |= (1<<PIND5) | (1<<PIND4);
	PORTD &= ~(1<<PIND5) | (1<<PIND4);
	// set digital pin 7 and 6 as input, external pull-down resistor necessary
	DDRD &= ~(1<<PIND7) | (1<<PIND6);
}

ISR (SPI_STC_vect){
	//ISR that sends the status of the door over the SPI
	SPDR = doorStatus;
}
