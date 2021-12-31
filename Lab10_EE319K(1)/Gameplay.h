//
//
//***********Gameplay.h************
//
//
//

//************Systick_Init**********
// Initializes systick for ADC interrupt
// Inputs: 32 bit period for interrupt
// Outputs: none
void SysTick_Init(int period);


//Disables SysTick
void SysTick_Disable(void);


//Clears the board of all previous input
void ClearBoard(void);

//************ConnectStart**********
// Outputs the empty connect four board to the LCD.
// Inputs: none
// Outputs: none
void ConnectStart(void);

void HumanVHuman(void);

void HumanVMachine(void);
