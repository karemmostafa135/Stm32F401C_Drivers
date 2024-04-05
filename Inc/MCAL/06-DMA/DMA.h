#ifndef DMA_H
#define DMA_H

typedef enum
{
	Dma_Stream_En = 0,
	Dma_Wrong_Stream,
	Dma_Wrong_Channel,
	Dma_NullPtr,
	Dma_Ok
}Dma_ErrorStatus_t;


typedef enum
{
	DMA1 = 0,
	DMA2

}Dma_Id_t;

typedef enum
{
	Dma_Stream0 = 0,
	Dma_Stream1 = 1,
	Dma_Stream2 = 2,
	Dma_Stream3 = 3,
	Dma_Stream4 = 4,
	Dma_Stream5 = 5,
	Dma_Stream6 = 6,
	Dma_Stream7 = 7

}Stream_Id_t;

typedef enum
{
	Dma_Channel0 = 0,
	Dma_Channel1 = 1,
	Dma_Channel2 = 2,
	Dma_Channel3 = 3,
	Dma_Channel4 = 4,
	Dma_Channel5 = 5,
	Dma_Channel6 = 6,
	Dma_Channel7 = 7

}Channel_Id_t;

typedef enum
{
	Dma_Low = 0,
	Dma_Medium,
	Dma_High,
	Dma_VeryHigh

}Priority_t;

typedef enum
{
	Byte = 0,
	Half_Word,
	Word,

}Size_t;

typedef enum
{
	Per_to_Mem = 0,
	Mem_to_Per,
	Mem_to_Mem

}Direction_t;

typedef enum{

	Dma_Disable = 0,
	Dma_Enable

}State_t;

typedef enum
{
	Single = 0,
	Inc_4_Beats,
	Inc_8_Beats,
	Inc_16_Beats

}Burst_t;

/*
 * The struct is made to configure the DMA Channel
 *
 * !! Check all user define types (enum) above for each Mode as use from it
 *
 * !! TO USE IN FUCNTION :: Dma_enuConfiguration
 *
 * */
typedef struct
{
	Dma_Id_t   Dma;
	Stream_Id_t Stream;
	Channel_Id_t Channel;
	Burst_t Memory_Burst;
	Burst_t Peripheral_Burst;
	State_t Double_Buffer;
	Priority_t Priority_Cfg;
	Size_t	Memory_Size;
	Size_t	Peripheral_Size;
	State_t Mem_Auto_Increment;
	State_t Per_Auto_Increment;
	State_t Circular_Mode;
	Direction_t Data_Direction;

	struct
	{
		State_t Direct_Mode;
		State_t Transfer_Err;
		State_t HalfTransfer;
		State_t Transfer_Complete;

	}Interrupt;

}Dma_config_t;


typedef struct
{
	Stream_Id_t Stream;
	uint32_t Peripheral_Address;
	uint32_t Memory_0_Address;
	uint32_t Memory_1_Address;
	uint16_t Data_Length;

}Criteria_t;

Dma_ErrorStatus_t DMA_Config(Dma_config_t * Dma_Config);
/*
 * to set the Addresses of Source and Destination and the data lentgh to be trasnfered
 */
Dma_ErrorStatus_t DMA_Set_Criteria(Criteria_t * Critera,Dma_Id_t  Selected_Dma);


Dma_ErrorStatus_t DMA_Stream_Set_State(Dma_Id_t  Selected_Dma,Stream_Id_t Stream,State_t State);
#endif
