#include "STD_types.h"
#include "Traffic.h"
#include "LED.h"

#define TRAFFIC_PERIODICTY			2

State_t state=green;
// function every 2 seconds
void Traffic_Lights(void){
	static uint32_t seconds=0;
	seconds+=TRAFFIC_PERIODICTY;
	switch(state){
	case red:
		LED_Set_State(green, LED_STATE_OFF);
		LED_Set_State(yellow, LED_STATE_OFF);
		LED_Set_State(red, LED_STATE_ON);
				if(seconds==4){
					state=yellow;
				}
				break;
	case yellow:
			LED_Set_State(green, LED_STATE_OFF);
			LED_Set_State(red, LED_STATE_OFF);
			LED_Set_State(yellow, LED_STATE_ON);
					if(seconds==6){
						state=green;
					}
					break;
	case green:
			LED_Set_State(red, LED_STATE_OFF);
			LED_Set_State(yellow, LED_STATE_OFF);
			LED_Set_State(green, LED_STATE_ON);
			if(seconds==12){
				state=red;
				seconds=0;
			}
			break;

	}

}
