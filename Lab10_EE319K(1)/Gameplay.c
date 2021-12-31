//
//
//************Gameplay.c****************
//
//Contains all functions regarding gameplay,
//including the game itself and the pause menu.
//
//


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Print.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "DAC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer1.h"
#include "Languageinfo.h"

void PauseMenu(void);

typedef enum {Red, Yellow} Player_t;
volatile Player_t CurPlay=Red;
typedef enum {White, Blue} background_t;
const unsigned short *Circles[2][2]={
	{RedCircleWB, RedCircleBB},
	{YellowCircleWB, YellowCircleBB}
};

uint8_t MainRetG = 0;											//Determines whether user wants to return to main menu
uint8_t Pause1 = 0;
volatile phrase_t Outcome;								//keeps track of the game outcome
uint8_t Human = 0; //Flag for Human v Human Mode
uint8_t Machine = 0; //Flag for Human v Machine Mode
uint8_t turn = 0; //keeps track of turn for AI or human 0 for human, 1 for machine
uint8_t	ScoreforRed = 0;
uint8_t	ScoreforYellow = 0;
//Digitized state of board, 0 meaning no token placed in that position,
//1 meaning red token placed in that position, 2 meaning
//yellow token placed in that position.
//Each array represents a row on the board, with the highest number array
//corresponding to the lowest row.
volatile uint8_t Board[7][7]={
	{0, 0, 0, 0, 0, 0, 0},				//First row of the board
	{0, 0, 0, 0, 0, 0, 0},	
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0}					//Last row of the board
};

volatile uint8_t Spaces[7] = {0, 0, 0, 0, 0, 0, 0};			//Keeps track of how many slots per column are filled

uint32_t ADCLast = 0;						//Contains previous ADC output
int8_t column = 0;							//Keeps track of which column the user is hovering over
uint8_t PlayChange = 0;					//Lets SysTick know if player has been changed

//Clears the board of all previous input
void ClearBoard(void){
	Human = 0;
	Machine = 0;
	uint8_t Ri = 0;
	uint8_t Ci = 0;
	while(Ri != 8){
		for(Ci=0; Ci != 8; Ci++){
			Board[Ri][Ci] = 0;
			Spaces[Ci] = 0;
		}
		Ci = 0;
		Ri++;
		}
	}

//Disables SysTick
void SysTick_Disable(void){
	NVIC_ST_CTRL_R = 0;
}
	

void SysTick_Init(int period){
  NVIC_ST_CTRL_R = 0;						//SysTick Init
	NVIC_ST_RELOAD_R = period;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R|0x08000000; //Priority 4
	NVIC_ST_CTRL_R = 0x07; //Enable with clock and interrupts
	
	}
uint32_t NeedtoDraw;
int32_t columnI;
void SysTick_Handler(void){
	Random_Init(NVIC_ST_CURRENT_R);
	uint32_t ADCMail;
	ADCMail = ADC_In();
	if((ADCMail != ADCLast) || (PlayChange == 1)){		//If we have received new input, change token placement
		column = -1;
		uint32_t ADCPnt = 585;
		columnI = -15; //SysTick runs first then outputs to LCD, but columnI needs to incremented in SysTick Handler so I set the initial columnI value to -15 so it starts at 3 and not 18.
		uint8_t OutFlag = 0;			//Keeps track of output
		while(OutFlag == 0){		  //Cycles through the different column zones until reaching the appropriate placement indicated by the ADC
			if(ADCMail <= ADCPnt){
				NeedtoDraw = 1; //Set flag to output to LCD
				OutFlag = 1;
				
			}
			columnI += 18;
			column ++;
			ADCPnt += 585;
		}
		ADCLast = ADCMail;
		PlayChange = 0;
		}
	}		

