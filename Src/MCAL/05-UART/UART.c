#include "RCC_interface.h"
#include "NVIC.h"
#include "STD_types.h"
#include "UART.h"

/* SR pins */
#define 	SR_TXE			7			//transmit data register empty pin
#define 	SR_TC			6			//transmission complete pin
#define 	SR_RXNE			5			//0: Data is not received 1: Received data is ready to be read.
#define 	SR_OR			3			//overrun error flag
#define 	SR_NF			2			//Noise detect flag
#define 	SR_FE			1			//framing error flag
#define 	SR_PE			0			//praity error flag
#define  	SR_RESET		0x00C00000

/*USART_CR1 Pins */
#define 	CR1_OVER8		15 			// 1 represents sampling by 8 and 0 represents sampling by 16
#define 	CR1_UE			13			// 1 to enable the UART 0 to disable prescaler and outputs
#define 	CR1_M			12			// 1 1 Start bit, 9 Data bits, n Stop bit, 0 1 Start bit, 8 Data bits, n Stop bit
#define 	CR1_PCE			10			// 1 to enable parity ,0 to disable
#define 	CR1_PS			9			// 1 for odd parity , 0 for even parity
#define 	CR1_PEIE		8			// 1 An USART interrupt is generated whenever PE=1 ,0 Interrupt is inhibited
#define 	CR1_TXEIE		7			// 1 An USART interrupt is generated whenever TXE=1, 0 Interrupt is inhibited
#define 	CR1_TCIE		6			// 1 An USART interrupt is generated whenever TC=1 ,0 Interrupt is inhibited
#define 	CR1_RXNEIE		5			// 1 An USART interrupt is generated whenever ORE=1 or RXNE=1 ,0  Interrupt is inhibited
#define 	CR1_IDLEIE		4			// 1 An USART interrupt is generated whenever IDLE=1, Interrupt is inhibited
#define 	CR1_TE			3			// 1 Transmitter is enabled , 0 Transmitter is disabled
#define 	CR1_RE			2			// 1 Receiver is enabled and begins searching for a start bit ,0 Receiver is disabled
#define 	CR1_RWU			1			// 1 Receiver in mute mode , 0 Receiver in active mode
#define 	CR1_SBK			0			// 1: Break character will be transmitted ,0: No break character is transmitted
#define  	CR1_RESET		0x00000000

/* USART CR3 pins*/
#define  CR3_DMAT			7
#define  CR3_DMAR			6
/*BRR pins
 * from pin 15 to pin 4 represents mantisaa
 * from pin 3 to pin 0 represents Divison factor
 * */
#define POSITION_OF_MANTISSA	4

#define READY			0
#define BUSY			1

/************* the microcontroler frequency ***********/
#define CLK_FREQ		16000000

/*Register Uart Addresses*/
#define UART1_ADDRESS	0x40011000
#define UART2_ADDRESS	0x40004400
#define UART6_ADDRESS	0x40011400

/****************** for setting the UART Data recived from user to transmit *********/
#define UART1_TX_REQUEST				0
#define UART2_TX_REQUEST				1
#define UART6_TX_REQUEST				2
/****************** for setting the UART Data buffers to recieve data *********/
#define UART1_RX_REQUEST				0
#define UART2_RX_REQUEST				1
#define UART6_RX_REQUEST				2

typedef struct{
	uint32_t SR;
	uint32_t DR;	//Data register
	uint32_t BRR;
	uint32_t CR1;
	uint32_t CR2;
	uint32_t CR3;
	uint32_t GTPR;
}UART_Registers_t;


typedef struct {
	Buffer_t *Tx_Buffer;
	uint32_t state;
	uint32_t pos;
	Tx_cb Cb;
}Tx_Req;

typedef struct {
	Buffer_t *Rx_Buffer;
	uint32_t state;
	uint32_t pos;
	Tx_cb Cb;
}Rx_Req;


/***********
 * initialize a struct to recieve the user requset
 * *************/

Tx_Req Tx_User_Request[3]={0};

Rx_Req Rx_User_Request[3]={0};

Uart_ConfigType * Global_Config_Ptr[3];


