#include "DMA.h"

/********************* macros to be used during configuration**************/
#define LIFCR_RESET_MASK			0x0F7D0F7D
#define HIFCR_RESET_MASK			0x0F7D0F7D
#define SXCR_RESET_MASK				0xF0000000
/*********************** for register DMA_SxCR******************/
#define CHANNEL_SELECTION_POS		0x00000019
#define MBURST_POS					0x00000017
#define PBURST_POS					0x00000015
#define CT_POS						0x00000013
#define DBM_POS						0x00000012
#define PL_POS						0x00000010
#define PINCOS_POS					0x0000000F
#define MSIZE_POS					0x0000000D
#define PSIZE_POS					0x0000000B
#define MINC_POS					0x0000000A
#define PINC_POS					0x00000009
#define CIRC_POS					0x00000008
#define DIR_POS						0x00000006
#define PFCTRL_POS					0x00000005
#define TCIE_POS					0x00000004
#define HTIE_POS					0x00000003
#define TEIE_POS					0x00000002
#define DMEIE_POS					0x00000001
#define EN_POS						0x00000000

#define DMA1_ADDRESS	((DMA_Registers_t*)0x40026000)

#define DMA2_ADDRESS	((DMA_Registers_t*)0x40026400)

/*Register Address*/
typedef struct
{
	uint32_t LISR;
	uint32_t HISR;
	uint32_t LIFCR;
	uint32_t HIFCR;

	struct
	{
		uint32_t SxCR;
		uint32_t SxNDTR;
		uint32_t SxPAR;
		uint32_t SxM0AR;
		uint32_t SxM1AR;
		uint32_t SxFCR;
	}Cfg_Reg[8];

}DMA_Registers_t;





Dma_ErrorStatus_t DMA_Config(Dma_config_t * Dma_Config){
	Dma_ErrorStatus_t Local_Error_Status=Dma_Ok;
	uint32_t Local_Helper=0;
/**************** to validate the Null ptr ****************/

if(Dma_Config){
/*************** to validate the DMA It self**************/
	if(Dma_Config->Dma==DMA1){
		/************ to validate that the stream number is valid***********/
		if(Dma_Config->Stream>Dma_Stream7||Dma_Config->Stream<Dma_Stream0){
			Local_Error_Status=Dma_Wrong_Stream;
		}
		else{
			/*************** to make sure that the enable pin is equal to zero **********/
			if((DMA1_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR>>EN_POS)&1){
				/************* setting the enable bit to zero ***************/
				DMA1_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR&=~(1<<EN_POS);
				while(DMA1_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR>>EN_POS){
					/******** loop until the enable pin is zero *********/
				}
			}
			/*************************** setting all the configurations ***************/

			Local_Helper=DMA1_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR;
			/****************** to reset the previous configurations if any *******/
			Local_Helper&=SXCR_RESET_MASK;
			/******************* to set tha channel **************/
			Local_Helper|=(Dma_Config->Channel<<CHANNEL_SELECTION_POS);
			/**************** to set the MBURST************/
			Local_Helper|=(Dma_Config->Memory_Burst<<MBURST_POS);
			/**************** to set the PBURST************/
			Local_Helper|=(Dma_Config->Peripheral_Burst<<PBURST_POS);
			/*************** to set the double buffer mode config *******/
			Local_Helper|=(Dma_Config->Double_Buffer<<DBM_POS);
			/*************** to set the circular buffer mode config *******/
			Local_Helper|=(Dma_Config->Circular_Mode<<CIRC_POS);
			/************* to set the Priority level config ***********/
			Local_Helper|=(Dma_Config->Priority_Cfg<<PL_POS);
			/************* to set the memory auto increment config config ***********/
			Local_Helper|=(Dma_Config->Mem_Auto_Increment<<MINC_POS);
			/************* to set the peripheral auto increment config config ***********/
			Local_Helper|=(Dma_Config->Per_Auto_Increment<<PINC_POS);
			/************* to set the peripheral size **************/
			Local_Helper|=(Dma_Config->Peripheral_Size<<PSIZE_POS);
			/************* to set the peripheral size **************/
			Local_Helper|=(Dma_Config->Memory_Size<<MSIZE_POS);
			/************* to set the data Direction **********/
			Local_Helper|=(Dma_Config->Data_Direction<<DIR_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.Transfer_Complete<<TCIE_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.HalfTransfer<<HTIE_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.Transfer_Err<<TEIE_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.Direct_Mode<<DMEIE_POS);
			/****************** to set the configurations on the real register ***********/
			DMA1_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR=Local_Helper;
			}
		}
	else if (Dma_Config->Dma==DMA2){
		/************ to validate that the stream number is valid***********/
				if(Dma_Config->Stream>Dma_Stream7||Dma_Config->Stream<Dma_Stream0){
					Local_Error_Status=Dma_Wrong_Stream;
				}
				else{
					/*************** to make sure that the enable pin is equal to zero **********/
					if((DMA2_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR>>EN_POS)&1){
					/************* setting the enable bit to zero ***************/
						DMA2_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR&=~(1<<EN_POS);
						while(DMA2_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR>>EN_POS){
							/******** loop until the enable pin is zero *********/
						}
					}
					/*************************** setting all the configurations ***************/

			Local_Helper=DMA2_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR;
			/****************** to reset the previous configurations if any *******/
			Local_Helper&=SXCR_RESET_MASK;
			/******************* to set tha channel **************/
			Local_Helper|=(Dma_Config->Channel<<CHANNEL_SELECTION_POS);
			/**************** to set the MBURST************/
			Local_Helper|=(Dma_Config->Memory_Burst<<MBURST_POS);
			/**************** to set the PBURST************/
			Local_Helper|=(Dma_Config->Peripheral_Burst<<PBURST_POS);
			/*************** to set the double buffer mode config *******/
			Local_Helper|=(Dma_Config->Double_Buffer<<DBM_POS);
			/*************** to set the circular buffer mode config *******/
			Local_Helper|=(Dma_Config->Circular_Mode<<CIRC_POS);
			/************* to set the Priority level config ***********/
			Local_Helper|=(Dma_Config->Priority_Cfg<<PL_POS);
			/************* to set the memory auto increment config config ***********/
			Local_Helper|=(Dma_Config->Mem_Auto_Increment<<MINC_POS);
			/************* to set the peripheral auto increment config config ***********/
			Local_Helper|=(Dma_Config->Per_Auto_Increment<<PINC_POS);
			/************* to set the peripheral size **************/
			Local_Helper|=(Dma_Config->Peripheral_Size<<PSIZE_POS);
			/************* to set the peripheral size **************/
			Local_Helper|=(Dma_Config->Memory_Size<<MSIZE_POS);
			/************* to set the data Direction **********/
			Local_Helper|=(Dma_Config->Data_Direction<<DIR_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.Transfer_Complete<<TCIE_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.HalfTransfer<<HTIE_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.Transfer_Err<<TEIE_POS);
			/************* to set the interrupt of TCIE **********/
			Local_Helper|=(Dma_Config->Interrupt.Direct_Mode<<DMEIE_POS);
			/****************** to set the configurations on the real register ***********/
			DMA2_ADDRESS->Cfg_Reg[Dma_Config->Stream].SxCR=Local_Helper;
					}
	}
	else{
		Local_Error_Status=Dma_Wrong_Channel;
	}
}
else
{
	Local_Error_Status=Dma_NullPtr;
}
return Local_Error_Status;
}

