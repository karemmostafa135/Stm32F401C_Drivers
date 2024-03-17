#include <stdio.h>

#include "NVIC.h"

#define SCB_AIRCR_VECTKEY        (0x5FA << 16)

#define SCB_AIRCR_VECTKEY_CLEAR_MASK   0x0000FFFF

#define SUB_GROUP_OFFSET			8

#define PRIORITY_OFFSET				8

#define MAX_NUM_OF_INTERRUPTS 		84

#define CLEAR_GROUPING				0xFFFFF8FF

#define MIN_NUM_OF_INTERRUPTS		0

#define MAX_PRIORITY_VALUE			15

#define MIN_PRIORITY_VALUE			0

#define NVIC_BASE_ADDRESS		0xE000E100

#define reg ((NVIC_Registers_t *)(NVIC_BASE_ADDRESS))

#define SCB_AIRCR    *((volatile uint32_t *)0xE000ED0C)

typedef unsigned char uint8_t;

	typedef struct{
			uint32_t NVIC_ISER[8]; //Interrupt set-enable register
			uint32_t Reserved1[24];
			uint32_t NVIC_ICER[8]; //Interrupt clear-enable register
			uint32_t Reserved2[24];
			uint32_t NVIC_ISPR[8]; //Interrupt set-pending register
			uint32_t Reserved3[24];
			uint32_t NVIC_ICPR[8]; //Interrupt clear-pending register
			uint32_t Reserved4[24];
			uint32_t NVIC_IABR[8]; //Interrupt active bit register
			uint32_t Reserved5[56];
			uint8_t NVICIPR[240]; //Interrupt priority register written as u8 as ever IRQ has a 8 bits represeting its priorty
			uint32_t Reserved6[644];
			uint32_t NVIC_STIR; //Software trigger interrupt register
		}NVIC_Registers_t;


#define NVIC_BASE_ADDRESS		0xE000E100

