
#include "STD_types.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "LED.h"
#include "NVIC.h"
#include "SYSTICK.h"
#include "Sched.h"
#include "UART.h"
#include "LCD.h"

void led_(void){}

int main(void)
{
	RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOAEN, ENABLE_PERI);
		RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOBEN, ENABLE_PERI);
	Pin_Config_t Tx;
		Pin_Config_t Rx;
		Tx.Port_num=GPIOB;
		Tx.Pin_num=6;
		Tx.Mode=GPIO_MODE_AF_PP;
		Tx.Speed=GPIO_SPEED_MEDIUM;
		Tx.Af=7;
		GPIO_PinConfig(&Tx);
		Rx.Port_num=GPIOB;
	   	Rx.Pin_num=7;
	   	Rx.Mode=GPIO_MODE_AF_OPENDRAIN_PU;
	   	Rx.Speed=GPIO_SPEED_MEDIUM;
	    Rx.Af=7;
	    GPIO_PinConfig(&Rx);

		uint8_t arr[30]={'k','A','F',11,23,25,6,7,8,12,9,14,35,65,88,100,30,15,20,18,20,33,74};
			Buffer_t buf1;
			buf1.data=arr;
			buf1.size=30;
			buf1.Channel=UART1;
			Uart_ConfigType c1={0};
			c1.BaudRate=9600;
			c1.Channel=UART1;
			c1.Oversampling=0;
			c1.ParityControl=0;
			c1.ReceiveDateRegisterEmpty_Int=1;
			c1.WordLength=0;
			c1.TransComplete_Int=1;
			c1.TransDateRegisterEmpty_Int=0;
			c1.UartEnable=1;
			UART_Init(&c1);
			UART_SendData_AsyncZeroCopy(&buf1,led_);
			//UART_SendByte(&buf1);
			while (1)
			{
				for(uint16_t rx=0;rx<1000;rx){
				uint32_t x=2;
				GPIO_PinConfig(&rx);
			}
			}
}
/* to test gpio ,led and switch
uint32_t counter =0;
	uint32_t read;
	LED_Init();
	SWITCH_Init();
	LED_Set_State(0, 1);
	while (1){
	SWITCH_Get_Read(0, &read);
	if (read==1){
		LED_Set_State(0, 1);
		counter++;
	}
	if(counter%2==0){
		LED_Set_State(0, 0);
	}
*/

//testing the systick
/*
unsigned int z=0;
	LED_Init();
	SYSTICK_Setms(1000);
	SYSTICK_Start();
	SYSTICK_Set_CallBack(led_on);
	while(1){
		int x=5;
		x++;
	}
*/

/******* testing the scheduler******/
 /*
	LED_Init();
	SWITCH_Init();
	Sched_Init();
	Sched_Start();
 */


/** Pin_Config_t Tx;
	Pin_Config_t Rx;
	Tx.Port_num=GPIOB;
	Tx.Pin_num=6;
	Tx.Mode=GPIO_MODE_AF_PP;
	Tx.Speed=GPIO_SPEED_MEDIUM;
	Tx.Af=7;
	GPIO_PinConfig(&Tx);
	Rx.Port_num=GPIOB;
   	Rx.Pin_num=7;
   	Rx.Mode=GPIO_MODE_AF_PP;
   	Rx.Speed=GPIO_SPEED_MEDIUM;
    Rx.Af=7;
    GPIO_PinConfig(&Rx);

	uint8_t arr[30]={11,23,25,6,7,8,12,9,14,35,65,88,100,30,15,20,18,20,33,74};
		Buffer_t buf1;
		buf1.data=arr;
		buf1.size=30;
		buf1.Channel=UART1;
		Uart_ConfigType c1={0};
		c1.BaudRate=9600;
		c1.Channel=UART1;
		c1.Oversampling=0;
		c1.ParityControl=0;
		c1.ReceiveDateRegisterEmpty_Int=1;
		c1.WordLength=0;
		c1.TransComplete_Int=1;
		c1.TransDateRegisterEmpty_Int=0;
		c1.UartEnable=1;
		UART_Init(&c1);
		UART_SendData_AsyncZeroCopy(&buf1,led_);
		//UART_SendByte(&buf1);
		while (1)
		{
			for(uint16_t rx=0;rx<1000;rx){
			uint32_t x=2;
			GPIO_PinConfig(&rx);
		}
}
 */

/* testing the LCD
 *
 * uint8_t *name={"helloo"};
	RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOAEN, ENABLE_PERI);
	RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOBEN, ENABLE_PERI);
	LCD_Init_Asynch();
	LCD_Write_String_NoCopy(name,6, 2, 3);
	Sched_Init();
	Sched_Start();
 */

