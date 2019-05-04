
#ifndef _BSP_H_
#define _BSP_H_

#define STM32_V5

#if !defined (STM32_V5) && !defined(STM32_V3)
	#error "some params error,please check"
#endif

//开关全局中断宏
#define ENABLE_INT() __set_PRIMASK(0)
#define DISABLE_INT() __set_PRIMASK(1)

#define BSP_Printf printf

#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#include "bsp_uart_fifo.h"
#include "bsp_timer.h"
//#include "bsp_key.h"
#include "bsp_key_new.h"

void bsp_Init(void);


#endif

