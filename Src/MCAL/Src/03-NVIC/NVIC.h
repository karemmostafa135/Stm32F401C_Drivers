#ifndef NVIC_H
#define NVIC_H

#include <stdio.h>


#define NVIC_WINDOW_WATCHDOG_TIMER		0
#define	NIVC_EXTERNAL_LINE16			1
#define	NIVC_EXTERNAL_LINE21			2
#define	NIVC_EXTERNAL_LINE22			3
#define	NIVC_FLASH_INTERRUPT			4
#define	NVIC_RCC_INTERRUPT				5
#define NVIC_EXT0_INTERRRUPT			6
#define NVIC_EXT1_INTERRRUPT			7
#define NVIC_EXT2_INTERRRUPT			8
#define NVIC_EXT3_INTERRRUPT			9
#define NVIC_EXT4_INTERRRUPT			10
#define NVIC_DMA1_STREAM0				11
#define NVIC_DMA1_STREAM1				12
#define NVIC_DMA1_STREAM2				13
#define NVIC_DMA1_STREAM3				14
#define NVIC_DMA1_STREAM4				15
#define NVIC_DMA1_STREAM5				16
#define NVIC_DMA1_STREAM6               17
#define NVIC_ADC_INTERRUPT              18
#define NVIC_EXT9_INTERRUPT             23
#define NVIC_TIM1_BRK_INTERRUPT         24
#define NVIC_TIM1_UP_INTERRUPT          25
#define NVIC_TIMI1_TRG_INTERRUPT        26
#define NVIC_TIMI1_CC_INTERRUPT         27
#define NVIC_TIMI2_INTERRUPT            28
#define NVIC_TIMI3_INTERRUPT            29
#define NVIC_TIMI4_INTERRUPT            30
#define NVIC_I2C1_EV_INTERRUPT          31
#define NVIC_I2C1_ER_INTERRUPT          32
#define NVIC_I2C2_EV_INTERRUPT          33
#define NVIC_I2C2_ER_INTERRUPT          34
#define NVIC_SPI1_INTERRUPT             35
#define NVIC_SPI2_INTERRUPT             36
#define NVIC_USART1_INTERRUPT           37
#define NVIC_USART2_INTERRUPT           38
#define NVIC_EXTI15_INTERRUPT           40
#define NVIC_EXTI17_INTERRUPT           41
#define NVIC_EXTI18_INTERRUPT           42
#define NVIC_DMA1_INTERRUPT             47
#define NVIC_SDIO_INTERRUPT             49
#define NVIC_TIM5_INTERRUPT             50
#define NVIC_SPI3_INTERRUPT             51
#define NVIC_DMA2_STREAM0_INTERRUPT     56
#define NVIC_DMA2_STREAM1_INTERRUPT     57
#define NVIC_DMA2_STREAM2_INTERRUPT     58
#define NVIC_DMA2_STREAM3_INTERRUPT     59
#define NVIC_DMA2_STREAM4_INTERRUPT     60
#define NVIC_OTG_FS_INTERRUPT           67
#define NVIC_DMA2_STREAM5_INTERRUPT     68
#define NVIC_DMA2_STREAM6_INTERRUPT     69
#define NVIC_DMA2_STREAM7_INTERRUPT     70
#define NVIC_USART6_INTERRUPT           71
#define NVIC_I2C3_EV_INTERRUPT          72
#define NVIC_I2C3_ER_INTERRUPT          73
#define NVIC_FPU_INTERRUPT              81
#define NVIC_SPI4_INTERRUPT             84

/************************* grouping options ****************/
#define NO_GROUPING			0x03
#define ONE_GROUP_PIN		0x04
#define TWO_GROUP_PIN		0x05
#define THREE_GROUP_PIN		0x06
#define FOUR_GROUP_PIN		0x07

/************************* priority options ***************/
#define PRIORITY_0    0
#define PRIORITY_1    1
#define PRIORITY_2    2
#define PRIORITY_3    3
#define PRIORITY_4    4
#define PRIORITY_5    5
#define PRIORITY_6    6
#define PRIORITY_7    7
#define PRIORITY_8    8
#define PRIORITY_9    9
#define PRIORITY_10   10
#define PRIORITY_11   11
#define PRIORITY_12   12
#define PRIORITY_13   13
#define PRIORITY_14   14
#define PRIORITY_15   15


typedef unsigned int	uint32_t;


typedef enum {
	NVIC_Nok,
	NVIC_Ok,
	NVIC_WrongInput,
	NVIC_NullPtr
}NVIC_Errors_t;

/*
 *@options from 0 to 84
 *@brief to enable the interrupt for a peripheral
 */

NVIC_Errors_t NVIC_EnableIRQ(uint32_t Copy_IRQValue);

/*
 *@options from 0 to 84
 *@brief to disable the interrupt for a peripheral
 */

NVIC_Errors_t NVIC_DisableIRQ(uint32_t Copy_IRQValue);

/*
 *@options from 0 to 84
 *@brief to disable the interrupt for a peripheral
 *@return 0 represent inactive and 1 represent active
 */


NVIC_Errors_t NVIC_GetActiv(uint32_t Copy_IRQValue,uint32_t * Copy_Acitve_Value);


NVIC_Errors_t NVIC_SetPending(uint32_t Copy_IRQValue);

NVIC_Errors_t NVIC_ClearPending(uint32_t Copy_IRQValue);
/*
 *@options for group value
 1/NO_GROUPING	2/ONE_GROUP_PIN	3/TWO_GROUP_PIN	4/THREE_GROUP_PIN 5/FOUR_GROUP_PIN
 *@options for priority
 *from PRIORITY_0 TO PRIORITY_15
 *@options for IRQ
 *from 0 to 84
 *
 */
NVIC_Errors_t NVIC_SetPriority(uint32_t Copy_IRQValue,uint32_t Copy_Priorty,uint32_t Copy_GroupingPins);

NVIC_Errors_t NVIC_GetPriority(NVIC_Errors_t);




#endif
