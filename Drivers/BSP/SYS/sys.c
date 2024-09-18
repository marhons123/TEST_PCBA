#include "sys.h"

//´óÖÂusÑÓÊ±º¯Êı
void delay_us(volatile uint32_t us)
{
	volatile uint16_t i,j;
	while(us--)
	{
		for(i=0;i<11;i++)
			j++;
	}
}

