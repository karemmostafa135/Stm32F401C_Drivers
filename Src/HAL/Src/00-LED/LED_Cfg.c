#include "GPIO_interface.h"
#include "LED.h"
#include "LED_Cfg.h"

const led_cfg_t leds[_led_num]={
		[Alarm_Led]={.port=GPIOA,
		.pin=GPIO_PIN0,
		.connection=LED_CONN_FORWARD,
		.current_state=LED_STATE_ON
		},
		 [Second_Led]={.port=GPIOA,
		 		.pin=GPIO_PIN1,
		 		.connection=LED_CONN_FORWARD,
		 		.current_state=LED_STATE_OFF
		 		}
};


