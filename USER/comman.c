#include "comman.h"
int g_uart3_irq = 0;

void my_strcpy(u8 *dst, u8 *src,u16 len)
{
	int i;
	for(i = 0;i<len;i++)
	{
		*(dst+i) = *(src+i);
	}
}
