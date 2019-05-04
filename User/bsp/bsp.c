
#include "bsp.h"

void bsp_Init(void)
{
	bsp_InitUart();
	//bsp_InitKey();
	KEY_Init();
	bsp_InitTimer();
}

void bsp_RunPer10ms(void)
{
	//bsp_KeyScan();
	KEY_Scan_10MS();
}


void bsp_RunPer1ms(void)
{

}

void bsp_Idle(void)
{

}

