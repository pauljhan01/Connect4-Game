// DAC.c
// This software configures DAC output
// Lab 6 requires 6 bits for the DAC
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 10/15/21 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data



// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R|(0x02);  //Enable clock for port B and F
  volatile uint16_t delay;			//Wait for clock to stabilize
	delay = 1000;
	while(delay!=0){
		delay = (delay - 1);
	}
	GPIO_PORTB_DIR_R = GPIO_PORTB_DIR_R|0x3F; //Set all bits 0-5 as outputs
	GPIO_PORTB_DEN_R = 0x3F; //Enable all bits 0-5
	
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Input=n is converted to n*3.3V/63
// Output: none
void DAC_Out(uint32_t data){
	
	GPIO_PORTB_DATA_R = data;
	
}
