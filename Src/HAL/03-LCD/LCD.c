#include "stdio.h"
#include "GPIO_interface.h"
#include "LCD.h"

/****************** macros for lcd commands ************/
#define LCD_FUNCTION_SET_MASK		0x38
#define LCD_DISPLAY_ON_OFF_MASK		0x0F
#define LCD_DISPLAY_CLEAR_MASK		0x01
#define LCD_ENTRY_MODE_MASK			0x06
#define LCD_SHIFT_LEFT_MASK			0x18
#define LCD_SHIFT_RIGHT_MASK		0x1C
#define LCD_CURSOR_OFF				0x0C
#define LCD_CURSOR_ON 				0x0E
#define LCD_SET_CURSOR_LOCATION 	0x80
#define LCD_SET_DDRAM_ADRESS		128

/***************** macros for goto_XY function to set the location to write   ***************/
#define FIRST_LINE			0
#define SECOND_LINE			1

#define NUMBER_OF_REQUESTES				13
#define DATE_TIME_MODE					0
#define STOP_WATCH_MODE					1

#define CHANGED							1
#define NOT_CHANGED						0

 void Init_Sm();

 void LCD_Write_Command(uint8_t Command);


 void Sec_Increment_Task(void);
 void LCD_Write_Data(uint8_t Data);

 void LCD_Write_String_Helper(void);

 void LCD_Write_Number_Helper();

 void Write_Date_Time_Task();

 void LCD_Set_Cursor_Helper();

 void LCD_Clear_Display_Helper();


extern LCD_pin_Cfg LCD_Pins_Config[NUMBER_OF_LCD_PINS];


/*************** enable pin states **************/
#define ENABLE_PIN_LOW				0
#define ENABLE_PIN_HIGH				1


#define READY		0

#define BUSY		1

typedef enum{
	year,
	month,
	day,
	hours,
	minutes,
	seconds,
	milliseconds,
	end
}Date_Time_t;