// This function checkst the current board for any wins.
//
void CheckWin(void){
	SysTick_Disable();
	volatile uint8_t r = 0;						  // r for row and c for column
	volatile uint8_t c = 0;
	

	//Check left-leaning diagonal win
  for(r = 0; (r+3) < 7 && MainRetG == 0; r++){
		for(c = 0; (c+3 < 7) && MainRetG == 0; c++){
			if(((Board[r][c] == 1) && (Board[r+1][c+1] == 1) && (Board[r+2][c+2] == 1) && (Board[r+3][c+3] == 1))){
				Outcome = RedWin;
			ScoreforRed++;
			MainRetG = 1;
		}
			if(((Board[r][c] == 2) && (Board[r+1][c+1] == 2) && (Board[r+2][c+2] == 2) && (Board[r+3][c+3] == 2))){
			  Outcome = YellowWin;
			ScoreforYellow++;
			MainRetG = 1;
		}
	}
		c = 0;
	}

	//Check right-leaning diagonal win
	r = 3;
	 for(r = 3; r < 7  && MainRetG == 0; r++){
		for(c = 0; (c+3 < 7)  && MainRetG == 0; c++){
			if(((Board[r][c] == 1) && (Board[r-1][c+1] == 1) && (Board[r-2][c+2] == 1) && (Board[r-3][c+3] == 1))){
				Outcome = RedWin;
			ScoreforRed++;
			MainRetG = 1;
		}
			if(((Board[r][c] == 2) && (Board[r-1][c+1] == 2) && (Board[r-2][c+2] == 2) && (Board[r-3][c+3] == 2))){
			  Outcome = YellowWin;
			ScoreforYellow++;
			MainRetG = 1;
		}
	}
		c = 0;
	}
	 
	//Check for horizontal win
	r = 0;
  for(r = 0; r < 7  && MainRetG == 0; r++){
		for(c = 0; (c+3 < 7)  && MainRetG == 0; c++){
			if(((Board[r][c] == 1) && (Board[r][c+1] == 1) && (Board[r][c+2] == 1) && (Board[r][c+3] == 1))){
				Outcome = RedWin;
			ScoreforRed++;
			MainRetG = 1;
		}
			if(((Board[r][c] == 2) && (Board[r][c+1] == 2) && (Board[r][c+2] == 2) && (Board[r][c+3] == 2))){
			  Outcome = YellowWin;
			ScoreforYellow++;
			MainRetG = 1;
		}
	}
		c = 0;
	}
	
	//Check for vertical win
	r = 0;
  for(r = 0; r+3 < 7  && MainRetG == 0; r++){
		for(c = 0; c < 7 && MainRetG == 0; c++){
			if(((Board[r][c] == 1) && (Board[r+1][c] == 1) && (Board[r+2][c] == 1) && (Board[r+3][c] == 1))){
				Outcome = RedWin;
			ScoreforRed++;
			MainRetG = 1;
		}
			if(((Board[r][c] == 2) && (Board[r+1][c] == 2) && (Board[r+2][c] == 2) && (Board[r+3][c] == 2))){
			  Outcome = YellowWin;
			ScoreforYellow++;
			MainRetG = 1;
		}
	}
		c = 0;
	}
	
	//Check for draw
	if(MainRetG == 0 && Spaces[0] == 7 && Spaces[1] == 7 && Spaces[2] == 7 && Spaces[3] == 7 &&
		 Spaces[4] == 7 && Spaces[5] == 7 && Spaces[6] == 7){
				Outcome = Draw;
			 MainRetG = 1;
		 }
	if(MainRetG == 0){
		 SysTick_Init(800000);        //Re-enable Systick
	}
		 }

// Outputs the empty connect four board to the LCD.
void ConnectStart(void){
	MainRetG = 0;
	if(Pause1 == 0) //If not resuming game, clear board with flag
	{
		ClearBoard();
	}
	ST7735_FillScreen(0xf800);				//Set screen to blue
	ST7735_FillRect(0, 0, 138, 20, 0xffff); //Create a twenty pixel tall white band at the top of the screen for disk placement
//	ST7735_FillRect(0, 160, 138, 20, 0x0000); //Create a twenty pixel tall white band at the top of the screen for disk placement
	uint32_t columnI = 3;
	uint32_t rowI = 39;
	
	for(rowI = 39; rowI < 150; (rowI += 18)){
		for(columnI = 3; columnI < 130; columnI = (columnI + 18)){
			ST7735_DrawBitmap(columnI, rowI, WhiteCircle, 14, 14);
		}
		columnI = 3;
	}
	uint8_t x;
	uint8_t y;
	uint8_t xcoord = 0;
	uint8_t ycoord = 0;
	for(y=0;y<7;y++){
		for(x=0;x<7;x++)
		{
			if(Board[y][x] == 1)
			{
				CurPlay = Red;
				xcoord = x*18+3;
				ycoord = y*18+39;
				ST7735_DrawBitmap(xcoord, ycoord, (unsigned short *)Circles[CurPlay][Blue], 14, 14);
			}
			if(Board[y][x] == 2)
			{
				CurPlay = Yellow;
				xcoord = x*18+3;
				ycoord = y*18+39;
				ST7735_DrawBitmap(xcoord, ycoord, (unsigned short *)Circles[CurPlay][Blue], 14, 14);
			}
		}
		columnI = 3;
	}
	if(Pause1 == 0)
	{
		CurPlay = Red; //If game has reset, set first player to Red
	}
	SysTick_Init(800000);										//Set up ADC interrupt
}



