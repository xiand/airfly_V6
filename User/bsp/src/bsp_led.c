#include "bsp.h"


void LED_Status_Set(LED_ID_E id,LED_STATUS_E status)
{
	if(id == BSP_LED_0)
	{
		EXTIO_Set_Pin(LED1,(uint8_t)status);
	}
	else if(id == BSP_LED_1)
	{
		EXTIO_Set_Pin(LED2,(uint8_t)status);
	}
	else if(id == BSP_LED_2)
	{
		EXTIO_Set_Pin(LED3,(uint8_t)status);
	}
	else if(id == BSP_LED_3)
	{
		EXTIO_Set_Pin(LED4,(uint8_t)status);
	}
}

void LED_Status_Toggle(LED_ID_E id)
{
	uint32_t pin;

	if(id == BSP_LED_0)
	{
		pin = LED1;
	}
	else if(id == BSP_LED_1)
	{
		pin = LED2;
	}
	else if(id == BSP_LED_2)
	{
		pin = LED3;
	}
	else if(id == BSP_LED_3)
	{
		pin = LED4;
	}
	else
	{
		return;
	}

	if(EXTIO_Get_Pin(pin) == LED_ON)
	{
		EXTIO_Set_Pin(pin,LED_OFF);
	}
	else
	{
		EXTIO_Set_Pin(pin,LED_ON);
	}
}


uint8_t LED_Status_Check(LED_ID_E id)
{
	uint32_t pin;

	if(id == BSP_LED_0)
	{
		pin = LED1;
	}
	else if(id == BSP_LED_1)
	{
		pin = LED1;
	}
	else if(id == BSP_LED_2)
	{
		pin = LED3;
	}
	else if(id == BSP_LED_3)
	{
		pin = LED4;
	}
	else 
	{
		return 0;
	}
	if(EXTIO_Get_Pin(pin) == LED_ON)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void LED_Init(void)
{
	LED_Status_Set(BSP_LED_0,LED_OFF);
	LED_Status_Set(BSP_LED_1,LED_OFF);
	LED_Status_Set(BSP_LED_2,LED_OFF);
	LED_Status_Set(BSP_LED_3,LED_OFF);
}

