#include "GPIO_interface.h"
#include "STD_types.h"

/****************** for validation ***************/
#define NULL				0
#define MAX_PINS_NUMBER		15

#define HALF_WORD			16
/******** Get Pin Configuration mask *******/
#define GET_PUPDR		0x3
#define GET_OTYPER		0x4
#define GET_MODER		0x18

/******************** Reset Masks ***********/
#define RESET_PUPDR		0x3
#define RESET_MODER		0x3
#define RESET_OTYPER	0x1
#define RESET_SPEED		0x3


typedef struct {
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
}GPIO_t;



GPIO_Error_t GPIO_PinConfig(Pin_Config_t *Pin_Config){
	uint32_t Loc_Helper,Loc_PUPDR_Value,Loc_MODER_Value,Loc_OTYPER_Value;

	GPIO_Error_t Loc_Error_Status=GPIO_Ok;

	if ((Pin_Config->Pin_num<GPIO_PIN0)||(Pin_Config->Pin_num>GPIO_PIN15)){
		Loc_Error_Status=GPIO_Wrong_Pin_Config;
	}
	else if((Pin_Config->Speed>GPIO_SPEED_VERY_HIGH)||(Pin_Config->Speed<GPIO_SPEED_LOW)){
		Loc_Error_Status=GPIO_Wrong_Speed_Config;
	}
	else if((Pin_Config->Mode<GPIO_MODE_INPUT_FLOATING)||(Pin_Config->Mode>GPIO_MODE_ANALOG)) {
		Loc_Error_Status=GPIO_Wrong_Mode_Config;
	}
	else {
		Loc_PUPDR_Value=(Pin_Config->Mode&GET_PUPDR);
		Loc_MODER_Value=((Pin_Config->Mode&GET_MODER)>>3);
		Loc_OTYPER_Value=((Pin_Config->Mode&GET_OTYPER)>>2);
		/************** to configure the PUPDR Register ********/

		Loc_Helper=((GPIO_t*)(Pin_Config->Port_num))->PUPDR;
		Loc_Helper&=~(RESET_PUPDR<<(2*Pin_Config->Pin_num));
		Loc_Helper|=(Loc_PUPDR_Value<<(2*Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->PUPDR=Loc_Helper;

		/************** to configure the MODER Register ********/
		Loc_Helper=((GPIO_t*)(Pin_Config->Port_num))->MODER;
		Loc_Helper&=~(RESET_MODER<<(2*Pin_Config->Pin_num));
		Loc_Helper|=(Loc_MODER_Value<<(2*Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->MODER=Loc_Helper;

		/************** to configure the OTYPER Register ********/
		Loc_Helper=((GPIO_t*)(Pin_Config->Port_num))->OTYPER;
		Loc_Helper&=~(RESET_OTYPER<<(Pin_Config->Pin_num));
		Loc_Helper|=(Loc_OTYPER_Value<<(Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->OTYPER=Loc_Helper;

		/************** to configure the OSPEEDR Register ********/
		Loc_Helper=((GPIO_t*)(Pin_Config->Port_num))->OSPEEDR;
		Loc_Helper&=~(RESET_SPEED<<(2*Pin_Config->Pin_num));
		Loc_Helper|= (Pin_Config->Speed<<(2*Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->OSPEEDR=Loc_Helper;
		if(Pin_Config->Af!=0){
			if(Pin_Config->Pin_num<=7){
				((GPIO_t*)(Pin_Config->Port_num))->AFRL|=(Pin_Config->Af<<(4*Pin_Config->Pin_num));
			}
				else if(Pin_Config->Pin_num>7){
					((GPIO_t*)(Pin_Config->Port_num))->AFRH|=(Pin_Config->Af<<(4*(Pin_Config->Pin_num-8)));
				}

		}
	}
	return Loc_Error_Status;
}


GPIO_Error_t GPIO_Set_Pin_Value(void *port, uint32_t Pin_num, uint32_t value) {
    GPIO_Error_t Error_status = GPIO_Ok;

    switch(value) {
        case PIN_HIGH:
            ((GPIO_t*)(port))->BSRR |= (1 << Pin_num);
            break;
        case PIN_LOW:
            ((GPIO_t*)(port))->BSRR |= (1 << (Pin_num + HALF_WORD));
            break;
        default:
            Error_status = GPIO_Nok;
            break;
    }

    return Error_status;
}


GPIO_Error_t GPIO_Get_Pin_Value(void * port,uint32_t Pin_num,uint32_t* Read){
	GPIO_Error_t Local_Error_Status=GPIO_Ok;
	if(port==NULL){
		Local_Error_Status=GPIO_Nok;
	}
	else if (Read==NULL){
		Local_Error_Status=GPIO_Nok;
	}
	else if (Pin_num>MAX_PINS_NUMBER){
		Local_Error_Status=GPIO_Nok;
	}
	else{
		*Read=(((GPIO_t*)(port))->IDR>>Pin_num)&1;
	}
	return Local_Error_Status ;
}


