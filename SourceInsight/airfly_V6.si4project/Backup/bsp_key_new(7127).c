#include "bsp.h"

#define HARD_KEY_NUM  8
#define KEY_COUNT    (HARD_KEY_NUM + 1)

//使能时钟
#define ALL_KEY_GPIO_CLK_ENABLE() {\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);\
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);\
	}

typedef struct
{
	GPIO_TypeDef *gpio;
	uint16_t pin;
	uint8_t active_level;   //按键激活电平
}X_GPIO_T;

static const X_GPIO_T s_gpio_list[HARD_KEY_NUM] = {
	{GPIOI, GPIO_Pin_8, 0},		/* K1 */
	{GPIOC, GPIO_Pin_13, 0},	/* K2 */
	{GPIOH, GPIO_Pin_4, 0},		/* K3 */
	{GPIOG, GPIO_Pin_2, 0},		/* JOY_U */	
	{GPIOF, GPIO_Pin_10, 0},	/* JOY_D */
	{GPIOG, GPIO_Pin_3, 0},		/* JOY_L */	
	{GPIOG, GPIO_Pin_7, 0},		/* JOY_R */	
	{GPIOI, GPIO_Pin_11, 0},	/* JOY_OK */
};


static KEY_T s_tBtn[KEY_COUNT] = {0};
static KEY_FIFO_T s_tkey;   //按键FIFO变量结构体


//#define KEY_PIN_ACTIVE(id)
static uint8_t KEY_Pin_Active(uint8_t id)
{
	uint8_t level;

	if((s_gpio_list[id].gpio->IDR & s_gpio_list[id].pin) == 0)
	{
		level = 0;
	}
	else 
	{
		level = 1;
	}

	if(level == s_gpio_list[id].active_level)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}


static uint8_t KEY_Is_Put_Down_Detect(uint8_t id)
{
	//单按键测试
	if(id < HARD_KEY_NUM)
	{
		uint8_t i;
		uint8_t count = 0;
		uint8_t save = 255;

		//判断是哪个键按下
		for(i = 0; i < HARD_KEY_NUM; i++)
		{
			if(KEY_Pin_Active(id))
			{
				count++;
				save = i;
			}
		}
		if(count == 1 && save == id)
		{
			return 1;   //只判断一个按键，只有单按键按下才有效
		}
		return 0;
	}
	//组合键判断
	if(id == HARD_KEY_NUM + 0)
	{
		if(KEY_Pin_Active(KID_K1) && KEY_Pin_Active(KID_K2))
		{
			return 1;
		}
		else 
		{
			return 0;
		}

	}
}


void KEY_Set_Param(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* 长按时间 0 表示不检测长按键事件 */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* 按键连发的速度，0表示不支持连发 */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* 连发计数器 */
}


static void KEY_Init_Var(void)
{
	uint8_t i;

	//按键FIFO操作
	s_tkey.Read = 0;
	s_tkey.Write = 0;
	s_tkey.Read2 = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* 长按时间 0 表示不检测长按键事件 */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State = 0;							/* 按键缺省状态，0为未按下 */
		s_tBtn[i].RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;						/* 连发计数器 */
	}

	/* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
	
	/* 摇杆上下左右，支持长按1秒后，自动连发 */
	KEY_Set_Param(KID_JOY_U, 100, 6);
	KEY_Set_Param(KID_JOY_D, 100, 6);
	KEY_Set_Param(KID_JOY_L, 100, 6);
	KEY_Set_Param(KID_JOY_R, 100, 6);
}

static void KEY_Init_Hard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t i;

	ALL_KEY_GPIO_CLK_ENABLE();

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	for(i = 0; i < HARD_KEY_NUM; i++)
	{
		GPIO_InitStructure.GPIO_Pin = s_gpio_list[i].pin;
		GPIO_Init(s_gpio_list[i].gpio,&GPIO_InitStructure);
	}
}


void KEY_Init(void)
{
	KEY_Init_Var();
	KEY_Init_Hard();
}

static void KEY_Put_FIFO(uint8_t key_code)
{
	s_tkey.Buf[s_tkey.Write] = key_code;
	if(++s_tkey.Write >= KEY_FIFO_SIZE)
	{
		s_tkey.Write = 0;
	}
}


static uint8_t KEY_Get_FIFO(void)
{
	uint8_t ret;

	if(s_tkey.Read == s_tkey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tkey.Buf[s_tkey.Read];

		if(++s_tkey.Read >= KEY_FIFO_SIZE)
		{
			s_tkey.Read = 0;
		}
		return ret;
	}
}

uint8_t KEY_Get_State(KEY_ID_E key_id)
{
	return s_tBtn[key_id].State;
}


void KEY_Clear_FIFO(void)
{
	s_tkey.Read = s_tkey.Write;
}


static void KEY_Detect(uint8_t id)
{
	KEY_T *pBtn;

	pBtn = &s_tBtn[id];
	pBtn->double_press_time++;
	if(KEY_Is_Put_Down_Detect(id))
	{
		if(pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			pBtn->Count++;
			if(pBtn->State == 0)
			{
				pBtn->State = 1;  //按键按下
			}
			if((pBtn->Count > KEY_LONG_TIME)&&(pBtn->State != 2))  //长按事件
			{
				pBtn->State = 2;
				KEY_Put_FIFO((uint8_t)(4*id + 2));
			}
		} 
	}
	else
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if(pBtn->State == 2)
			{
				pBtn->State = 0;
				KEY_Put_FIFO((uint8_t)(4*id + 4));    //长按按键弹起事件
			}
			if(pBtn->State == 1) //短按
			{
				pBtn->State = 0;
				if(pBtn->double_press_flag == 0)
				{
					pBtn->double_press_flag = 1;
					pBtn->double_press_time = 0;
				}
				else
				{
					if(pBtn->double_press_time < KEY_DOUBLE_PRESS_TIME)
					{
						pBtn->double_press_flag = 0;
						pBtn->State = 0;
						KEY_Put_FIFO((uint8_t)(4*id + 3));  //将双击事件放入FIFO中
					}
				}
			}
			else
			{
				if(pBtn->double_press_flag == 1)
				{
					if(pBtn->double_press_time > KEY_DOUBLE_PRESS_TIME)
					{
						pBtn->double_press_flag = 0;
						pBtn->State = 0;
						KEY_Put_FIFO((uint8_t)(4*id + 1));  //将单击事件放入FIFO中
					}
				}
			}
		}
	}
}

void KEY_Scan_10MS(void)
{
	uint8_t i = 0;

	for(i = 0; i < KEY_COUNT; i++)
	{
		KEY_Detect(i);
	}
}

