// Program Flash Memory

#include "msp432.h"
#include "driverlib.h"
#include "string.h"


#define BUFFER_MEMORY_START0 0x020000
#define BUFFER_MEMORY_START1 0x021000
#define BUFFER_MEMORY_START2 0x022000
#define BUFFER_MEMORY_START3 0x023000
#define BUFFER_MEMORY_START4 0x024000
#define BUFFER_MEMORY_START5 0x025000
#define BUFFER_MEMORY_START6 0x026000
#define BUFFER_MEMORY_START7 0x027000
#define BUFFER_MEMORY_START8 0x028000
#define BUFFER_MEMORY_START9 0x029000
#define BUFFER_MEMORY_START10 0x02A000
#define BUFFER_MEMORY_START11 0x02B000
#define BUFFER_MEMORY_START12 0x02C000
#define BUFFER_MEMORY_START13 0x02D000
#define BUFFER_MEMORY_START14 0x02E000
#define BUFFER_MEMORY_START15 0x02F000
#define BUFFER_MEMORY_START16 0x030000
#define BUFFER_MEMORY_START17 0x031000
#define BUFFER_MEMORY_START18 0x032000
#define BUFFER_MEMORY_START19 0x033000
#define BUFFER_MEMORY_START20 0x034000
#define BUFFER_MEMORY_START21 0x035000
#define BUFFER_MEMORY_START22 0x036000
#define BUFFER_MEMORY_START23 0x037000
#define BUFFER_MEMORY_START24 0x038000
#define BUFFER_MEMORY_START25 0x039000
#define BUFFER_MEMORY_START26 0x03A000
#define BUFFER_MEMORY_START27 0x03B000
#define BUFFER_MEMORY_START28 0x03C000
#define BUFFER_MEMORY_START29 0x03D000
#define BUFFER_MEMORY_START30 0x03E000
#define BUFFER_MEMORY_START31 0x03F000

uint8_t BUFFER_MEMORY_START_COUNT = 0x0000;


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
																			//making sure my offset is set to 0
	WDTCTL = WDTPW |WDTSSEL_3|WDTIS_4|WDTCNTCL;												//Watchdog timer set to expire every second
	WDTCTL = WDTPW | WDTCNTCL;

	//MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
	//MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
	WDTCTL = WDTPW | WDTCNTCL;

	while(1){
		WDTCTL = WDTPW | WDTCNTCL; //clear timer to prevent timeout
	if (!(P1IN & BIT4)){																	//check if timeout button is pushed
		P1OUT |= BIT0;																		//RED LED
		delay_1sec();
		WDTCTL = (WDTPW-1);																	//forces reset
	}

	uint8_t buffer[4096];
		memset(buffer, 0xA5, 4096);

		WDTCTL = WDTPW | WDTCNTCL;																			//Unprotects all sectors
	    MAP_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR0|FLASH_SECTOR1|FLASH_SECTOR2|FLASH_SECTOR3|FLASH_SECTOR4|FLASH_SECTOR5
				|FLASH_SECTOR6|FLASH_SECTOR7|FLASH_SECTOR8|FLASH_SECTOR9|FLASH_SECTOR10|FLASH_SECTOR11|FLASH_SECTOR12|FLASH_SECTOR13|FLASH_SECTOR14
				|FLASH_SECTOR15|FLASH_SECTOR16|FLASH_SECTOR17|FLASH_SECTOR18|FLASH_SECTOR19|FLASH_SECTOR20|FLASH_SECTOR21|FLASH_SECTOR22|FLASH_SECTOR23
				|FLASH_SECTOR24|FLASH_SECTOR25|FLASH_SECTOR26|FLASH_SECTOR27|FLASH_SECTOR28|FLASH_SECTOR29|FLASH_SECTOR30|FLASH_SECTOR31);

	    WDTCTL = WDTPW | WDTCNTCL;																			//Erases all sectors
	    if(!MAP_FlashCtl_eraseSector(BUFFER_MEMORY_START0|BUFFER_MEMORY_START1|BUFFER_MEMORY_START2|BUFFER_MEMORY_START3|BUFFER_MEMORY_START4|BUFFER_MEMORY_START5|
	    		BUFFER_MEMORY_START6|BUFFER_MEMORY_START7|BUFFER_MEMORY_START8|BUFFER_MEMORY_START9|BUFFER_MEMORY_START10|BUFFER_MEMORY_START11|BUFFER_MEMORY_START12|
				BUFFER_MEMORY_START13|BUFFER_MEMORY_START14|BUFFER_MEMORY_START15|BUFFER_MEMORY_START16|BUFFER_MEMORY_START17|BUFFER_MEMORY_START18|BUFFER_MEMORY_START19|
				BUFFER_MEMORY_START20|BUFFER_MEMORY_START21|BUFFER_MEMORY_START22|BUFFER_MEMORY_START23|BUFFER_MEMORY_START24|BUFFER_MEMORY_START25|BUFFER_MEMORY_START26|
				BUFFER_MEMORY_START27|BUFFER_MEMORY_START28|BUFFER_MEMORY_START29|BUFFER_MEMORY_START30|BUFFER_MEMORY_START31)) while(1);
	    WDTCTL = WDTPW | WDTCNTCL;
	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START0, 4096))while(1);
	    WDTCTL = WDTPW | WDTCNTCL;

	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START1, 4096))while(1);		//Numerous program statements foreach sector, could not figure out
	    	    WDTCTL = WDTPW | WDTCNTCL;																	//how to do it in a loop. Programs each sector, then clears timer.

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START2, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START3, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START4, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START5, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START6, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START7, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START8, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START9, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START10, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START11, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START12, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START13, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START14, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START15, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START16, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START17, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START18, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START19, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START20, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START21, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START21, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START22, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START23, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START24, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START25, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START26, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START27, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START28, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START29, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START30, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    	    WDTCTL = WDTPW | WDTCNTCL;
	    	    if(!MAP_FlashCtl_programMemory(buffer,  (void*) BUFFER_MEMORY_START31, 4096))while(1);
	    	    WDTCTL = WDTPW | WDTCNTCL;

	    MAP_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR0|FLASH_SECTOR1|FLASH_SECTOR2|FLASH_SECTOR3|FLASH_SECTOR4|FLASH_SECTOR5
				|FLASH_SECTOR6|FLASH_SECTOR7|FLASH_SECTOR8|FLASH_SECTOR9|FLASH_SECTOR10|FLASH_SECTOR11|FLASH_SECTOR12|FLASH_SECTOR13|FLASH_SECTOR14
				|FLASH_SECTOR15|FLASH_SECTOR16|FLASH_SECTOR17|FLASH_SECTOR18|FLASH_SECTOR19|FLASH_SECTOR20|FLASH_SECTOR21|FLASH_SECTOR22|FLASH_SECTOR23
				|FLASH_SECTOR24|FLASH_SECTOR25|FLASH_SECTOR26|FLASH_SECTOR27|FLASH_SECTOR28|FLASH_SECTOR29|FLASH_SECTOR30|FLASH_SECTOR31);	//Protect sectors.
		WDTCTL = WDTPW | WDTCNTCL;																//clear timer constantly to prevent timeout.
																	//Once all statements have been written to, a GREEN LED flashes, then starts over until timeout
    	P2OUT |= BIT1;
    	delay_1sec();
    	P2OUT &= ~BIT1;



	}
}
