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
volatile u32 CR;			//to disable and enable clocks
volatile u32 PLLCFGR;
volatile u32 CFGR;
volatile u32 CIR;
volatile u32 AHB1RSTR;
volatile u32 AHB2RSTR;
volatile u32 reserved1[2];
volatile u32 APB1RSTR;
volatile u32 APB2RSTR;
volatile u32 reserved2[2];
volatile u32 AHB1ENR;
volatile u32 AHB2ENR;
volatile u32 reserved3[2];
volatile u32 APB1ENR;
volatile u32 APB2ENR;
volatile u32 reserved4[2];
volatile u32 AHB1LPENR;
volatile u32 AHB2LPENR;
volatile u32 reserved5[2];
volatile u32 APB1LPENR;
volatile u32 APB2LPENR;
volatile u32 reserved6[2];
volatile u32 BDCR;
volatile u32 CSR;
volatile u32 reserved7[2];
volatile u32 SSCGR;
volatile u32 RCC_PLLI2SCFGR;
volatile u32 reserved8;
volatile u32 DCKCFGR;
}RCC_Peri_t;

 RCC_Peri_t * const RCC=((RCC_Peri_t *)(RCC_BASE_ADDRESS));

RCC_Error_status_t RCC_EnableClock(u32 Copy_Clk){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
switch (Copy_Clk){
case HSI: RCC->CR|=HSI; break;
case HSE: RCC->CR|=HSE; break;
case PLL: RCC->CR|=PLL; break;
default : return Loc_ErrorStatus=RCC_Nok;
}
return Loc_ErrorStatus;
}

RCC_Error_status_t RCC_DisableClock(u32 Copy_Clk){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
switch (Copy_Clk){
case HSI: RCC->CR&=~HSI; break;
case HSE: RCC->CR&=~HSE; break;
case PLL: RCC->CR&=~PLL; break;
default : return Loc_ErrorStatus=RCC_Nok;
}
return Loc_ErrorStatus;
}

u32 RCC_Get_Ready_Status(u32 Get_read ){
	u32 ready_status;
switch(Get_read){
case HSI: ready_status=((RCC->CR>>HSI_RDY)&1); break;
case HSE: ready_status=((RCC->CR>>HSE_RDY)&1); break;
case PLL: ready_status=((RCC->CR>>PLL_RDY)&1); break;
default:   ready_status=2; 		break;
}
return ready_status;
}


RCC_Error_status_t RCC_Set_PLLSrc(u32 Copy_PllSrc){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
	switch (Copy_PllSrc){
	case HSI : RCC->PLLCFGR&=~(1<<PLL_SELECT_SRC_BIT); break;
	case HSE : RCC->PLLCFGR&=(1<<PLL_SELECT_SRC_BIT); break;
	default  : Loc_ErrorStatus=RCC_Nok;
	}
	return Loc_ErrorStatus;
}


RCC_Error_status_t RCC_PllConfig(u32 Copy_N_factor,u32 Copy_M_factor,u32 Copy_P_factor ){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
	u32 Local_Helper=0,P_Factor_Value;
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


RCC_Error_status_t RCC_Set_SysClk(u32 Copy_ClkSrc){
	RCC_Error_status_t Loc_ErrorStatus=RCC_Ok;
	u32 Loc_SysClkHelper=RCC->CFGR;
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

void RCC_Get_SysClk(u32* Copy_ClkSrc){
	*Copy_ClkSrc	=((RCC->CFGR>>2)&GET_SYS_CLK);
	return ((RCC->CFGR>>2)&GET_SYS_CLK); // to get the two digits which repersents the current system clock
}


RCC_Error_status_t RCC_AHB1ENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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


RCC_Error_status_t RCC_AHB2ENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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

RCC_Error_status_t RCC_APB1ENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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

RCC_Error_status_t RCC_APB2ENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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

RCC_Error_status_t RCC_AHB1LPENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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


RCC_Error_status_t RCC_AHB2LPENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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


RCC_Error_status_t RCC_APB1LPENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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

RCC_Error_status_t RCC_APB2LPENR_Enable_Disable_peri(u8 Copy_Peri,u8 Copy_Peri_Mode){
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
