#ifndef SWITCH_H
#define SWITCH_H
typedef unsigned char u8;
typedef unsigned int u32;

typedef enum {
	SWITCH_Nok,
	SWITCH_Ok
}SWITCH_Errors_t;


typedef struct{
		void * port;
		u32 pin;
		u8 mode;
}SWITCH_t;

void SWITCH_Init();

/*
 * @brief
 * if Read=1 this means switch is Pressed  if Read=0 this means switch is Not Pressed
 */

SWITCH_Errors_t SWITCH_Get_Read(u32 Switch_num,u32* Read);
#endif
