
#include "bsp.h"


uint8_t myTest ;


int main(void)
{
	uint8_t i = 0;
	bsp_Init();
	
	while(1)
	{
		i = KEY_Read_FIFO();
		if(i != 0)
		{
			myTest = i;
		}
	}
}

