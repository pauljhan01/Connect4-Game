// TitleScreens.c
// Runs on TM4C123
// Anya Price and Paul Han
// Contains all intializations and title card functions

// Last Modified: 11/15/2021 

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "Print.h"
#include "Gameplay.h"
#include "Gameplay.c"

uint8_t LangRet = 0;		//Two flags that indicate whether the user wants to return to the main menu
uint8_t MainRet = 0; 		//or language options.



//*******Game_Init***********
//Initializes the buttons required to play connect four (PE0 and PE1)
//Inputs: None
//Outputs: None
void Game_Init(void){
	SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R|(0x10);  //Enable clock for port E
  volatile uint16_t delay = 1000;								 //Wait for clock to stabilize
	while(delay!=0){
		delay = (delay - 1);
	}
	GPIO_PORTE_DIR_R &= 0x03; //Set bits 0 and 1 as inputs
	GPIO_PORTE_DEN_R = 0x03; // Digital enable bits 0 and 1
}

//*******Game_End*********
//Outputs the game end screen to LCD and allows user to return to main menu
//Inputs: None
//Outputs: None
void Game_End(void){
	ST7735_FillScreen(0x0000);           			 // set screen to black
	ST7735_SetCursor(6, 5);
	ST7735_OutString((char *)Phrases[Outcome][myL]);  //Display game outcome
	if(Human == 1)
	{
		if(CurPlay == Yellow)
		{
			ST7735_SetCursor(1, 7);
			ST7735_OutString((char *)Phrases[ScoreRed][myL]);
			LCD_OutDec(ScoreforRed);
		}
		if(CurPlay == Red)
		{
			ST7735_SetCursor(1,7);
			ST7735_OutString((char *)Phrases[ScoreYellow][myL]);
			LCD_OutDec(ScoreforYellow);
		}
	}
	if(Machine == 1)
	{
		if(turn==0)
		{
			ST7735_SetCursor(1, 7);
			ST7735_OutString((char *)Phrases[ScoreRed][myL]);
			LCD_OutDec(ScoreforRed);
		}
		if(turn==1)
		{
			ST7735_SetCursor(1,7);
			ST7735_OutString((char *)Phrases[ScoreYellow][myL]);
			LCD_OutDec(ScoreforYellow);
		}
	}
	ST7735_SetCursor(12, 13);
	ST7735_OutString((char *)Phrases[Okay][myL]);				//Output okay
	ST7735_DrawBitmap(80, 150, SouthTriangle, 9, 5);		//Arrow above right button to indicate click okay
	while(MainRet == 0){
	if((GPIO_PORTE_DATA_R&(0x02)) != 0){											
		while((GPIO_PORTE_DATA_R&(0x02)) != 0){										//Wait for button release
		}
		MainRet = 1;
	}
}
}


//*******ModeScreen************
//Goes to gameplay options
//Allows user to choose to play against another person
//or against a machine.
//User can also return to main menu.
void ModeScreen(void){
		ST7735_FillScreen(0x0000);           			 // set screen to black
		ST7735_SetCursor(3, 3);
		ST7735_OutString((char *)Phrases[HvH][myL]);	//Output "human v human"
		ST7735_SetCursor(3, 6);
		ST7735_OutString((char *)Phrases[HvM][myL]);
		ST7735_SetCursor(3, 9);
		ST7735_OutString((char *)Phrases[Back][myL]);			//Output back
	  ST7735_DrawBitmap(5, 37, EastTriangle, 5, 9);		//Arrow on human v human
		ST7735_SetCursor(4, 13);
	  ST7735_OutString((char *)Phrases[Scroll][myL]);			//Output Scroll
		ST7735_DrawBitmap(30, 150, SouthTriangle, 9, 5);		//Arrow above left button to indicate click to scroll
		ST7735_SetCursor(12, 13);
	  ST7735_OutString((char *)Phrases[Okay][myL]);				//Output okay
		ST7735_DrawBitmap(80, 150, SouthTriangle, 9, 5);		//Arrow above right button to indicate click okay
	MainRet = 0;						//Reset return flag
	uint8_t scrollArr = 0;	//Indicates which menu option the user is currently 'looking' at
	while(MainRet == 0){
		//If user scrolls (clicks PE0 button)
		if((GPIO_PORTE_DATA_R&(0x01)) == 1){											
		while((GPIO_PORTE_DATA_R&(0x01)) == 1){										//Wait for button release
		}
		if(scrollArr == 1){																				//If arrow is on hvm, switch to back arrow
		ST7735_FillRect(5, 58, 5, 12, 0x0000);
		ST7735_DrawBitmap(5, 97, EastTriangle, 5, 9);						//arrow on back
		scrollArr = 2;
		}
		else if(scrollArr == 0){																	//If arrow is on hvh, switch arrow to hvm
		ST7735_FillRect(5, 28, 5, 10, 0x0000);
		ST7735_DrawBitmap(5, 67, EastTriangle, 5, 9);						//arrow on hvm
		scrollArr = 1;
		}else{																										//If neither, arrow is on back, switch to hvh
			ST7735_DrawBitmap(5, 37, EastTriangle, 5, 9);					//Arrow on human v human
			ST7735_FillRect(5, 88, 5, 12, 0x0000);
			scrollArr = 0;
		}
}
	//If user confirms (clicks PE1 button)
		if((GPIO_PORTE_DATA_R&(0x02)) != 0){		
		while((GPIO_PORTE_DATA_R&(0x02)) != 0){										//Wait for button release	
		}
		if(scrollArr == 0){					//If arrow is on hvh, go to hvh mode
			HumanVHuman();
			Game_End();
		}
		if(scrollArr == 1){					//If arrow is on hvm, go to hvm mode
			HumanVMachine();
			Game_End();
		}
		if(scrollArr == 2){
			MainRet = 1;						//If user has chosen to return to main screen, set flag for return
}
		}
	}
}



