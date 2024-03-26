#include "Sched.h"
#include "Sched_Cfg.h"

typedef enum {
	SWITCH_Nok,
	SWITCH_Ok
}SWITCH_Errors_t;


extern void Traffic_Lights(void);

extern void Switch_task();



extern void led_on1(void);

extern void led_on2(void);

extern void led_on3(void);



const Runnable_t rInfo[_Runnable_Count]={
		[Runnable_1]={
				.periodicty=100,
				.Cb=led_on1
		}
		,
		[Runnable_2]={
				.periodicty=5,
				.Cb=Switch_task
		}
		,
		[Traffic_Lightss]={
				.periodicty=500000,
				.Cb=Traffic_Lights	}

};
