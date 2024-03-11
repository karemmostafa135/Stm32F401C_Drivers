#include "STD_types.h"
#include "RCC_interface.h"

#define PLL_RESET_MNP_MASK 			0xFFFC0000
#define PLL_SELECT_SRC_BIT				22

/***************** clocks ready pins **********/
#define PLL_RDY				25
#define HSE_RDY				17
#define HSI_RDY				1

/**************** needed masks *********/
#define SYSCLK_RESET			0xFFFFFFFC
#define GET_SYS_CLK				0x00000003
#define RCC_BASE_ADDRESS		0x40023800

/*********** to make an instance refer to the RCC registers on memory *******/


typedef struct {
volatile uint32_t CR;			//to disable and enable clocks
volatile uint32_t PLLCFGR;
volatile uint32_t CFGR;
volatile uint32_t CIR;
volatile uint32_t AHB1RSTR;
volatile uint32_t AHB2RSTR;
volatile uint32_t reserved1[2];
volatile uint32_t APB1RSTR;
volatile uint32_t APB2RSTR;
volatile uint32_t reserved2[2];
volatile uint32_t AHB1ENR;
volatile uint32_t AHB2ENR;
volatile uint32_t reserved3[2];
volatile uint32_t APB1ENR;
volatile uint32_t APB2ENR;
volatile uint32_t reserved4[2];
volatile uint32_t AHB1LPENR;
volatile uint32_t AHB2LPENR;
volatile uint32_t reserved5[2];
volatile uint32_t APB1LPENR;
volatile uint32_t APB2LPENR;
volatile uint32_t reserved6[2];
volatile uint32_t BDCR;
volatile uint32_t CSR;
volatile uint32_t reserved7[2];
volatile uint32_t SSCGR;
volatile uint32_t RCC_PLLI2SCFGR;
volatile uint32_t reserved8;
volatile uint32_t DCKCFGR;
}RCC_Peri_t;

 RCC_Peri_t * const RCC=((RCC_Peri_t *)(RCC_BASE_ADDRESS));

RCC_Error_status_t RCC_EnableClock(uint32_t Copy_Clk){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
switch (Copy_Clk){
case HSI: RCC->CR|=HSI; break;
case HSE: RCC->CR|=HSE; break;
case PLL: RCC->CR|=PLL; break;
default : return Loc_ErrorStatus=RCC_Nok;
}
return Loc_ErrorStatus;
}

RCC_Error_status_t RCC_DisableClock(uint32_t Copy_Clk){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
switch (Copy_Clk){
case HSI: RCC->CR&=~HSI; break;
case HSE: RCC->CR&=~HSE; break;
case PLL: RCC->CR&=~PLL; break;
default : return Loc_ErrorStatus=RCC_Nok;
}
return Loc_ErrorStatus;
}

uint32_t RCC_Get_Ready_Status(uint32_t Get_read ){
	uint32_t ready_status;
switch(Get_read){
case HSI: ready_status=((RCC->CR>>HSI_RDY)&1); break;
case HSE: ready_status=((RCC->CR>>HSE_RDY)&1); break;
case PLL: ready_status=((RCC->CR>>PLL_RDY)&1); break;
default:   ready_status=2; 		break;
}
return ready_status;
}


RCC_Error_status_t RCC_Set_PLLSrc(uint32_t Copy_PllSrc){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
	switch (Copy_PllSrc){
	case HSI : RCC->PLLCFGR&=~(1<<PLL_SELECT_SRC_BIT); break;
	case HSE : RCC->PLLCFGR&=(1<<PLL_SELECT_SRC_BIT); break;
	default  : Loc_ErrorStatus=RCC_Nok;
	}
	return Loc_ErrorStatus;
}


RCC_Error_status_t RCC_PllConfig(uint32_t Copy_N_factor,uint32_t Copy_M_factor,uint32_t Copy_P_factor ){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
	uint32_t Local_Helper=0,P_Factor_Value;
	/** these are the max and minimum values that can be configured on M and N ***/
	if((Copy_N_factor<2)||(Copy_N_factor>432)||(Copy_M_factor<2)||(Copy_M_factor>63)){
		 Loc_ErrorStatus=RCC_Nok;
	}
	else{
		P_Factor_Value=((Copy_P_factor/2)-1);
		Local_Helper=RCC->PLLCFGR;
		Local_Helper&=PLL_RESET_MNP_MASK;
		Local_Helper|=(Copy_N_factor<<6)|(Copy_M_factor)|P_Factor_Value;
		RCC->PLLCFGR=Local_Helper;
	}
	return Loc_ErrorStatus;
}


RCC_Error_status_t RCC_Set_SysClk(uint32_t Copy_ClkSrc){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
	uint32_t Loc_SysClkHelper=RCC->CFGR;
	Loc_SysClkHelper&=SYSCLK_RESET;
	switch (Copy_ClkSrc){
	case SYSCLK_HSI: Loc_SysClkHelper|=SYSCLK_HSI; 	 break;
	case SYSCLK_HSE: Loc_SysClkHelper|=SYSCLK_HSE;    break;
	case SYSCLK_PLL: Loc_SysClkHelper|=SYSCLK_PLL; 	 break;
	default  : Loc_ErrorStatus=RCC_Nok;
	}
	RCC->CFGR=Loc_SysClkHelper;
	return Loc_ErrorStatus;
}