//*******MenuScreen************
//Goes to the main menu
void MenuScreen(void){
		ST7735_FillScreen(0x0000);           			 // set screen to black
		ST7735_SetCursor(7, 5);
		ST7735_OutString((char *)Phrases[ChangeLang][myL]);	//Output "language"
		ST7735_SetCursor(7, 8);
		ST7735_OutString((char *)Phrases[Back][myL]);			//Output back
	  ST7735_DrawBitmap(30, 57, EastTriangle, 5, 9);		//Arrow on language
		ST7735_SetCursor(4, 13);
	  ST7735_OutString((char *)Phrases[Scroll][myL]);			//Output Scroll
		ST7735_DrawBitmap(30, 150, SouthTriangle, 9, 5);		//Arrow above left button to indicate click to scroll
		ST7735_SetCursor(12, 13);
	  ST7735_OutString((char *)Phrases[Okay][myL]);				//Output okay
		ST7735_DrawBitmap(80, 150, SouthTriangle, 9, 5);		//Arrow above right button to indicate click okay
	MainRet = 0;						//Reset both return flags
	LangRet = 0;
	uint8_t scrollArr = 0;	//Indicates which menu option the user is currently 'looking' at
	while(MainRet == 0 && LangRet == 0){
		//If user scrolls (clicks PE0 button)
		if((GPIO_PORTE_DATA_R&(0x01)) == 1){											
		while((GPIO_PORTE_DATA_R&(0x01)) == 1){										//Wait for button release
		}
		if(scrollArr == 0){																					//If arrow is on change language, switch arrow to back
		ST7735_FillRect(30, 48, 5, 10, 0x0000);
		ST7735_DrawBitmap(30, 85, EastTriangle, 5, 9);						//arrow on back
		scrollArr = 1;
		}else{																										//If not, arrow is on back, switch to language arrow
		ST7735_FillRect(30, 74, 5, 12, 0x0000);
		ST7735_DrawBitmap(30, 57, EastTriangle, 5, 9);						//arrow on language
		scrollArr = 0;
		}
}
	//If user confirms (clicks PE1 button)
		if((GPIO_PORTE_DATA_R&(0x02)) != 0){		
		while((GPIO_PORTE_DATA_R&(0x02)) != 0){										//Wait for button release	
		}
		if(scrollArr == 1){
			MainRet = 1;							//If user haa chosen to return to main screen, set flag for return
}else{
	LangRet = 1;									//Else, return to language screen
}
		}
	}
}

//*********StScreenInit*********
//Ouputs the title screen on the LED
void StScreenInit(void){
	ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_SetCursor(6, 5);
	ST7735_OutString((char *)Phrases[Title][myL]);	//Output Connect4!
	ST7735_SetCursor(5, 13);
	ST7735_OutString((char *)Phrases[Menu][myL]);		//Menu at bottom left
	ST7735_DrawBitmap(35, 150, NorthTriangle, 9, 5);	//Arrow to menu
	ST7735_SetCursor(12, 13);
	ST7735_OutString((char *)Phrases[Play][myL]);		//Play at bottom right
	ST7735_DrawBitmap(80, 150, NorthTriangle, 9, 5);	//Arrow to play
}

//*********StartScreen**********
//Waits for user input regarding the title screen
void StartScreen(void){
	StScreenInit();						//Output the main menu on screen
	while(LangRet == 0){
		if(MainRet != 0){				//If returning to the main menu, must be output again
			StScreenInit();
			MainRet = 0;
		}
	//If user selects menu (clicks PE0 button)
	if((GPIO_PORTE_DATA_R&(0x01)) == 1){											
		while((GPIO_PORTE_DATA_R&(0x01)) == 1){										//Wait for button release
		}
		MenuScreen();
	}
	//If user selects play (clicks PE1 button)
	if((GPIO_PORTE_DATA_R&(0x02)) != 0){		
		while((GPIO_PORTE_DATA_R&(0x02)) != 0){										//Wait for button release	
		}
		ModeScreen();																							//Go to screen where user chooses game mode
	}
}
	}