#define reg ((NVIC_Registers_t *)(NVIC_BASE_ADDRESS))



	NVIC_Errors_t NVIC_EnableIRQ(uint32_t Copy_IRQValue){
		NVIC_Errors_t Loc_Error=NVIC_Ok;
		uint32_t Register_Helper=0,Pin_Helper=0;
		if ((Copy_IRQValue<0)||(Copy_IRQValue>MAX_NUM_OF_INTERRUPTS)){
			Loc_Error=NVIC_Nok;
		}
		else{
		NVIC_ClearPending(Copy_IRQValue);
		Register_Helper=Copy_IRQValue/32;
		Pin_Helper=Copy_IRQValue%32;
		reg->NVIC_ISER[Register_Helper]|=(0x01<<Pin_Helper);
		}
		return Loc_Error;
	}


	NVIC_Errors_t NVIC_DisableIRQ(uint32_t Copy_IRQValue){
		NVIC_Errors_t Loc_Error=NVIC_Ok;
				uint32_t Register_Helper=0,Pin_Helper=0;
				if ((Copy_IRQValue<0)||(Copy_IRQValue>MAX_NUM_OF_INTERRUPTS)){
					Loc_Error=NVIC_Nok;
				}
				else{
				Register_Helper=Copy_IRQValue/32;
				Pin_Helper=Copy_IRQValue%32;
				reg->NVIC_ICER[Register_Helper]=(0x01<<Pin_Helper);
				}
				return Loc_Error;
	}


	NVIC_Errors_t NVIC_SetPending(uint32_t Copy_IRQValue){
			NVIC_Errors_t Loc_Error=NVIC_Ok;
			uint32_t Register_Helper=0,Pin_Helper=0;
			if ((Copy_IRQValue<0)||(Copy_IRQValue>MAX_NUM_OF_INTERRUPTS)){
				Loc_Error=NVIC_Nok;
			}
			else{
			Register_Helper=Copy_IRQValue/32;
			Pin_Helper=Copy_IRQValue%32;
			reg->NVIC_ISPR[Register_Helper]|=(0x01<<Pin_Helper);
			}
						return Loc_Error;
	}


	NVIC_Errors_t NVIC_GetPending(uint32_t Copy_IRQValue,uint32_t *Pending_value){
		NVIC_Errors_t Loc_Error=NVIC_Ok;
					uint32_t Register_Helper=0,Pin_Helper=0;
					if ((Copy_IRQValue<0)||(Copy_IRQValue>MAX_NUM_OF_INTERRUPTS)){
						Loc_Error=NVIC_Nok;
					}
					else{
					Register_Helper=Copy_IRQValue/32;
					Pin_Helper=Copy_IRQValue%32;
					* Pending_value=reg->NVIC_ISPR[Register_Helper]&(0x01<<Pin_Helper);
					}
								return Loc_Error;

	}

	NVIC_Errors_t NVIC_ClearPending(uint32_t Copy_IRQValue){
					NVIC_Errors_t Loc_Error=NVIC_Ok;
					uint32_t Register_Helper=0,Pin_Helper=0;
					if ((Copy_IRQValue<0)||(Copy_IRQValue>MAX_NUM_OF_INTERRUPTS)){
						Loc_Error=NVIC_Nok;
					}
					else{
					Register_Helper=Copy_IRQValue/32;
					Pin_Helper=Copy_IRQValue%32;
					reg->NVIC_ICPR[Register_Helper]=(0x01<<Pin_Helper);
					}
						return Loc_Error;
	}


	NVIC_Errors_t NVIC_GetActiv(uint32_t Copy_IRQValue,uint32_t * Copy_Acitve_Value){
		NVIC_Errors_t Loc_Error=NVIC_Ok;
		uint32_t Register_Helper=0,Pin_Helper=0;
		if ((Copy_IRQValue<MIN_NUM_OF_INTERRUPTS)||(Copy_IRQValue>MAX_NUM_OF_INTERRUPTS)){
			Loc_Error=NVIC_Nok;
		}
		else if (Copy_Acitve_Value==NULL){
			Loc_Error=NVIC_NullPtr;
		}
		else{
		Register_Helper=Copy_IRQValue/32;
		Pin_Helper=Copy_IRQValue%32;
		* Copy_Acitve_Value=(reg->NVIC_IABR[Register_Helper]&(0x01<<Pin_Helper));
		}
				return Loc_Error;
	}

NVIC_Errors_t NVIC_SetPriority(uint32_t Copy_IRQValue,uint32_t Copy_Priorty,uint32_t Copy_GroupingPins){
	NVIC_Errors_t Loc_Error=NVIC_Ok;
	uint32_t AIRCR_Helper=0;
	if ((Copy_IRQValue<MIN_NUM_OF_INTERRUPTS)||(Copy_IRQValue>MAX_NUM_OF_INTERRUPTS)){
				Loc_Error=NVIC_Nok;
			}
	else if ((Copy_Priorty<MIN_PRIORITY_VALUE)||(Copy_Priorty>MAX_PRIORITY_VALUE)){
		Loc_Error=NVIC_Nok;
	}
	else if ((Copy_GroupingPins<NO_GROUPING)||(Copy_GroupingPins>FOUR_GROUP_PIN)){
		Loc_Error=NVIC_Nok;
	}
	else {
		AIRCR_Helper=SCB_AIRCR;
		AIRCR_Helper&=(SCB_AIRCR_VECTKEY_CLEAR_MASK|CLEAR_GROUPING);
		AIRCR_Helper|=(SCB_AIRCR_VECTKEY|Copy_GroupingPins<<SUB_GROUP_OFFSET); //to set the grouping value
		SCB_AIRCR=AIRCR_Helper;
		reg->NVICIPR[Copy_IRQValue]|=(Copy_Priorty<<PRIORITY_OFFSET); //to set the whole priority value

	}
	return Loc_Error;
}

