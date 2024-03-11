#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "LED.h"
#include "LED_Cfg.h"

extern const led_cfg_t leds[_led_num];

void LED_Init(){
	RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOAEN, ENABLE_PERI);
	RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOBEN, ENABLE_PERI);
	RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOCEN, ENABLE_PERI);
	uint32_t counter=0;
	Pin_Config_t pin;
	pin.Mode=GPIO_MODE_OUTPUT_PP;
	pin.Speed=GPIO_SPEED_HIGH;
	for(counter=0;counter<_led_num;counter++){
		pin.Port_num=leds[counter].port;
		pin.Pin_num=leds[counter].pin;
		GPIO_PinConfig(&pin);
		GPIO_Set_Pin_Value(
				leds[counter].port,
				leds[counter].pin,
				(leds[counter].current_state^leds[counter].connection));
}
}
	void LED_Set_State(uint32_t led,uint8_t status){
		GPIO_Set_Pin_Value(
						leds[led].port,
						leds[led].pin,
						leds[led].connection^status);
	}
