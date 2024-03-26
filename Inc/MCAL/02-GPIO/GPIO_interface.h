#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H


/****** pin value options *****/
#define PIN_HIGH		1
#define PIN_LOW 		0


/***defines for the pins ******/
#define GPIO_PIN0   0x00000000
#define GPIO_PIN1   0x00000001
#define GPIO_PIN2   0x00000002
#define GPIO_PIN3   0x00000003
#define GPIO_PIN4   0x00000004
#define GPIO_PIN5   0x00000005
#define GPIO_PIN6   0x00000006
#define GPIO_PIN7   0x00000007
#define GPIO_PIN8   0x00000008
#define GPIO_PIN9   0x00000009
#define GPIO_PIN10  0x0000000A
#define GPIO_PIN11  0x0000000B
#define GPIO_PIN12  0x0000000C
#define GPIO_PIN13  0x0000000D
#define GPIO_PIN14  0x0000000E
#define GPIO_PIN15  0x0000000F

/*************** the available modes *****/
#define GPIO_MODE_INPUT_FLOATING 0x00000000
#define GPIO_MODE_INPUT_PU       0x00000001
#define GPIO_MODE_INPUT_PD       0x00000002

#define GPIO_MODE_OUTPUT_PP        0x00000008
#define GPIO_MODE_OUTPUT_PP_PU     0x00000009
#define GPIO_MODE_OUTPUT_PP_PD     0x0000000A

#define GPIO_MODE_OUTPUT_OPENDRAIN          0x0000000C
#define GPIO_MODE_OUTPUT_OPENDRAIN_PU       0x0000000D
#define GPIO_MODE_OUTPUT_OPENDRAIN_PD       0x0000000E

#define GPIO_MODE_AF_PP             0x00000010
#define GPIO_MODE_AF_PP_PU          0x00000011
#define GPIO_MODE_AF_PP_PD          0x00000012

#define GPIO_MODE_AF_OPENDRAIN      0x00000014
#define GPIO_MODE_AF_OPENDRAIN_PU   0x00000015
#define GPIO_MODE_AF_OPENDRAIN_PD   0x00000016

#define GPIO_MODE_ANALOG            0x00000018


/********* listing the available ports *****/
#define   GPIOA	 ((void *)(0x40020000))

#define   GPIOB	 ((void *)(0x40020400))

#define   GPIOC	 ((void *)(0x40020800))

typedef unsigned int uint32_t;



typedef enum {
	GPIO_Nok,
	GPIO_Ok,
	GPIO_Wrong_Port_Config,
	GPIO_Wrong_Pin_Config,
	GPIO_Wrong_Speed_Config,
	GPIO_Wrong_Mode_Config
}GPIO_Error_t;


typedef enum {
	GPIO_SPEED_LOW,
	GPIO_SPEED_MEDIUM,
	GPIO_SPEED_HIGH,
	GPIO_SPEED_VERY_HIGH
}
GPIO_Speed_t;


typedef struct{
	volatile void * Port_num;
	volatile uint32_t Pin_num;
	volatile uint32_t Mode;
	volatile GPIO_Speed_t Speed;
	volatile uint32_t Af;
}Pin_Config_t;

/************* to configure the pin actually on hardware *******/
GPIO_Error_t GPIO_PinConfig(Pin_Config_t *Pin_Config);

/****
 * @options of port
 * 1/GPIOA
 * 2/GPIOB
 * 3/GPIOC
 *@options for value
 *1/PIN_HIGH
 *1/PIN_LOW
 */


GPIO_Error_t GPIO_Set_Pin_Value(void * port,uint32_t Pin_num,uint32_t value);


GPIO_Error_t GPIO_Get_Pin_Value(void * port,uint32_t Pin_num,uint32_t* Read);
#endif
