#ifndef UART_H_
#define UART_H_

#include "STD_types.h"

#define UART1		(void*)(0x40011000)
#define UART2		(void*)(0x40004400)
#define UART6		(void*)(0x40011400)

typedef void (*Tx_cb)(void);

typedef struct {
	uint8_t * data;
	uint32_t size;
	void* Channel;
}Buffer_t;

/***********************************************
 * // 1 represents sampling by 8 and 0 represents sampling by 16
// 1 to enable the UART 0 to disable prescaler and outputs
// 1, 1 Start bit, 9 Data bits, n Stop bit, 0 1 Start bit, 8 Data bits, n Stop bit
// 1 to enable parity ,0 to disable
// 1 for odd parity , 0 for even parity
// 1 An USART interrupt is generated whenever PE=1 ,0 Interrupt is inhibited
// 1 An USART interrupt is generated whenever TXE=1, 0 Interrupt is inhibited
// 1 An USART interrupt is generated whenever TC=1 ,0 Interrupt is inhibited
// 1 An USART interrupt is generated whenever ORE=1 or RXNE=1 ,0  Interrupt is inhibited
// 1 An USART interrupt is generated whenever IDLE=1, Interrupt is inhibited
// 1 Transmitter is enabled , 0 Transmitter is disabled
// 1 Receiver is enabled and begins searching for a start bit ,0 Receiver is disabled
 */


/*Uart Configuration*/
typedef struct
{
	void * Channel;
	uint32_t Oversampling;
	uint32_t UartEnable;
	uint32_t WordLength;
	uint32_t ParityControl;
	uint32_t TransDateRegisterEmpty_Int;
	uint32_t TransComplete_Int;
	uint32_t ReceiveDateRegisterEmpty_Int;
	uint32_t TransEnable;
	uint32_t ReceiveEnable;
	uint32_t BaudRate;

}Uart_ConfigType;


void UART_Init(Uart_ConfigType * Uart_Config);


void UART_SendData_AsyncZeroCopy(Buffer_t *Data_Buffer,Tx_cb Call_back);

void UART_RecieveData_Async(Buffer_t *Data_Buffer,Tx_cb Call_back);


void UART_SendByte(Buffer_t *Data_Buffer);

void UART_SendBufferDma(void * Channel);

void UART_RecieveBufferDma(void * Channel);

uint8_t UART_GetTx_State(void);

uint8_t UART_GetRx_State(void);

#endif