void UART_Init(Uart_ConfigType * Uart_Config){
	uint32_t CR1_Helper=0;
	uint32_t Mantissa_Helper=0;
	uint32_t Loc_u32TempValue;
		uint16_t	Loc_u16TempMantissa;
		uint16_t Loc_u16TempFraction;
if(Uart_Config){
if (Uart_Config->Channel==UART1){
	RCC_APB2ENR_Enable_Disable_peri(APB2ENR_USART1EN, ENABLE_PERI);
	NVIC_EnableIRQ(NVIC_USART1_INTERRUPT);
}
else if(Uart_Config->Channel==UART6){
	RCC_APB2ENR_Enable_Disable_peri(APB2ENR_USART6EN, ENABLE_PERI);
	NVIC_EnableIRQ(NVIC_USART6_INTERRUPT);
}
else if(Uart_Config->Channel==UART2){
	RCC_APB1ENR_Enable_Disable_peri(APB1ENR_USART2EN, ENABLE_PERI);
	NVIC_EnableIRQ(NVIC_USART2_INTERRUPT);
}
else{
	// wrong peripheral
}
// to get the mantissa value from the given baud rate

Loc_u32TempValue=((uint64_t)CLK_FREQ * 1000)/(Uart_Config->BaudRate * (8 * (2 - Uart_Config->Oversampling)));
		Loc_u16TempFraction = (Loc_u32TempValue % 1000) * (8 * (2 - Uart_Config->Oversampling));
		Loc_u16TempFraction = Loc_u16TempFraction / 1000;
		if(Loc_u16TempFraction > 0xF)
		{
			Loc_u16TempMantissa += (Loc_u16TempFraction & 0xF0);
		}
		Loc_u16TempMantissa = Loc_u32TempValue / 1000;

/************* configuring the BRR Register***************/
((UART_Registers_t*)(Uart_Config->Channel))->BRR|=(Loc_u16TempMantissa << 4) | (Loc_u16TempFraction & 0x0F);

/************** configuring the CR1 Register **************/
 CR1_Helper=((UART_Registers_t*)(Uart_Config->Channel))->CR1;
CR1_Helper&=CR1_RESET;
// 1 represents sampling by 8 and 0 represents sampling by 16
CR1_Helper|=(Uart_Config->Oversampling<<CR1_OVER8);
// 1 to enable the UART 0 to disable prescaler and outputs
CR1_Helper|=(Uart_Config->UartEnable<<CR1_UE);
// reset the status register
((UART_Registers_t*)(UART1))->SR=0x00000000;
// 1 1 Start bit, 9 Data bits, n Stop bit, 0 1 Start bit, 8 Data bits, n Stop bit
CR1_Helper|=(Uart_Config->WordLength<<CR1_M);
// 1 to enable parity ,0 to disable
CR1_Helper|=(Uart_Config->ParityControl<<CR1_PCE);
// Transmit data register empty interrupt is generated whenever TXE=1
CR1_Helper|=(Uart_Config->TransDateRegisterEmpty_Int<<CR1_TXEIE);
// 1 An USART interrupt is generated whenever TC=1 ,0 Interrupt is inhibited
CR1_Helper|=(Uart_Config->TransComplete_Int<<CR1_TCIE);
CR1_Helper|=(Uart_Config->TransEnable<<CR1_TE);
//Read data register not empty interrupt enable
CR1_Helper|=(Uart_Config->ReceiveDateRegisterEmpty_Int<<CR1_RXNEIE);
CR1_Helper|=(Uart_Config->ReceiveEnable<<CR1_RE);
((UART_Registers_t*)(Uart_Config->Channel))->CR1=CR1_Helper;
}
}



void UART_SendData_AsyncZeroCopy(Buffer_t *Data_Buffer,Tx_cb Call_back){


	/*************** to check if the required peripheral to send data is UART1 *************/
	if(Data_Buffer->Channel==UART1){
	if(Tx_User_Request[0].state==READY){
	//to validate that data not NULL and length not zero of negative
	if((Data_Buffer)&&(Data_Buffer->size>0)){
		Tx_User_Request[0].state=BUSY;
		Tx_User_Request[0].Tx_Buffer=Data_Buffer;
		Tx_User_Request[0].Cb=Call_back;
		Tx_User_Request[0].pos=1; 	//to initialize the position
		// to start sending the first byte
		((UART_Registers_t*)(Tx_User_Request[0].Tx_Buffer->Channel))->CR1|=(1<<CR1_TE);
		((UART_Registers_t*)(Tx_User_Request[0].Tx_Buffer->Channel))->DR=(Tx_User_Request[0].Tx_Buffer->data[0]);
	}
}
	}

	/*************** to check if the required peripheral to send data is UART2 *************/
	else if(Data_Buffer->Channel==UART2){
		if(Tx_User_Request[1].state==READY){
			//to validate that data not NULL and length not zero of negative

			if((Data_Buffer)&&(Data_Buffer->size>0)){
		Tx_User_Request[1].state=BUSY;
		Tx_User_Request[1].Tx_Buffer=Data_Buffer;
		Tx_User_Request[1].Cb=Call_back;
		Tx_User_Request[1].pos=1; 	//to initialize the position
		// to start sending the first byte and enable the transmitting
		((UART_Registers_t*)(Tx_User_Request[1].Tx_Buffer->Channel))->CR1|=(1<<CR1_TE);
		((UART_Registers_t*)(Tx_User_Request[1].Tx_Buffer->Channel))->DR=(Tx_User_Request[1].Tx_Buffer->data[0]);
			}

	}
	}

	/*************** to check if the required peripheral to send data is UART6 *************/
	else if(Data_Buffer->Channel==UART6){
			if(Tx_User_Request[2].state==READY){
				//to validate that data not NULL and length not zero of negative
		if((Data_Buffer)&&(Data_Buffer->size>0)){
			Tx_User_Request[2].state=BUSY;
			Tx_User_Request[2].Tx_Buffer=Data_Buffer;
			Tx_User_Request[2].Cb=Call_back;
			Tx_User_Request[2].pos=1; 	//to initialize the position
			// to start sending the first byte and enable the transmitting
			((UART_Registers_t*)(Tx_User_Request[2].Tx_Buffer->Channel))->CR1|=(1<<CR1_TE);
			((UART_Registers_t*)(Tx_User_Request[2].Tx_Buffer->Channel))->DR=(Tx_User_Request[2].Tx_Buffer->data[0]);
				}

		}
		}
}

