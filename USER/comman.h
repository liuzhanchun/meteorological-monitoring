#ifndef __COMMAN_H
#define __COMMAN_H
#include "sys.h"

typedef enum
{
	FALSE = 0,
	TRUE
	
}BOOL; 

extern int g_uart3_irq;
extern void my_strcpy(u8 *dst, u8 *src,u16 len);
	
#endif
















