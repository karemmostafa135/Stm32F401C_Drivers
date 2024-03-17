#include "SYSTICK.h"

#define SYSTICK_BASE_ADDRESS			0xE000E010
#define TICKINT_ENABLE					0x2
#define COUNTER_ENABLE					0x1
#define MAXIMUM_COUNTER_REG_CAPACITY	0x00FFFFFF
#define MINIMUM_COUNTER_REG_CAPACITY	0X00000001
#define CLK_SOURCE_PIN					2
#define CLK_FREQUENCY					16000000
#define EXTRA_TICK						0x00000001

typedef struct{
	uint32_t STK_CTRL;
	uint32_t STK_LOAD;
	uint32_t STK_VAL;
	uint32_t STK_CALIB;
}Systick_Reg_t;

volatile Systick_Reg_t *const SYSTICK=(Systick_Reg_t *)(0xE000E010);

static uint32_t Timer_Current_Freq=CLK_FREQUENCY; // to get the timer frequency

static SYSTICK_CallBk SYSTICK_CallBack_Func;	// pointer to get the call bk function from user

static uint32_t Global_SysTick_Counts=0;	// to count the number of entering timer handler

void SYSTICK_Start(){
	SYSTICK->STK_CTRL|=TICKINT_ENABLE; //to enable the systick interrupt
	SYSTICK->STK_CTRL|=COUNTER_ENABLE;	//to start the counter
}


void SYSTICK_Stop(){
	SYSTICK->STK_CTRL&=(~COUNTER_ENABLE);	//to stop the counter
	SYSTICK->STK_CTRL&=(~TICKINT_ENABLE); //to disable the systick interrupt
}

SYSTICK_Errors_t SYSTICK_Set_Prescaler(uint32_t Copy_Prescaler_Value){
	SYSTICK_Errors_t Local_Error=SYSTICK_Ok;
	if((Copy_Prescaler_Value>NO_PRESCALER)||(Copy_Prescaler_Value<PRESCALER_DIVIDE_BY_8)){
		Local_Error=SYSTICK_Nok;
	}
	else{
		switch (Copy_Prescaler_Value){
		case NO_PRESCALER:
			SYSTICK->STK_CTRL|=(1<<CLK_SOURCE_PIN);
		break;
		case PRESCALER_DIVIDE_BY_8:
			SYSTICK->STK_CTRL&=~(1<<CLK_SOURCE_PIN);
			Timer_Current_Freq/=8;
		break;
		default: Local_Error=SYSTICK_Nok; break;
		}
	}
	return Local_Error;
}

SYSTICK_Errors_t SYSTICK_Setms(uint32_t Copy_Required_Time)
{
	SYSTICK_Errors_t Local_Error_Status=SYSTICK_Ok;
	uint32_t Timer_Counts=0;
	Timer_Counts=(Copy_Required_Time*(Timer_Current_Freq/1000))-(uint32_t)EXTRA_TICK;
	if(Timer_Counts<MINIMUM_COUNTER_REG_CAPACITY||Timer_Counts>MAXIMUM_COUNTER_REG_CAPACITY){
		Local_Error_Status=SYSTICK_Wrong_Input;
	}
	else{
		SYSTICK->STK_VAL=0;	//to clear the counting register
		SYSTICK->STK_LOAD=Timer_Counts; // to set the time in the preload register

	}
return Local_Error_Status;
}

/*
 *@brief to get the function to do the action after the required time is completed
 */
SYSTICK_Errors_t SYSTICK_Set_CallBack(SYSTICK_CallBk Copy_CallBack_Func){
	SYSTICK_Errors_t Local_Error=SYSTICK_Ok;

	//to validate if the input pointer to function is not a null pointer
if(Copy_CallBack_Func){
	SYSTICK_CallBack_Func=Copy_CallBack_Func;
}
else{
	Local_Error=SYSTICK_NullPtr;
}
return Local_Error;
}


/************* systick handler function **************/

void SysTick_Handler (void)
{
if(SYSTICK_CallBack_Func){
	SYSTICK_CallBack_Func();
}
else{

}

}
