#include "GPIO_interface.h"
#include "LCD.h"

/****************** macros for lcd commands ************/
#define LCD_FUNCTION_SET_MASK		0x38
#define LCD_DISPLAY_ON_OFF_MASK		0x0C
#define LCD_DISPLAY_CLEAR_MASK		0x01
#define LCD_ENTRY_MODE_MASK			0x06
#define LCD_SHIFT_LEFT_MASK			0x18
#define LCD_SHIFT_RIGHT_MASK		0x1C
#define LCD_SET_DDRAM_ADRESS		128

/***************** macros for goto_XY function to set the location to write   ***************/
#define FIRST_LINE			0
#define SECOND_LINE			1


void Init_Sm();

void LCD_Write_Command(uint8_t Command);

void LCD_Write_Data(uint8_t Data);

void LCD_Write_String_Helper(void);

void LCD_enuGotoDDRAM_XY(uint8_t Copy_u8Row, uint8_t Copy_u8Column);

extern LCD_pin_Cfg LCD_Pins_Config[NUMBER_OF_LCD_PINS];


/*************** enable pin states **************/
#define ENABLE_PIN_LOW				0
#define ENABLE_PIN_HIGH				1


#define READY		1

#define BUSY		0


typedef struct{
	const uint8_t * s;
	uint8_t length;
	uint8_t state;
	uint8_t Type;
}User_Req;

typedef struct {
	uint8_t X_pos;
	uint8_t Y_pos;
}LCD_XY_Values;

/*********** for the lcd state options **************/
typedef enum{
	Init,
	Operation,
	Off
}LCD_State;

/***************** request types enum *********************/

typedef enum{
	None,
	Write_Data,
	Write_command,

}LCD_Requests_options;

/******************* for the init function to initilize the lcd **************/
typedef enum {
	Power_On,
	Function_Set,
	Display_On_Off,
	Display_Clear,
	End
}Init_Modes;

/************************ global variables *********************/

LCD_State Lcd_State=Off;

Init_Modes Mode_Of_Init=Power_On;

uint8_t Enable_Pin_State=ENABLE_PIN_LOW;

LCD_XY_Values Cordinates;


static User_Req Req
={
		.s=NULL,
		.length=0,
		.state=READY,
		.Type=None
};



void LCD_Init_Asynch(){
	 Lcd_State=Init;
}


/******************* LCD Runnable *****************/
void LCD_Task(){
	if(Lcd_State==Init){
		Init_Sm();
	}
	else if (Lcd_State==Operation){
		switch(Req.Type){
		case Write_Data:
			//LCD_enuGotoDDRAM_XY(Cordinates.X_pos,Cordinates.Y_pos);
			LCD_Write_String_Helper();
		break;
		}

	}

}


/********************* to get the string from the user **************/
void LCD_Write_String_NoCopy(uint8_t * str,uint32_t length,uint8_t Copy_u8Row, uint8_t Copy_u8Column){
	if(str){
if(Req.state==READY){
	Req.state=BUSY;
	Req.s=str;
	Req.length=length;
	Req.Type=Write_Data;
	Cordinates.X_pos=Copy_u8Row;		// to set the x-axis value
	Cordinates.Y_pos=Copy_u8Column;		// to set the y-axis value
	//iterator=0; // to initialize the iterator for Lcd_Write_String_Helper function
}
	}
}


/********************* helper function to write the string on lcd *******/
void LCD_Write_String_Helper(void){
	static uint8_t iterator=0;
/******************** set the needed position **************/


	if(Req.s[iterator]!="\0"){
			LCD_Write_Data(Req.s[iterator]);
			if(Enable_Pin_State==ENABLE_PIN_LOW){
			iterator++;
			}

	}
	// when writing the string finish
	else{
		Req.Type=None;
		Req.state=READY;
	}
}


void LCD_enuGotoDDRAM_XY(uint8_t Copy_u8Row, uint8_t Copy_u8Column){
	uint8_t Loc_u8Location =0;
	if(Copy_u8Row==FIRST_LINE){
		Loc_u8Location=Copy_u8Column;
	}
	else if (Copy_u8Row==SECOND_LINE){
		Loc_u8Location=Copy_u8Column+0x40 ;
	}
	else{
	}
	/********** SET_DDRAM_Adress = 64 as we need to but Data Pin 7 equal 1 ***********/
	LCD_Write_Command(LCD_SET_DDRAM_ADRESS+Loc_u8Location);
}

