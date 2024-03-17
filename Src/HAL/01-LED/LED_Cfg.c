#include "GPIO_interface.h"
#include "LED.h"
#include "LED_Cfg.h"

/*
 * @options
 ******************************************************************************************
 * 					1/ for port
 * 						1/GPIOA			2/GPIOB			3/GPIOC
 *******************************************************************************************
 * 					2/for pin
 * 					from  GPIO_PIN0 to GPIO_PIN15
*******************************************************************************************
 *					3/connection
 *					1/LED_CONN_FORWARD  	2/LED_CONN_BACKWORD
 *******************************************************************************************
 	 	 	 	 	4/current_state
 	 	 	 	 	1/LED_STATE_ON			2/LED_STATE_OFF
 */

const led_cfg_t leds[_led_num]={
		[Alarm_Led]={.port=GPIOA,
		.pin=GPIO_PIN0,
		.connection=LED_CONN_FORWARD,
		.current_state=LED_STATE_OFF
		},
		 [Second_Led]={.port=GPIOA,
		 		.pin=GPIO_PIN1,
		 		.connection=LED_CONN_FORWARD,
		 		.current_state=LED_STATE_OFF
		 		},
		[Third_led]	={.port=GPIOA,
		 		.pin=GPIO_PIN2,
		 		.connection=LED_CONN_FORWARD,
		 		.current_state=LED_STATE_OFF
		 		}
};


