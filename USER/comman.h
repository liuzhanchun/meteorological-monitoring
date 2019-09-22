#ifndef __COMMAN_H
#define __COMMAN_H
#include "sys.h"


extern u8 wind_direction_cmd[8];
extern u8 wind_speed_cmd[8];
extern u8 box_cmd[8];

typedef enum
{
	FALSE = 0,
	TRUE
	
}BOOL; 

extern int g_uart3_irq;
extern void my_strcpy(u8 *dst, u8 *src,u16 len);
	
#endif
