void RCC_Get_SysClk(uint32_t* Copy_ClkSrc){
	*Copy_ClkSrc	=((RCC->CFGR>>2)&GET_SYS_CLK);
	return ((RCC->CFGR>>2)&GET_SYS_CLK); // to get the two digits which repersents the current system clock
}


RCC_Error_status_t RCC_AHB1ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
if((Copy_Peri>31)||(Copy_Peri<0)){
	return Local_ErrorStatus=RCC_Nok;
}
else{
	switch (Copy_Peri_Mode){
	case ENABLE_PERI: RCC->AHB1ENR|=(1<<Copy_Peri);	  break;
	case DISABLE_PERI: RCC->AHB1ENR&=~(1<<Copy_Peri); break;
	default : return Local_ErrorStatus=RCC_Nok;		  break;
	}
}
return Local_ErrorStatus;
}


RCC_Error_status_t RCC_AHB2ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
	if((Copy_Peri>31)||(Copy_Peri<0)){
		return Local_ErrorStatus=RCC_Nok;
	}
	else{
		switch (Copy_Peri_Mode){
		case ENABLE_PERI: RCC->AHB2ENR|=(1<<Copy_Peri);	  break;
		case DISABLE_PERI: RCC->AHB2ENR&=~(1<<Copy_Peri); break;
		default : return Local_ErrorStatus=RCC_Nok;		  break;
		}
	}
	return Local_ErrorStatus;
}

RCC_Error_status_t RCC_APB1ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
		if((Copy_Peri>31)||(Copy_Peri<0)){
			return Local_ErrorStatus=RCC_Nok;
		}
		else{
			switch (Copy_Peri_Mode){
			case ENABLE_PERI: RCC->APB1ENR|=(1<<Copy_Peri);	  break;
			case DISABLE_PERI: RCC->APB1ENR&=~(1<<Copy_Peri); break;
			default : return Local_ErrorStatus=RCC_Nok;		  break;
			}
		}
		return Local_ErrorStatus;
}

RCC_Error_status_t RCC_APB2ENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
		if((Copy_Peri>31)||(Copy_Peri<0)){
			Local_ErrorStatus=RCC_Nok;
		}
		else{
			switch (Copy_Peri_Mode){
			case ENABLE_PERI: RCC->APB2ENR|=(1<<Copy_Peri);	  break;
			case DISABLE_PERI: RCC->APB2ENR&=~(1<<Copy_Peri); break;
			default : Local_ErrorStatus=RCC_Nok;		  break;
			}
		}
		return Local_ErrorStatus;
}

RCC_Error_status_t RCC_AHB1LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
		if((Copy_Peri>31)||(Copy_Peri<0)){
			Local_ErrorStatus=RCC_Nok;
		}
		else{
			switch (Copy_Peri_Mode){
			case ENABLE_PERI: RCC->AHB1LPENR|=(1<<Copy_Peri);	  break;
			case DISABLE_PERI: RCC->AHB1LPENR&=~(1<<Copy_Peri); break;
			default : Local_ErrorStatus=RCC_Nok;		  break;
			}
		}
		return Local_ErrorStatus;
}


RCC_Error_status_t RCC_AHB2LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
		if((Copy_Peri>31)||(Copy_Peri<0)){
			Local_ErrorStatus=RCC_Nok;
		}
		else{
			switch (Copy_Peri_Mode){
			case ENABLE_PERI: RCC->AHB2LPENR|=(1<<Copy_Peri);	  break;
			case DISABLE_PERI: RCC->AHB2LPENR&=~(1<<Copy_Peri); break;
			default : Local_ErrorStatus=RCC_Nok;		  break;
			}
		}
		return Local_ErrorStatus;
}


RCC_Error_status_t RCC_APB1LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
		if((Copy_Peri>31)||(Copy_Peri<0)){
			Local_ErrorStatus=RCC_Nok;
		}
		else{
			switch (Copy_Peri_Mode){
			case ENABLE_PERI: RCC->APB1LPENR|=(1<<Copy_Peri);	  break;
			case DISABLE_PERI: RCC->APB1LPENR&=~(1<<Copy_Peri); break;
			default : Local_ErrorStatus=RCC_Nok;		  break;
			}
		}
		return Local_ErrorStatus;
}

RCC_Error_status_t RCC_APB2LPENR_Enable_Disable_peri(uint8_t Copy_Peri,uint8_t Copy_Peri_Mode){
	RCC_Error_status_t Local_ErrorStatus=RCC_Ok;
		if((Copy_Peri>31)||(Copy_Peri<0)){
			Local_ErrorStatus=RCC_Nok;
		}
		else{
			switch (Copy_Peri_Mode){
			case ENABLE_PERI: RCC->APB2LPENR|=(1<<Copy_Peri);	  break;
			case DISABLE_PERI: RCC->APB2LPENR&=~(1<<Copy_Peri); break;
			default : Local_ErrorStatus=RCC_Nok;		  break;
			}
		}
		return Local_ErrorStatus;
}
