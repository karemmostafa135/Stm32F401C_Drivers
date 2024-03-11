
#include "STD_types.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "LED.h"
#include "SWITCH.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  //#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

int main(void)
{
	uint32_t counter =0;
	uint32_t read;
	LED_Init();
	SWITCH_Init();
	while (1){
	SWITCH_Get_Read(0, &read);
	if (read==1){
		LED_Set_State(0, 1);
		counter++;
	}
	if(counter%2==0){
		LED_Set_State(0, 0);
	}
	}
}