//**********OpenScreen***********
//Initializes to the opening screen where one selects the game's language
//
void OpenScreen(void){
	  ST7735_FillScreen(0x0000);           			 // set screen to black
			ST7735_SetCursor(7, 5);
    for(Language_t myL=English; myL<= French; myL++){					//Output language options
         ST7735_OutString((char *)Phrases[Language][myL]);
		ST7735_SetCursor(7, 8);
	}
	ST7735_DrawBitmap(30, 57, EastTriangle, 5, 9);						//Arrow on English
	ST7735_DrawBitmap(30, 140, SouthTriangle, 9, 5);					//Arrow above left button to indicate scroll
	ST7735_DrawBitmap(80, 140, Check, 6, 8);	                //Checkmark above right button to indicate dz
	LangPnt = 0;
}

//********GameStart*************
//Waits for user to choose a language for gameplay.
//User uses PE0 to scroll through two options and uses
//PE1 to confirm their choice.
void GameStart(void){
		while(1){
			if(LangRet != 0){																			//If we are returning to this screen, LED must be reinitialized
				OpenScreen();
				LangRet = 0;
			}
	//If user scrolls (clicks PE0 button)
			if((GPIO_PORTE_DATA_R&(0x01)) == 1){											
				while((GPIO_PORTE_DATA_R&(0x01)) == 1){};										//Wait for button release
				if(LangPnt == 0){																					//If click scroll button on english, change arrow to french
				ST7735_FillRect(30, 48, 5, 10, 0x0000);
				ST7735_DrawBitmap(30, 85, EastTriangle, 5, 9);						//Pointer on French
				LangPnt = 1;
			}else{																										//If not, arrow is on french, switch to english arrow
				ST7735_FillRect(30, 74, 5, 12, 0x0000);
				ST7735_DrawBitmap(30, 57, EastTriangle, 5, 9);						//arrow on English
				LangPnt = 0;
			}
		}
	//If user confirms (clicks PE1 button)
		if((GPIO_PORTE_DATA_R&(0x02)) != 0){		
		while((GPIO_PORTE_DATA_R&(0x02)) != 0){										//Wait for button release	
		}
		if(LangPnt == 0){
			myL = English;							//Set language to English
			StartScreen();
}else{
	myL = French;			//Else, set language to french
	StartScreen();
}
		}
	}
}

//*******PauseMenu*************
//Goes to Pause Menu and gives the option to go to the main menu and stop the game or resume the game
void PauseMenu(void){
		Pause1 = 1;
		ST7735_FillScreen(0x0000);
		ST7735_SetCursor(6, 3);
		ST7735_OutString((char *)Phrases[Pause][myL]); //Prints out "Pause Menu"
		ST7735_SetCursor(3, 6);
		ST7735_OutString((char *)Phrases[Resume][myL]); //Prints out "Resume" Option
		ST7735_SetCursor(3, 9);
		ST7735_OutString((char *)Phrases[Return][myL]);
		ST7735_DrawBitmap(5, 67, EastTriangle, 5, 9); //arrow on Resume
		ST7735_SetCursor(4, 13);
	  ST7735_OutString((char *)Phrases[Scroll][myL]); //"Scroll"
		ST7735_DrawBitmap(30, 150, SouthTriangle, 9, 5);
		ST7735_SetCursor(12, 13);
		ST7735_OutString((char *)Phrases[Okay][myL]); //"Okay"
		ST7735_DrawBitmap(80, 150, SouthTriangle, 9, 5);
		MainRet = 0;
		uint8_t scrollArr = 0;
		while(MainRet == 0)
		{
			if(GPIO_PORTE_DATA_R&(0x01) != 0){
				while(GPIO_PORTE_DATA_R&(0x01) != 0){};
				if(scrollArr==0)
				{
					ST7735_FillRect(5, 58, 5, 12, 0x0000);
					ST7735_DrawBitmap(5, 97, EastTriangle, 5, 9);  //arrow on Resume
					scrollArr = 1;
				}
				else
				{
					ST7735_FillRect(5, 88, 5, 10, 0x0000);
					ST7735_DrawBitmap(5, 67, EastTriangle, 5, 9); //arrow on Main Menu
					scrollArr = 0;
				}
			}
			if((GPIO_PORTE_DATA_R&(0x02)) != 0){		
				while((GPIO_PORTE_DATA_R&(0x02)) != 0){};
				if(scrollArr == 0) //if User clicks Resume
				{
					if(Human == 1) //if Mode was Human v Human
					{
						HumanVHuman();
						Game_End();
					}
					if(Machine == 1) //If Mode was Human v AI
					{
						HumanVMachine();
						Game_End();
					}
				}
				if(scrollArr == 1) //If User clicks Main Menu
				{
					Pause1 = 0; //Clears board and resets game
					MainRetG = 1; //sets main menu flag
					StartScreen(); //Goes back to main menu
					
				}
			}
		}
}
