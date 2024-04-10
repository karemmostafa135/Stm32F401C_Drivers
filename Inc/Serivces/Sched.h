#ifndef SCHED_H
#define SCHED_H

#include "STD_types.h"

#define SCHED_TIME_IN_MS			2

typedef void (*RunnableCb_t)(void);


typedef struct{
	uint32_t periodicty;
	sint32_t First_Delay;
	RunnableCb_t	Cb;

}Runnable_t;

void Sched_Init();

void Sched();

void Sched_Start();

void Sched_Stop();

void Pending_Ticks_Counting();

#endif
