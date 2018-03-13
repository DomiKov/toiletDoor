// toiletdoor.c

// Code for the RPi. Sends out a request to the ATmega about
// the state of the door. 

// gcc -o toiletdoor toiletdoor.c -l bcm2835
// sudo ./toiletdoor

// Author: Dominik Kovacik

#include <unistd.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	 if (!bcm2835_init())
	 {
		 printf("bcm2835_init failed. Are you running as root??\n");
		 return 1;
	 }
	 if (!bcm2835_spi_begin())
	 {
		 printf("bcm2835_spi_begin failed. Are you running as root??\n");
		 return 1;
	 }
	 bcm2835_spi_begin();	// Switches the usual GPIO function to SPI
	 bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	 bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default => CPOL = 0, CPHA = 0
	 bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);   // The default => approx 1MHz.
	 bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // Using chip_select0 of the RPi
	 bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // Active low

	 uint8_t request = 0;
	 uint8_t doorState = 1;
	 //Send an empty byte
	 bcm2835_spi_transfer(request);
	 //Send another empty byte and set the doorStatus with the return from the AVR
	 doorState = bcm2835_spi_transfer(request);
	 switch (doorState)
	 {
		 case 0:
		 {
		 printf("The bathroom is free.\n");
		 break;
		}
		 case 1:
		 {
			 printf("The bathroom is occupied.\n");
			 break;
		 }
		 case 2:
		 {
			 printf("The bathroom is locked.\n");
			 break;
		 }
		 default:
		 {
			 printf("Error, unexpected value: %d\n", doorState);
		}
	 }
	 bcm2835_spi_end();
	bcm2835_close();
  //Return the value of the doorStatus. This could be later used for
  //Node-RED connetion for example.
	return doorState;
}