/******************** function to init the lcd ************/
void Init_Sm(){

Pin_Config_t pin;
uint8_t idx=0;
uint8_t Enable_Pin_State=ENABLE_PIN_LOW;

switch (Mode_Of_Init){
case Power_On:
/***** to initialize the lcd pins ************/
	pin.Mode=GPIO_MODE_OUTPUT_PP;
	pin.Speed=GPIO_SPEED_HIGH;
	for(idx=0;idx<NUMBER_OF_LCD_PINS;idx++){
		pin.Pin_num=LCD_Pins_Config[idx].pin;
		pin.Port_num=LCD_Pins_Config[idx].port;
		GPIO_PinConfig(&pin);
	}
	Mode_Of_Init=Function_Set;
	break;
case Function_Set:
	LCD_Write_Command(LCD_FUNCTION_SET_MASK);
		if(Enable_Pin_State==ENABLE_PIN_LOW){
		Mode_Of_Init=Display_On_Off;
		}
	break;
case Display_On_Off:
	LCD_Write_Command(LCD_DISPLAY_ON_OFF_MASK);
	if(Enable_Pin_State==ENABLE_PIN_LOW){
		Mode_Of_Init=Display_On_Off;
	}
	break;
case Display_Clear:
	LCD_Write_Command(LCD_DISPLAY_CLEAR_MASK);
	if(Enable_Pin_State==ENABLE_PIN_LOW){
		Mode_Of_Init=End;
	}
	break;
case End:
	Lcd_State=Operation;
	break;

}
}


/******************* to send a one byte command to Lcd ***********/
void LCD_Write_Command(uint8_t Command){


if(Enable_Pin_State==ENABLE_PIN_LOW){


/******* to set the RW and RS pins to LOw ************/
GPIO_Set_Pin_Value(LCD_Pins_Config[RW_PIN].port, LCD_Pins_Config[RW_PIN].pin, PIN_LOW);
GPIO_Set_Pin_Value(LCD_Pins_Config[RS_PIN].port, LCD_Pins_Config[RS_PIN].pin, PIN_LOW);
/************* copying the command bits to the physical bits ************/
GPIO_Set_Pin_Value(LCD_Pins_Config[D0_PIN].port, LCD_Pins_Config[D0_PIN].pin,(Command&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D1_PIN].port, LCD_Pins_Config[D1_PIN].pin,(Command&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D2_PIN].port, LCD_Pins_Config[D2_PIN].pin,(Command&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D3_PIN].port, LCD_Pins_Config[D3_PIN].pin,(Command&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D4_PIN].port, LCD_Pins_Config[D4_PIN].pin,(Command&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D5_PIN].port, LCD_Pins_Config[D5_PIN].pin,(Command&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D6_PIN].port, LCD_Pins_Config[D6_PIN].pin,(Command&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D7_PIN].port, LCD_Pins_Config[D7_PIN].pin,(Command&(1<<D0_PIN)));

/****************** set the enable pin to high ****************/
GPIO_Set_Pin_Value(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, PIN_HIGH);
Enable_Pin_State=ENABLE_PIN_HIGH;
}
else if(Enable_Pin_State==ENABLE_PIN_HIGH){
		GPIO_Set_Pin_Value(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, PIN_LOW);
		Enable_Pin_State=ENABLE_PIN_LOW;
}

}


/***************** to write a byte of data on lcd *************/

void LCD_Write_Data(uint8_t Data){

	if(Enable_Pin_State==ENABLE_PIN_LOW){
/******* to set the RW and RS pins to LOw ************/
GPIO_Set_Pin_Value(LCD_Pins_Config[RW_PIN].port, LCD_Pins_Config[RW_PIN].pin, PIN_LOW);
GPIO_Set_Pin_Value(LCD_Pins_Config[RS_PIN].port, LCD_Pins_Config[RS_PIN].pin, PIN_HIGH);
/************* copying the command bits to the physical bits ************/
GPIO_Set_Pin_Value(LCD_Pins_Config[D0_PIN].port, LCD_Pins_Config[D0_PIN].pin,(Data&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D1_PIN].port, LCD_Pins_Config[D1_PIN].pin,(Data&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D2_PIN].port, LCD_Pins_Config[D2_PIN].pin,(Data&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D3_PIN].port, LCD_Pins_Config[D3_PIN].pin,(Data&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D4_PIN].port, LCD_Pins_Config[D4_PIN].pin,(Data&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D5_PIN].port, LCD_Pins_Config[D5_PIN].pin,(Data&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D6_PIN].port, LCD_Pins_Config[D6_PIN].pin,(Data&(1<<D0_PIN)));
GPIO_Set_Pin_Value(LCD_Pins_Config[D7_PIN].port, LCD_Pins_Config[D7_PIN].pin,(Data&(1<<D0_PIN)));

/****************** set the enable pin to high ****************/
GPIO_Set_Pin_Value(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, PIN_HIGH);
Enable_Pin_State=ENABLE_PIN_HIGH;
	}

	else if(Enable_Pin_State==ENABLE_PIN_HIGH){
			GPIO_Set_Pin_Value(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, PIN_LOW);
			Enable_Pin_State=ENABLE_PIN_LOW;
	}

}