typedef struct{
	uint8_t * s;
	uint8_t Number[10];
	uint8_t length;
	uint8_t state;
	uint8_t Type;
	uint8_t Cursor_Pos;
	uint8_t Command;
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

typedef struct{
	uint32_t	Year;
	uint32_t	Month;
	uint32_t	Day;
}Current_Date;

typedef struct
{
	uint32_t 	Hours;
	uint32_t	Minutes;
	uint32_t	Seconds;

}Current_Time;

/***************** request types enum *********************/

typedef enum{
	None,
	Write_Data,
	Write_Number,
	Set_Cursor,
	Clear_Display

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

Date_Time_t Current=year;

/************** to get the current time to display *******************/
Current_Time Time={
.Hours=11,
.Minutes=22,
.Seconds=14
};


Current_Date Date={
		.Year=2024,
		.Month=4,
		.Day=5
};

static User_Req Req[NUMBER_OF_REQUESTES]
={
	[0]={.s=NULL,
		.length=0,
		.state=READY,
		.Type=None,
		.Cursor_Pos=0,
		.Command=0,
		.Number={0}
	},

	[1]={.s=NULL,
		.length=0,
		.state=READY,
		.Type=None,
		.Cursor_Pos=0,
		.Command=0,
		.Number={0}
	},

	[2]={.s=NULL,
		.length=0,
		.state=READY,
		.Type=None,
		.Cursor_Pos=0,
		.Command=0,
		.Number={0}
	},
	[3]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[4]={.s=NULL,
			.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
			},
			[5]={.s=NULL,
					.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
				[6]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[7]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[8]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[9]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[10]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[11]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[12]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[13]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
		[14]={.s=NULL,
						.length=0,
			.state=READY,
			.Type=None,
			.Cursor_Pos=0,
			.Command=0,
			.Number={0}
		},
};

volatile static uint8_t Request_Index=0;


volatile static uint8_t Counter_to_Clear_Screen=0;
volatile uint8_t Curren_Display_Mode=DATE_TIME_MODE;
uint8_t Change_Of_Time=CHANGED;

/*** task to write the date and time on lcd ***/
void Write_Date_Time_Task(){
	/*to clear the display at the first time of this task */
	if(Curren_Display_Mode==DATE_TIME_MODE){
	/*if(Counter_to_Clear_Screen==0){
	LCD_Clear_Display_Asynch();
	Counter_to_Clear_Screen++;
	}*/
	if(Change_Of_Time==CHANGED){
	LCD_Clear_Display_Asynch();
	/**** to init the cursor position */
	LCD_Set_Cursor_Asynch(0,0);
  /* to print the date : YEAR/MONTH/DAY */
   LCD_Wrtite_Number_Asynch(Date.Year);
   LCD_Set_Cursor_Asynch(0,5);
   //LCD_Write_Data('/');
   LCD_Wrtite_Number_Asynch(Date.Month);
   LCD_Set_Cursor_Asynch(0,7);
   //LCD_Write_Data('/');
     LCD_Wrtite_Number_Asynch(Date.Day);

     /* Set the cursor on the second Line */
    LCD_Set_Cursor_Asynch(1,2);

     /* to print the Time : HOURS:MINUTES:SECONDS */
     LCD_Wrtite_Number_Asynch(Time.Hours);
     //LCD_Write_Data(':');
     LCD_Set_Cursor_Asynch(1,6);
     LCD_Wrtite_Number_Asynch(Time.Minutes);
     //LCD_Write_Data(':');
     LCD_Set_Cursor_Asynch(1,9);
    LCD_Wrtite_Number_Asynch(Time.Seconds);
     Change_Of_Time=NOT_CHANGED;
	}
	}

}

//task every 1000 miliSecond
void Sec_Increment_Task(void)
{
  Time.Seconds++ ;
  Change_Of_Time=CHANGED;
  if(Time.Seconds==60)
  {
    Time.Minutes++ ;
    Time.Seconds = 0 ;
  }
  else
  {
    // do nothing
  }
  if(Time.Minutes == 60 )
  {
    Time.Hours++ ;
    Time.Minutes = 0 ;
  }
  else
  {
    // do nothing
  }
  if(Time.Hours == 24)
  {
    Date.Day++ ;
    Time.Hours = 0 ;
  }
  else
  {
    // do nothing
  }
  if(Date.Day == 30)
  {
    Date.Month++ ;
    Date.Day = 0 ;
  }
  else
  {
    // do nothing
  }
  if(Date.Month == 12)
  {
    Date.Year++ ;
    Date.Month = 0 ;
  }
  else
  {
    // do nothing
  }

}

void LCD_Init_Asynch(){
	 Lcd_State=Init;
}


/******************* LCD Runnable *****************/
void LCD_Task(){
	if(Lcd_State==Init){
		Init_Sm();
	}
	else if (Lcd_State==Operation){
		switch(Req[Request_Index].Type){
		case Write_Data:
			//LCD_enuGotoDDRAM_XY(Cordinates.X_pos,Cordinates.Y_pos);
			LCD_Write_String_Helper();
			//increment the index and break
		break;
		case Clear_Display:
			LCD_Clear_Display_Helper();
			break;
		case Set_Cursor:
			LCD_Set_Cursor_Helper();
			break;
		case Write_Number:
			LCD_Write_Number_Helper();
		break;
		}
	}
//to reset the Request index
	if(Request_Index==NUMBER_OF_REQUESTES){
		Request_Index=0;
	}
}

void LCD_Set_Cursor_Asynch(uint8_t Copy_u8Row, uint8_t Copy_u8Column){
	uint8_t Loc_u8Location =0;
	uint8_t counter=0;
	for(counter;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){
		Req[counter].state=BUSY;
		Req[counter].Type=Set_Cursor;
		if(Copy_u8Row==FIRST_LINE){
			Loc_u8Location=Copy_u8Column;
		}
		else if (Copy_u8Row==SECOND_LINE){
			Loc_u8Location=Copy_u8Column+0x40 ;
		}
		else{
			//do noting
		}
		Req[counter].Command=LCD_SET_DDRAM_ADRESS+Loc_u8Location;
		counter=NUMBER_OF_REQUESTES; // to terminate from the loop
	}
	}
}





/******************** function to init the lcd ************/
void Init_Sm(){
static uint8_t counter=0;
Pin_Config_t pin;
uint8_t idx=0;
counter++;
switch (Mode_Of_Init){
case Power_On:
/***** to initialize the lcd pins ************/
	if(counter==1){
	pin.Mode=GPIO_MODE_OUTPUT_PP;
	pin.Speed=GPIO_SPEED_HIGH;
	for(idx=0;idx<NUMBER_OF_LCD_PINS;idx++){
		pin.Pin_num=LCD_Pins_Config[idx].pin;
		pin.Port_num=LCD_Pins_Config[idx].port;
		pin.Af=0;
		GPIO_PinConfig(&pin);
	}
	}
	else if (counter==15){
		counter=0;
	Mode_Of_Init=Function_Set;
	}
	else{
		//do nothing
	}
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
		Mode_Of_Init=Display_Clear;
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


/********************* to get the string from the user **************/
void LCD_Write_String_NoCopy(uint8_t * str,uint32_t length){
	uint8_t counter=0;
	if(str){
			for(counter;counter<NUMBER_OF_REQUESTES;counter++){
if(Req[counter].state==READY){
	Req[counter].state=BUSY;
	Req[counter].s=str;
	Req[counter].length=length;
	Req[counter].Type=Write_Data;
	counter=NUMBER_OF_REQUESTES; // to terminate from the loop
}
			}
	}
}

/************************* to get command request from the user *************/
void LCD_Clear_Display_Asynch(){
	uint8_t counter=0;
	for(counter;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){
		Req[counter].state=BUSY;
		Req[counter].Command=LCD_DISPLAY_CLEAR_MASK;
		Req[counter].Type=Clear_Display;
		counter=NUMBER_OF_REQUESTES; // to terminate from the loop
}
	}
}

/****************** helper function to clear the displayy**********/
void LCD_Clear_Display_Helper(){
LCD_Write_Command(Req[Request_Index].Command);
if(Enable_Pin_State==ENABLE_PIN_LOW){
	Req[Request_Index].Type=None;
	Req[Request_Index].state=READY;
	Request_Index++;
	}
}



/******************* function to write number *************/
void LCD_Wrtite_Number_Asynch(uint32_t Copy_number){
	uint8_t counter=0;
	uint8_t digit=0;
		for(counter;counter<NUMBER_OF_REQUESTES;counter++){
	if(Req[counter].state==READY){

		Req[counter].state=BUSY;
		Req[counter].Type=Write_Number;

		if(Copy_number == 0) {
						LCD_Write_Data('0');
				}

				if(Copy_number < 0) {
					LCD_Write_Data('-');
					Copy_number = -Copy_number; // Convert negative number to positive
				}

				// Array to hold digits in reverse order
				uint32_t index = 0;

				// Extract digits from right to left
				while(Copy_number > 0) {
					digit = Copy_number % 10;
					Req[counter].Number[index++] = digit + '0'; // Convert digit to ASCII character
					Copy_number /= 10;
				}
				Req[counter].length=index;
				counter=NUMBER_OF_REQUESTES; // to terminate from the loop
	}


	}

}

/************************ helper function to write number ************/
void LCD_Write_Number_Helper(){
		static uint8_t iterator=0;
/******************** set the needed position **************/


	if(iterator<Req[Request_Index].length){
			LCD_Write_Data(Req[Request_Index].Number[Req[Request_Index].length-iterator-1]);
			if(Enable_Pin_State==ENABLE_PIN_LOW){
			iterator++;
			}

	}
	// when writing the string finish
	else{
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		 memset(Req[Request_Index].Number, 0, sizeof(Req[Request_Index].Number));
		Request_Index++;
		iterator=0;
	}
}

/********************* helper function to write the string on lcd *******/
void LCD_Write_String_Helper(void){
	static uint8_t iterator=0;
/******************** set the needed position **************/


	if(iterator<Req[Request_Index].length){
			LCD_Write_Data(Req[Request_Index].s[iterator]);
			if(Enable_Pin_State==ENABLE_PIN_LOW){
			iterator++;
			}

	}
	// when writing the string finish
	else{
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		Request_Index++;
		iterator=0;
	}
}

void LCD_Write_Command_Helper(){
	LCD_Write_Command(Req[Request_Index].s[0]);
	if(Enable_Pin_State==ENABLE_PIN_LOW){
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		Request_Index++;
	}
}


void LCD_Set_Cursor_Helper()
{
LCD_Write_Command(Req[Request_Index].Command);
if(Enable_Pin_State==ENABLE_PIN_LOW){
		Req[Request_Index].Type=None;
		Req[Request_Index].state=READY;
		Request_Index++;
	}
}


/******************* to send a one byte command to Lcd ***********/
void LCD_Write_Command(uint8_t Command){


if(Enable_Pin_State==ENABLE_PIN_LOW){


/******* to set the RW and RS pins to LOw ************/
GPIO_Set_Pin_Value(LCD_Pins_Config[RW_PIN].port, LCD_Pins_Config[RW_PIN].pin, PIN_LOW);
GPIO_Set_Pin_Value(LCD_Pins_Config[RS_PIN].port, LCD_Pins_Config[RS_PIN].pin, PIN_LOW);
/************* copying the command bits to the physical bits ************/
GPIO_Set_Pin_Value(LCD_Pins_Config[D0_PIN].port, LCD_Pins_Config[D0_PIN].pin,((Command>>D0_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D1_PIN].port, LCD_Pins_Config[D1_PIN].pin,((Command>>D1_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D2_PIN].port, LCD_Pins_Config[D2_PIN].pin,((Command>>D2_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D3_PIN].port, LCD_Pins_Config[D3_PIN].pin,((Command>>D3_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D4_PIN].port, LCD_Pins_Config[D4_PIN].pin,((Command>>D4_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D5_PIN].port, LCD_Pins_Config[D5_PIN].pin,((Command>>D5_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D6_PIN].port, LCD_Pins_Config[D6_PIN].pin,((Command>>D6_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D7_PIN].port, LCD_Pins_Config[D7_PIN].pin,((Command>>D7_PIN)&1));

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
GPIO_Set_Pin_Value(LCD_Pins_Config[D0_PIN].port, LCD_Pins_Config[D0_PIN].pin,((Data>>D0_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D1_PIN].port, LCD_Pins_Config[D1_PIN].pin,((Data>>D1_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D2_PIN].port, LCD_Pins_Config[D2_PIN].pin,((Data>>D2_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D3_PIN].port, LCD_Pins_Config[D3_PIN].pin,((Data>>D3_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D4_PIN].port, LCD_Pins_Config[D4_PIN].pin,((Data>>D4_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D5_PIN].port, LCD_Pins_Config[D5_PIN].pin,((Data>>D5_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D6_PIN].port, LCD_Pins_Config[D6_PIN].pin,((Data>>D6_PIN)&1));
GPIO_Set_Pin_Value(LCD_Pins_Config[D7_PIN].port, LCD_Pins_Config[D7_PIN].pin,((Data>>D7_PIN)&1));

/****************** set the enable pin to high ****************/
GPIO_Set_Pin_Value(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, PIN_HIGH);
Enable_Pin_State=ENABLE_PIN_HIGH;
	}

	else if(Enable_Pin_State==ENABLE_PIN_HIGH){
			GPIO_Set_Pin_Value(LCD_Pins_Config[EN_PIN].port, LCD_Pins_Config[EN_PIN].pin, PIN_LOW);
			Enable_Pin_State=ENABLE_PIN_LOW;
	}

}









