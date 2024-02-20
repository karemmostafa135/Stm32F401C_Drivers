#include "GPIO_interface.h"
#include "SWITCH.h"
#include "SWITCH_Cfg.h"

#define NULL		0

extern const SWITCH_t Switches[_Switch_num];

void SWITCH_Init(){
Pin_Config_t pin;
u32 counter=0;
pin.Speed=GPIO_SPEED_HIGH;
for (counter=0;counter<_Switch_num;counter++){
	pin.Mode=Switches[counter].mode;
	pin.Port_num=Switches[counter].port;
	pin.Pin_num=Switches[counter].pin;
	GPIO_PinConfig(&pin);
}
}

SWITCH_Errors_t SWITCH_Get_Read(u32 Switch_num,u32* Read){
	SWITCH_Errors_t Local_Error=SWITCH_Ok;
	u32 helper;
if (Switch_num>_Switch_num){
	Local_Error=SWITCH_Nok;
}
else if(Read==NULL){
	Local_Error=SWITCH_Nok;
}
else{
	GPIO_Get_Pin_Value(Switches[Switch_num].port,Switches[Switch_num].pin,&helper);
}
switch(Switches[Switch_num].mode){
case GPIO_MODE_INPUT_PU: *Read=!helper; break;
case GPIO_MODE_INPUT_PD: *Read=helper; break;
case GPIO_MODE_INPUT_FLOATING: * Read=helper; break ;
default: Local_Error=SWITCH_Nok; break;
}
return Local_Error;
}
