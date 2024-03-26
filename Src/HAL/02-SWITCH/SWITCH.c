#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "SWITCH.h"
#include "SWITCH_Cfg.h"


SWITCH_Errors_t SWITCH_Get_Read(uint32_t Switch_num,uint32_t* Read);


#define NULL		0

static uint32_t Switches_Status[_Switch_num];

extern const SWITCH_t Switches[_Switch_num];

void SWITCH_Init(){
RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOAEN, ENABLE_PERI);
RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOBEN, ENABLE_PERI);
RCC_AHB1ENR_Enable_Disable_peri(AHB1ENR_GPIOCEN, ENABLE_PERI);
Pin_Config_t pin;
uint32_t counter=0;
pin.Speed=GPIO_SPEED_HIGH;
for (counter=0;counter<_Switch_num;counter++){
	pin.Mode=Switches[counter].mode;
	pin.Port_num=Switches[counter].port;
	pin.Pin_num=Switches[counter].pin;
	GPIO_PinConfig(&pin);
}
}

SWITCH_Errors_t SWITCH_Get_Status(uint32_t Switch_num,uint32_t* Read){
	SWITCH_Errors_t Local_Error=SWITCH_Ok;
	if (Switch_num>_Switch_num){
		Local_Error=SWITCH_Nok;
	}
	else if(Read==NULL){
		Local_Error=SWITCH_Nok;
	}
	else{
		* Read=Switches_Status[Switch_num];
	}
	return Local_Error;
}


SWITCH_Errors_t SWITCH_Get_Read(uint32_t Switch_num,uint32_t* Read){
	SWITCH_Errors_t Local_Error=SWITCH_Ok;
if (Switch_num>_Switch_num){
	Local_Error=SWITCH_Nok;
}
else if(Read==NULL){
	Local_Error=SWITCH_Nok;
}
else{
	GPIO_Get_Pin_Value(Switches[Switch_num].port,Switches[Switch_num].pin,Read);
}
	switch(Switches[Switch_num].mode){
case GPIO_MODE_INPUT_PU: *Read=!(*Read); break;
case GPIO_MODE_INPUT_PD: *Read=*Read; break;
case GPIO_MODE_INPUT_FLOATING: * Read=*Read; break ;
default: Local_Error=SWITCH_Nok; break;

	//*Read=(*Read)^Switches[Switch_num].mode;
}
/*
switch(Switches[Switch_num].mode){
case GPIO_MODE_INPUT_PU: *Read=!helper; break;
case GPIO_MODE_INPUT_PD: *Read=helper; break;
case GPIO_MODE_INPUT_FLOATING: * Read=helper; break ;
default: Local_Error=SWITCH_Nok; break;
}
*/
return Local_Error;
}



void Switch_task(){
uint32_t idx;
uint32_t current;
static uint32_t prev[_Switch_num]={0};
static uint32_t counts[_Switch_num]={0};

for (idx=0;idx<_Switch_num;idx++){
	SWITCH_Get_Read(idx,&current);
	if(current==prev[idx]){
		counts[idx]++;
	}
	else{
		counts[idx]=0;
	}
	if(counts[idx]==5){
		Switches_Status[idx]=current;
	}
	else{
	//do nothing
	}
	prev[idx]=current;
}
}

