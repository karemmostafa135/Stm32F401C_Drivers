
#include "STD_types.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "LED.h"
#include "NVIC.h"
#include "SYSTICK.h"
#include "Sched.h"
#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  //#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

static int y=1;

int main(void)
{
	unsigned int z=0;
	LED_Init();
	Sched_Init();
	Sched_Start();
}
/* to test gpio ,led and switch
uint32_t counter =0;
	uint32_t read;
	LED_Init();
	SWITCH_Init();
	LED_Set_State(0, 1);
	while (1){
	SWITCH_Get_Read(0, &read);
	if (read==1){
		LED_Set_State(0, 1);
		counter++;
	}
	if(counter%2==0){
		LED_Set_State(0, 0);
	}
*/

//testing the systick
/*
unsigned int z=0;
	LED_Init();
	SYSTICK_Setms(1000);
	SYSTICK_Start();
	SYSTICK_Set_CallBack(led_on);
	while(1){
		int x=5;
		x++;
	}
*/





