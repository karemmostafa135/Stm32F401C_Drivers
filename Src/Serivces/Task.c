#include "LED.h"
#include "SWITCH.h"


void led_on1(void)
{
	int x=0;
	SWITCH_Get_Status(0,&x);
	if(x==1){
	LED_Set_State(0, 1);
	}
	else{
		LED_Set_State(0, 0);
	}
}

 void led_on2(void){

		//LED_Set_State(1, 1);

}


 void led_on3(void){

		LED_Set_State(2, 1);

}


