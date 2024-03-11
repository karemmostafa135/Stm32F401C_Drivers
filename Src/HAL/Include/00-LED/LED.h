#ifndef LED_H
#define LED_H

#include "STD_types.h"

/*************** connect options ******/
#define LED_CONN_FORWARD 		0
#define LED_CONN_BACKWORD		1

#define LED_STATE_ON			1
#define LED_STATE_OFF			0



typedef struct{
	void * port;
	uint32_t pin;
	uint8_t connection;
	uint8_t current_state;
}led_cfg_t;

void LED_Init();

void LED_Set_State(uint32_t led,uint8_t status);

#endif
