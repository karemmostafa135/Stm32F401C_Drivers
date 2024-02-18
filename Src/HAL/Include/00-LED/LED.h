/*************** connect options ******/
#define LED_CONN_FORWARD 		0
#define LED_CONN_BACKWORD		1

#define LED_STATE_ON			1
#define LED_STATE_OFF			0


typedef struct{
	void * port;
	u32 pin;
	u8 connection;
	u8 current_state;
}led_cfg_t;

void LED_Init();

Void LED_Set_State(u32 led,u8 status);
