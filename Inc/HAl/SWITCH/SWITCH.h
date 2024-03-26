#ifndef SWITCH_H
#define SWITCH_H

#include "STD_types.h"

typedef enum {
	SWITCH_Nok,
	SWITCH_Ok
}SWITCH_Errors_t;


typedef struct{
		void * port;
		uint32_t pin;
		uint8_t mode;
}SWITCH_t;

void SWITCH_Init();

void Switch_task();

/*
 * @brief
 * if Read=1 this means switch is Pressed  if Read=0 this means switch is Not Pressed
 */
SWITCH_Errors_t SWITCH_Get_Read(uint32_t Switch_num,uint32_t* Read);



SWITCH_Errors_t SWITCH_Get_Status(uint32_t Switch_num,uint32_t* Read);
#endif