Dma_ErrorStatus_t DMA_Set_Criteria(Criteria_t * Critera,Dma_Id_t  Selected_Dma){
	Dma_ErrorStatus_t Local_Error_Status=Dma_Ok;
	if(Critera->Stream>Dma_Stream7||Critera->Stream<Dma_Stream0){
		Local_Error_Status=Dma_Wrong_Stream;
	}
	else{
		switch(Selected_Dma){
		case DMA1:
			DMA1_ADDRESS->Cfg_Reg[Critera->Stream].SxM0AR=Critera->Memory_0_Address;
			DMA1_ADDRESS->Cfg_Reg[Critera->Stream].SxM1AR=Critera->Memory_1_Address;
			DMA1_ADDRESS->Cfg_Reg[Critera->Stream].SxPAR=Critera->Peripheral_Address;
			DMA1_ADDRESS->Cfg_Reg[Critera->Stream].SxNDTR=Critera->Data_Length;
		break;
		case DMA2:
			DMA2_ADDRESS->Cfg_Reg[Critera->Stream].SxM0AR=Critera->Memory_0_Address;
			DMA2_ADDRESS->Cfg_Reg[Critera->Stream].SxM1AR=Critera->Memory_1_Address;
			DMA2_ADDRESS->Cfg_Reg[Critera->Stream].SxPAR=Critera->Peripheral_Address;
			DMA2_ADDRESS->Cfg_Reg[Critera->Stream].SxNDTR=Critera->Data_Length;
		break;
		default:

			Local_Error_Status=Dma_Wrong_Channel;
		break;
		}

	}
	return Local_Error_Status;
}

Dma_ErrorStatus_t DMA_Stream_Set_State(Dma_Id_t  Selected_Dma,Stream_Id_t Stream,State_t State){
	Dma_ErrorStatus_t Local_Error_Status=Dma_Ok;
	uint32_t Stream_Helper=0;
	if(Selected_Dma==DMA1){
		if(Stream>Dma_Stream7||Stream<Dma_Stream0){
			Local_Error_Status=Dma_Wrong_Stream;
		}
		else{
			Stream_Helper=DMA1_ADDRESS->Cfg_Reg[Stream].SxCR;
			/************** to reset the enable value ************/
			Stream_Helper&=~(1<<EN_POS);
			Stream_Helper|=(State<<EN_POS);
			DMA1_ADDRESS->Cfg_Reg[Stream].SxCR=Stream_Helper;
		}
	}
	else if(Selected_Dma==DMA2){
		if(Stream>Dma_Stream7||Stream<Dma_Stream0){
					Local_Error_Status=Dma_Wrong_Stream;
				}
				else{
			Stream_Helper=DMA2_ADDRESS->Cfg_Reg[Stream].SxCR;
			/************** to reset the enable value ************/
			Stream_Helper&=~(1<<EN_POS);
			Stream_Helper|=(State<<EN_POS);
			DMA2_ADDRESS->Cfg_Reg[Stream].SxCR=Stream_Helper;
				}

	}
	return Local_Error_Status;
}

void DMA1_Stream0_IRQHandler(void){

}


void DMA2_Stream0_IRQHandler(void){

}
