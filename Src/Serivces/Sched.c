#include "SYSTICK.h"
#include "Sched.h"
#include "Sched_Cfg.h"

static uint32_t Global_Number_of_pending_Ticks=0;
static uint32_t Global_Number_of_Sys_Ticks=0;


extern const Runnable_t rInfo[_Runnable_Count];

void led(void){

}

void Sched_Init(){
	SYSTICK_Set_Prescaler(NO_PRESCALER);
	SYSTICK_Setms(SCHED_TIME_IN_MS);
	SYSTICK_Set_CallBack(Pending_Ticks_Counting);

}


void Sched(){
	uint32_t idx=0;
	Global_Number_of_Sys_Ticks+=SCHED_TIME_IN_MS;
	for(idx;idx<_Runnable_Count;idx++){
			if((rInfo[idx].Cb)&&!(Global_Number_of_Sys_Ticks%rInfo[idx].periodicty)){
				rInfo[idx].Cb();
			}
	}

}

void Sched_Start(){
	SYSTICK_Start();
	while(1){
			if(Global_Number_of_pending_Ticks){
				Global_Number_of_pending_Ticks--;
				Sched();
			}

		}
}

void Sched_Stop(){


}

void Pending_Ticks_Counting(){
	Global_Number_of_pending_Ticks++;
}