void UART_SendByte(Buffer_t *Data_Buffer){
	if((Data_Buffer)&&(Data_Buffer->size==1)){
		if(((UART_Registers_t*)(Data_Buffer->Channel))->SR&(1<<SR_TXE)){
			Tx_User_Request[0].Tx_Buffer=Data_Buffer;
			Tx_User_Request[0].pos=1;
			((UART_Registers_t*)(Data_Buffer->Channel))->CR1|=(1<<CR1_TE);
			((UART_Registers_t*)(Data_Buffer->Channel))->DR=Data_Buffer->data[0];
		}
		else{
			// do nothing
		}
	}
	else{
		//do nothing
	}

}
void UART_RecieveData_Async(Buffer_t *Data_Buffer,Tx_cb Call_back){
	/*************** to check if the required peripheral to receive data is UART1 *************/
		if(Data_Buffer->Channel==UART1){
		if(Tx_User_Request[UART1_RX_REQUEST].state==READY){
		//to validate that data not NULL and length not zero of negative
		if((Data_Buffer)&&(Data_Buffer->size>0)){
			// to reset the SR register
			((UART_Registers_t*)(UART1))->SR=0x00000000;
			Rx_User_Request[UART1_RX_REQUEST].state=BUSY;
			Rx_User_Request[UART1_RX_REQUEST].Rx_Buffer=Data_Buffer;
			Rx_User_Request[UART1_RX_REQUEST].Cb=Call_back;
			Rx_User_Request[UART1_RX_REQUEST].pos=0; 	//to initialize the position
			// to enable the receive in Uart Peripheral
		((UART_Registers_t*)(Rx_User_Request[UART1_RX_REQUEST].Rx_Buffer->Channel))->CR1|=(1<<CR1_RE);
	}
		}
		}

		/*************** to check if the required peripheral to receive data is UART2 *************/
		else if(Data_Buffer->Channel==UART2){
			if(Rx_User_Request[UART2_RX_REQUEST].state==READY){
				//to validate that data not NULL and length not zero of negative

				if((Data_Buffer)&&(Data_Buffer->size>0)){
			// to reset the SR register
			((UART_Registers_t*)(UART1))->SR=0x00000000;
			Rx_User_Request[UART2_RX_REQUEST].state=BUSY;
			Rx_User_Request[UART2_RX_REQUEST].Rx_Buffer=Data_Buffer;
			Rx_User_Request[UART2_RX_REQUEST].Cb=Call_back;
			Rx_User_Request[UART2_RX_REQUEST].pos=0; 	//to initialize the position
			// to enable the receive in Uart Peripheral
			((UART_Registers_t*)(Rx_User_Request[UART2_RX_REQUEST].Rx_Buffer->Channel))->CR1|=(1<<CR1_RE);
				}

		}
		}

		/*************** to check if the required preipheral to receive data is UART6 *************/
		else if(Data_Buffer->Channel==UART6){
				if(Rx_User_Request[UART6_RX_REQUEST].state==READY){
					//to validate that data not NULL and length not zero of negative
			if((Data_Buffer)&&(Data_Buffer->size>0)){
				// to reset the SR register
				((UART_Registers_t*)(UART1))->SR=0x00C00000;
				Rx_User_Request[UART6_RX_REQUEST].state=BUSY;
				Rx_User_Request[UART6_RX_REQUEST].Rx_Buffer=Data_Buffer;
				Rx_User_Request[UART6_RX_REQUEST].Cb=Call_back;
				Rx_User_Request[UART6_RX_REQUEST].pos=0; 	//to initialize the position
				// to enable the receive in Uart Peripheral
				((UART_Registers_t*)(Rx_User_Request[UART6_RX_REQUEST].Rx_Buffer->Channel))->CR1|=(1<<CR1_RE);
					}

			}
			}
}

