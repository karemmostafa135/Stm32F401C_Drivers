#include "Sched.h"
#include "Sched_Cfg.h"

extern void led(void);

const Runnable_t rInfo[_Runnable_Count]={
		[Runnable_1]={
				.periodicty=5,
				.Cb=led
		},
		[Runnable_2]={
				.periodicty=2,
				.Cb=led
		}
};
