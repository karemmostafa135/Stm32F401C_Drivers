#ifndef SYSTICK_H
#define SYSTICK_H

#define NO_PRESCALER			0x1
#define PRESCALER_DIVIDE_BY_8	0x0

typedef void(* SYSTICK_CallBk)(void);

typedef enum{
	SYSTICK_Nok,
	SYSTICK_Ok,
	SYSTICK_NullPtr,
	SYSTICK_Wrong_Input
}SYSTICK_Errors_t;

typedef unsigned int uint32_t;

/*
 *@brief to start the timer
 */
void SYSTICK_Start();

/*
 *@brief to stop the timer
 */

void SYSTICK_Stop();

/*
 *@brief to set the precaler value r
 *@options
 *1/NO_PRESCALER 2/PRESCALER_DIVIDE_BY_8
 *@default NO_PRESCALER
 */


SYSTICK_Errors_t SYSTICK_Set_Prescaler(uint32_t Copy_Prescaler_Value);

SYSTICK_Errors_t SYSTICK_Setms(uint32_t Copy_Required_Time);

SYSTICK_Errors_t SYSTICK_Set_CallBack(SYSTICK_CallBk Copy_CallBack_Func);



#endif
