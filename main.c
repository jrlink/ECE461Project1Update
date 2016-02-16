#include "msp432.h"
#include "driverlib.h"
#include "string.h"

#define BUFFER_MEMORY_START0 0x0000
#define BUFFER_MEMORY_START1 0x02000
int BUFFER_MEMORY_START_COUNT = 0x0000;

int sectorcount = 0;
																							//Default clock
void port_init()
{
	P1DIR |= BIT0;																			// make P1.0 a RED LED
	P1DIR &= ~BIT4;																			// make P1.4 an input
	P1REN |= BIT4;																			// enable pull up resistor
	P1OUT |= BIT4;
	P2DIR |= BIT1;																			// make P2.1 a GREEN LED
	P1OUT &= ~BIT0;																			//turn off LEDS (green and red)
	P2OUT &= ~BIT1;


}
void delay_1sec()      																		// 1 second delay
{
	int  cnt;																				//counter
	for (cnt=0;cnt<58000;cnt++);															//Delay loop
	return;
}

void main(void)
{

	port_init();
	WDTCTL = WDTPW | WDTCNTCL;
	P1OUT |= BIT0;				// turn red LED on
	delay_1sec();				// wait 1 second
	P1OUT &= ~BIT0;				// turn red LED off
	WDTCTL = WDTPW | WDTCNTCL;
	sectorcount = 0;																		//making sure my offset is set to 0
	WDTCTL = WDTPW |WDTSSEL_3|WDTIS_4|WDTCNTCL;												//Watchdog timer set to expire every second
	WDTCTL = WDTPW | WDTCNTCL;

	MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
	WDTCTL = WDTPW | WDTCNTCL;

	while(1){
		WDTCTL = WDTPW | WDTCNTCL; //clear timer to prevent timeout
	if (!(P1IN & BIT4)){																	//check if timeout button is pushed
		P1OUT |= BIT0;																		//RED LED
		delay_1sec();
		WDTCTL = (WDTPW-1);																	//forces reset
	}

	uint8_t buffer[4096];
	memset(buffer,0xA5,4096);

	if(sectorcount < 16){
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
		MAP_FlashCtl_unprotectSector(FLASH_INFO_MEMORY_SPACE_BANK0,FLASH_SECTOR0<<sectorcount); //Unprotect current sector.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
		BUFFER_MEMORY_START_COUNT = BUFFER_MEMORY_START0+ (4096*sectorcount);					//Memory address for the next sector to be erased and programmed.

		MAP_FlashCtl_eraseSector(BUFFER_MEMORY_START_COUNT);									//Erase current sector.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.

		MAP_FlashCtl_programMemory(buffer,(void*)BUFFER_MEMORY_START_COUNT, 4096 ); 			//Program sectors.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
		MAP_FlashCtl_protectSector(FLASH_INFO_MEMORY_SPACE_BANK0,FLASH_SECTOR0<<sectorcount);	//Protect sectors.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
	}
	else {
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
		MAP_FlashCtl_unprotectSector(FLASH_INFO_MEMORY_SPACE_BANK1,FLASH_SECTOR0<<sectorcount); //Unprotect current sector.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
		BUFFER_MEMORY_START_COUNT = BUFFER_MEMORY_START1+ (4096*(sectorcount - 16));					//Memory address for the next sector to be erased and programmed.

		MAP_FlashCtl_eraseSector(BUFFER_MEMORY_START_COUNT);									//Erase current sector.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.

		MAP_FlashCtl_programMemory(buffer,(void*)BUFFER_MEMORY_START_COUNT, 4096 ); 			//Program sectors.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
		MAP_FlashCtl_protectSector(FLASH_INFO_MEMORY_SPACE_BANK1,FLASH_SECTOR0<<sectorcount);	//Protect sectors.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
	}
    sectorcount++;																			//increase counter for number of sectors already wrote to.

    if (sectorcount>=32){																	//This if statement is for when we have wrote to all 32 sectors of 4KB,
    	P2OUT |= BIT1;																		//once done a green LED will turn on for a second, then turn off. Afterwards the system
    	delay_1sec();																		//is reset via the watchdog counter
    	P2OUT &= ~BIT1;
    	sectorcount = 0;
    	WDTCTL = (WDTPW-1);
    }
	}
}
