#include "Sched.h"
#include "Sched_Cfg.h"


extern void Traffic_Lights(void);

extern void led_on(void)

{
	LED_Set_State(0, 1);

}

extern void led_off(void){

		LED_Set_State(0, 0);

}





const Runnable_t rInfo[_Runnable_Count]={
		[Runnable_1]={
				.periodicty=150,
				.Cb=led_off
		}
		,
		[Runnable_2]={
				.periodicty=100,
				.Cb=led_on
		}
		,
		[Traffic_Lightss]={
				.periodicty=2000,
				.Cb=Traffic_Lights	}

};
