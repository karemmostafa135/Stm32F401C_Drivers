#include "LCD.h"
#include "GPIO_interface.h"

LCD_pin_Cfg LCD_Pins_Config[NUMBER_OF_LCD_PINS]={
[D0_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN0
},

[D1_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN1
},
[D2_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN2
},
[D3_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN3
},
[D4_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN4
},
[D5_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN5
},
[D6_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN6
},
[D7_PIN]={
		.port=GPIOA,
		.pin=GPIO_PIN7
},
[RS_PIN]={
		.port=GPIOB,
		.pin=GPIO_PIN8
},
[RW_PIN]={
		.port=GPIOB,
		.pin=GPIO_PIN9
},
[EN_PIN]={
		.port=GPIOB,
		.pin=GPIO_PIN10
},
};