void HumanVHuman(void){
	MainRetG = 0;
	ConnectStart();
	Human = 1;
	columnI = 3;
		while(MainRetG == 0){									//If user does not want to return to main menu
	//If user confirms (clicks PE1 button)
			if(NeedtoDraw == 1) //Flag set in SysTick to indicate that LCD needs to be outputted to
			{
				ST7735_FillRect(0, 0, 138, 20, 0xffff); 		//Clear current disk placement
				ST7735_DrawBitmap(columnI, 17, (unsigned short *)Circles[CurPlay][White], 14, 14);
				NeedtoDraw = 0;
			}
			if((GPIO_PORTE_DATA_R&(0x01)) != 0)
			{
				while((GPIO_PORTE_DATA_R&(0x01)) != 0){};
				PauseMenu(); //If PE0 is pressed, go to Pause Menu
			}
			if((GPIO_PORTE_DATA_R&(0x02)) != 0){		
				while((GPIO_PORTE_DATA_R&(0x02)) != 0){};										//Wait for button release
				Sound_Fastinvader3(); //periodic interrupt outputting sound
			if(Spaces[column] != 7){
				uint32_t yvalue = (21 + ((7-(Spaces[column]))*18));			//Find y-value to draw circle
				Spaces[column] ++;
				uint32_t xvalue = (3 + ((column)*18));
				ST7735_DrawBitmap(xvalue, yvalue,(unsigned short *)Circles[CurPlay][Blue], 14, 14);
				if(CurPlay == Red){													//Switch players and change state of board
					uint8_t track2 = (6-Spaces[column]+1);
				
					Board[6-(Spaces[column]-1)][column] = 1;
					CurPlay = Yellow;
					PlayChange = 1;
				}else{
					Board[6-(Spaces[column]-1)][column] = 2;
					CurPlay = Red;
					PlayChange = 1;
				}
			}
			CheckWin();				//See if there is a win or a draw.
		}
	}
}

void HumanVMachine(void){
	MainRetG = 0;
	ConnectStart();
	Machine = 1; //Flag indicating mode is Human vs Machine
	uint32_t rand = 0; //Used to output random moves by AI
	uint32_t lastrand = 0;
	uint32_t xvalue = 0; //used to keep track of where to print tokens on LCD
	uint32_t yvalue = 0; //same comment as above
	while(MainRetG == 0)
	{
		if(NeedtoDraw == 1) //Flag set in SysTick to indicate that LCD needs to be outputted to
			{
				ST7735_FillRect(0, 0, 138, 20, 0xffff); 		//Clear current disk placement
				ST7735_DrawBitmap(columnI, 17, (unsigned short *)Circles[CurPlay][White], 14, 14);
				NeedtoDraw = 0;
			}
		if((GPIO_PORTE_DATA_R&(0x01)) != 0)  //If PE0 is pressed, go to Pause Menu
			{
				while((GPIO_PORTE_DATA_R&(0x01)) != 0){};
				PauseMenu(); 
			}
		if((GPIO_PORTE_DATA_R&(0x02)) != 0) //For Player use only
		{		
			while((GPIO_PORTE_DATA_R&(0x02)) != 0){};										//Wait for button release	
			Sound_Fastinvader3(); //periodic interrupt outputting sound
			if(Spaces[column] != 7)
			{
				yvalue = (21 + ((7-(Spaces[column]))*18));			//Find y-value to draw circle
				Spaces[column] ++;
				xvalue = (3 + ((column)*18));
				ST7735_DrawBitmap(xvalue, yvalue,(unsigned short *)Circles[CurPlay][Blue], 14, 14);
				if(CurPlay == Red)
				{													//Switch players and change state of board
					uint8_t track2 = (6-(((Spaces[column]))));				
					Board[6-(((Spaces[column])-1))][column] = 1;
					CurPlay = Yellow;
					PlayChange = 1;
				}
				else
				{
					Board[6-(((Spaces[column])-1))][column] = 2;
					CurPlay = Red;
					PlayChange = 1;
				}
				turn = 1;
			}
			CheckWin();				//See if there is a win or a draw.
			if(turn == 1)
			{
				turn = 0;
				rand = (Random32()>>24)%6;
				Sound_Fastinvader4(); //periodic interrupt outputting sound
				while(rand==lastrand)
				{
					rand = (Random32()>>24)%6;
				}
				if(Spaces[rand]!=7)
				{
					yvalue = (21 + ((7-(Spaces[rand]))*18));
					Spaces[rand]++;
					xvalue = (3 + ((rand)*18));
					ST7735_DrawBitmap(xvalue, yvalue,(unsigned short *)Circles[CurPlay][Blue], 14, 14);
					if(CurPlay == Red)
					{													//Switch players and change state of board
						uint8_t track2 = (6-(((Spaces[rand]))));				
						Board[6-(((Spaces[rand])-1))][rand] = 1;
						CurPlay = Yellow;
						PlayChange = 1;
					}
					else
					{
						Board[6-(((Spaces[rand])-1))][rand] = 2;
						CurPlay = Red;
						PlayChange = 1;
					}
				}
				else if(Spaces[rand]==7)
				{
					while(Spaces[rand]==7)
					{
						rand = (Random32()>>24)%6;
					}
					yvalue = (21 + ((7-(Spaces[rand]))*18));
					Spaces[rand]++;
					xvalue = (3 + ((rand)*18));
					ST7735_DrawBitmap(xvalue, yvalue,(unsigned short *)Circles[CurPlay][Blue], 14, 14);
					if(CurPlay == Red)
					{													//Switch players and change state of board
						uint8_t track2 = (6-(((Spaces[rand]))));				
						Board[6-(((Spaces[rand])-1))][rand] = 1;
						CurPlay = Yellow;
						PlayChange = 1;
					}
					else
					{
						Board[6-(((Spaces[rand])-1))][rand] = 2;
						CurPlay = Red;
						PlayChange = 1;
					}
				}
				lastrand = rand;
				CheckWin();
			}
		}			
	}
}


