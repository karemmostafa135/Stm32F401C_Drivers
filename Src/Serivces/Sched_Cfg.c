#include "Sched.h"
#include "Sched_Cfg.h"

typedef enum {
	SWITCH_Nok,
	SWITCH_Ok
}SWITCH_Errors_t;


extern void Switch_task();

extern void LCD_Task();


extern void Write_Date_Time_Task();


extern void Sec_Increment_Task(void);



 Runnable_t rInfo[_Runnable_Count]={
		[Runnable_1]={
				.periodicty=2,
				.Cb=LCD_Task,
				.First_Delay=0
		}
		,
		[Runnable_2]={
				.periodicty=2,
				.Cb=Write_Date_Time_Task,
				.First_Delay=25

		}
		,
		[Traffic_Lightss]={
				.periodicty=1000,
				.Cb=Sec_Increment_Task,
				.First_Delay=50
		}
	};
