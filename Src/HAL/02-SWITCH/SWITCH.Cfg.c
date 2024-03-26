#include "SWITCH_Cfg.h"
#include "SWITCH.h"
#include "GPIO_interface.h"
const SWITCH_t Switches[_Switch_num]={
		[Alarm_Switch]={
				.port=GPIOB,
				.pin=GPIO_PIN0,
				.mode=GPIO_MODE_INPUT_PU
		},
		[Second_Switch]={
				.port=GPIOB,
				.pin=GPIO_PIN2,
				.mode=GPIO_MODE_INPUT_PD
		}
};
