#include "bsp.h"

//访问时设置为32位访问，如果是8位或者16位该变量需要进行修改
#define HC574_PORT_ADDR *(uint32_t *)0x64001000

__IO uint32_t g_HC574_data;  //地址数据设置


static void HC574_Fmc_Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//GPIO时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|\
	RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI,ENABLE);

	//FMC时钟使能
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC,ENABLE);

	//设置 FMC相关GPIO为复用推完输出
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_9;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
						| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOH, &GPIO_InitStructure);


	GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource3, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6
						| GPIO_Pin_7 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
}

static void HC574_Fmc_Config(void)
{
	FMC_NORSRAMInitTypeDef init;
	FMC_NORSRAMTimingInitTypeDef timing;

	timing.FMC_AddressSetupTime = 3;
	timing.FMC_AddressHoldTime = 0;
	timing.FMC_DataSetupTime = 6;
	timing.FMC_BusTurnAroundDuration = 1;
	timing.FMC_CLKDivision = 0;
	timing.FMC_DataLatency = 0;
	timing.FMC_AccessMode = FMC_AccessMode_A;


	init.FMC_Bank = FMC_Bank1_NORSRAM2;
	init.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
	init.FMC_MemoryType = FMC_MemoryType_SRAM;
	init.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_32b;
	init.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
	init.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	init.FMC_WrapMode = FMC_WrapMode_Disable;
	init.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
	init.FMC_WriteOperation = FMC_WriteOperation_Enable;
	init.FMC_WaitSignal = FMC_WaitSignal_Disable;
	init.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
	init.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;
	init.FMC_WriteBurst = FMC_WriteBurst_Disable;
	init.FMC_ContinousClock = FMC_CClock_SyncAsync;

	init.FMC_ReadWriteTimingStruct = &timing;
	init.FMC_WriteTimingStruct = &timing;

	FMC_NORSRAMInit(&init);

	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM2,ENABLE);
}

void EXTIO_Init(void)
{
	HC574_Fmc_Gpio_Config();
	HC574_Fmc_Config();

	//将一些片选和LED口设置为高
	g_HC574_data = (NRF24L01_CE | VS1053_XDCS | LED1 | LED2 | LED3 | LED4);

	HC574_PORT_ADDR = g_HC574_data;
}


void EXTIO_Set_Pin(uint32_t pin, uint8_t pin_value)
{
	if(pin_value == 0)
	{
		g_HC574_data &= (~pin);
	}
	else
	{
		g_HC574_data |= pin;
	}
	HC574_PORT_ADDR = g_HC574_data;
}


void EXTIO_Get_Pin(uint32_t pin)
{
	if(g_HC574_data & pin)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


