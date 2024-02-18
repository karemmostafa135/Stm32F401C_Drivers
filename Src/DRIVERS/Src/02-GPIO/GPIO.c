#include "GPIO_interface.h"
#include "STD_types.h"


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




GPIO_Error_t GPIO_PinConfig(Pin_Config_t *Pin_Config){
	u32 Loc_PUPDR_Helper,Loc_MODER_Helper,Loc_OTYPER_Helper,Loc_OSPEEDR_Helper;

	GPIO_Error_t Loc_Error_Status=GPIO_Ok;

	if ((Pin_Config->Pin_num<GPIO_PIN0)||(Pin_Config->Pin_num>GPIO_PIN15)){
		Loc_Error_Status=GPIO_Wrong_Pin_Config;
	}
	else if((Pin_Config->Speed>GPIO_SPEED_VERY_HIGH)||(Pin_Config->Speed>GPIO_SPEED_LOW)){
		Loc_Error_Status=GPIO_Wrong_Speed_Config;
	}
	else if((Pin_Config->Mode>GPIO_MODE_INPUT_FLOATING)||(Pin_Config->Mode>GPIO_MODE_ANALOG)) {
		Loc_Error_Status=GPIO_Wrong_Mode_Config;
	}
	else {


		/************** to configure the PUPDR Register ********/
		Loc_PUPDR_Helper=((GPIO_t*)(Pin_Config->Port_num))->PUPDR;
		Loc_PUPDR_Helper&=~(RESET_PUPDR<<(2*Pin_Config->Pin_num));
		Loc_PUPDR_Helper|=((GET_PUPDR&Pin_Config->Mode)<<(2*Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->PUPDR=Loc_PUPDR_Helper;

		/************** to configure the MODER Register ********/
		Loc_MODER_Helper=((GPIO_t*)(Pin_Config->Port_num))->MODER;
		Loc_MODER_Helper&=~(RESET_MODER<<(2*Pin_Config->Pin_num));
		Loc_MODER_Helper|=((GET_MODER&Pin_Config->Mode)<<(2*Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->MODER=Loc_MODER_Helper;

		/************** to configure the OTYPER Register ********/
		Loc_OTYPER_Helper=((GPIO_t*)(Pin_Config->Port_num))->OTYPER;
		Loc_OTYPER_Helper&=~(RESET_OTYPER<<(2*Pin_Config->Pin_num));
		Loc_OTYPER_Helper|=((GET_OTYPER&Pin_Config->Mode)<<(2*Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->OTYPER=Loc_OTYPER_Helper;

		/************** to configure the OSPEEDR Register ********/
		Loc_OSPEEDR_Helper=((GPIO_t*)(Pin_Config->Port_num))->OSPEEDR;
		Loc_OSPEEDR_Helper&=~(RESET_SPEED<<(2*Pin_Config->Pin_num));
		Loc_OSPEEDR_Helper|= (Pin_Config->Speed<<(2*Pin_Config->Pin_num));
		((GPIO_t*)(Pin_Config->Port_num))->OSPEEDR=Loc_OSPEEDR_Helper;
	}
	return Loc_Error_Status;
}


GPIO_Error_t GPIO_Set_Pin_Value(void *port, u32 Pin_num, u32 value) {
    GPIO_Error_t Error_status = GPIO_Ok;

    switch(value) {
        case PIN_HIGH:
            ((GPIO_t*)(port))->BSRR |= (1U << Pin_num);
            break;
        case PIN_LOW:
            ((GPIO_t*)(port))->BSRR |= (1U << (Pin_num + HALF_WORD));
            break;
        default:
            Error_status = GPIO_Nok;
            break;
    }

    return Error_status;
}


u32 GPIO_Get_Pin_Value(void * port,u32 Pin_num){
	return (((GPIO_t*)(port))->IDR&(1<<Pin_num)) ;
}