void UART_SendBufferDma(void * Channel){
	((UART_Registers_t*)(Channel))->CR3|=(1<<CR3_DMAT);
}

void UART_RecieveBufferDma(void * Channel){
	((UART_Registers_t*)(Channel))->CR3|=(1<<CR3_DMAR);
}

void USART1_IRQHandler(void){
/****** to check out if the tranmission register is empty pin that caused the interrupt **************/
if(((UART_Registers_t*)(UART1))->SR&1<<SR_TC){
	// to clear the transmission complete pin
		((UART_Registers_t*)(UART1))->SR&=~(1<<SR_TC);
	/**************** to check that the current position of data is less than the buffer size*****/
	if(Tx_User_Request[UART1_TX_REQUEST].pos<Tx_User_Request[UART1_TX_REQUEST].Tx_Buffer->size){
	((UART_Registers_t*)(UART1))->DR=(Tx_User_Request[UART1_TX_REQUEST].Tx_Buffer->data[Tx_User_Request[UART1_TX_REQUEST].pos]);
	Tx_User_Request[UART1_TX_REQUEST].pos++;
	// to clear the transmission complete pin
	}
	else{
		Tx_User_Request[UART1_TX_REQUEST].Cb();
		Tx_User_Request[UART1_TX_REQUEST].state=READY;
		((UART_Registers_t*)(UART1))->SR&=~(1<<SR_TC);
	}

	if(((UART_Registers_t*)(UART1))->SR&1<<SR_RXNE){
		if(Rx_User_Request[UART1_RX_REQUEST].pos<Rx_User_Request[UART1_RX_REQUEST].Rx_Buffer->size){
		(Rx_User_Request[UART1_TX_REQUEST].Rx_Buffer->data[Rx_User_Request[UART1_RX_REQUEST].pos])=((UART_Registers_t*)(UART1))->DR;
			Rx_User_Request[UART1_TX_REQUEST].pos++;
			// to clear the recieve data pin
			((UART_Registers_t*)(UART1))->SR&=~(1<<SR_RXNE);
			}
			else{
				Rx_User_Request[UART1_RX_REQUEST].Cb();
				Rx_User_Request[UART1_RX_REQUEST].state=READY;
				// to clear the recieve data pin
				((UART_Registers_t*)(UART1))->SR&=~(1<<SR_RXNE);
			}
	}
}
}

void USART2_IRQHandler(void){
	/****** to check out if the tranmission register is empty pin that caused the interrupt **************/
	if(((UART_Registers_t*)(UART2))->SR&1<<SR_TC){
		// to clear the transmission complete pin
			((UART_Registers_t*)(UART2))->SR&=~(1<<SR_TC);
		/**************** to check that the current position of data is less than the buffer size*****/
		if(Tx_User_Request[UART2_TX_REQUEST].pos<Tx_User_Request[UART2_TX_REQUEST].Tx_Buffer->size){
		((UART_Registers_t*)(UART1))->DR=(Tx_User_Request[UART2_TX_REQUEST].Tx_Buffer->data[Tx_User_Request[UART2_TX_REQUEST].pos]);
		Tx_User_Request[UART2_TX_REQUEST].pos++;
		// to clear the transmission complete pin
		((UART_Registers_t*)(UART2))->SR&=~(1<<SR_TC);
		}
		else{
			Tx_User_Request[UART2_TX_REQUEST].Cb();
			Tx_User_Request[UART2_TX_REQUEST].state=READY;
			((UART_Registers_t*)(UART2))->SR&=~(1<<SR_TC);
		}

		if(((UART_Registers_t*)(UART2))->SR&1<<SR_RXNE){
			if(Rx_User_Request[UART2_RX_REQUEST].pos<Rx_User_Request[UART2_RX_REQUEST].Rx_Buffer->size){
			(Rx_User_Request[UART2_TX_REQUEST].Rx_Buffer->data[Rx_User_Request[UART2_RX_REQUEST].pos])=((UART_Registers_t*)(UART2))->DR;
				Rx_User_Request[UART2_TX_REQUEST].pos++;
				// to clear the recieve data pin
				((UART_Registers_t*)(UART2))->SR&=~(1<<SR_RXNE);
				}
				else{
					Rx_User_Request[UART2_RX_REQUEST].Cb();
					Rx_User_Request[UART2_RX_REQUEST].state=READY;
					// to clear the recieve data pin
					((UART_Registers_t*)(UART2))->SR&=~(1<<SR_RXNE);
				}
		}
	}
}
