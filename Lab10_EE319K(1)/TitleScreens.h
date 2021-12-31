//TitleScreens.h
//
//
//*******Game_Init***********
//Initializes the buttons required to play connect four (PE0 and PE1)
//Inputs: None
//Outputs: None
void Game_Init(void);

//*******ModeScreen************
//Goes to gameplay options
//Allows user to choose to play against another person
//or against a machine.
//User can also return to main menu.
void ModeScreen(void);

//*******MenuScreen************
//Goes to the main menu
//Inputs: none
//Outputs: none
void MenuScreen(void);

//*********StScreenInit*********
//Ouputs the title screen on the LED
//Inputs: None
//Outputs: None
void StScreenInit(void);

//*******StartScreen*********
//Title screen, allows user to either go to settings or play a game
//Requires call of StScreenInit prior to call of this function so the LED displays
//the proper menu
//Inputs: None
//Outputs: None
void StartScreen(void);

//**********OpenScreen***********
//Outputs the opening screen to the LED, where one selects the game's language
//Inputs: None
//Outputs: None
void OpenScreen(void);

//********GameStart*************
//Waits for user to choose a language for gameplay.
//User uses PE0 to scroll through two options and uses
//PE1 to confirm their choice.
//Requires call of OpenScreen prior to calling this function so
//the LED displays the proper screen.
//Inputs: None
//Outputs: None
void GameStart(void);

