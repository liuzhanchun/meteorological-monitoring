#ifndef __COMMAN_H
#define __COMMAN_H
#include "sys.h"


extern u8 wind_direction_cmd[8];
extern u8 wind_speed_cmd[8];
extern u8 box_cmd[8];

extern u8 led_init_cmd[30];
extern u8 led_PM25_PM10_cmd[93];
extern u8 led_humidity_temp_cmd[90];
extern u8 led_speed_direction_cmd[90];
extern u8 led_noise_atmosphere_cmd[90];
extern u8 led_jingdu_weidu_cmd[90];
extern u8 led_crc[3];


typedef enum
{
	FALSE = 0,
	TRUE
	
}BOOL; 

extern int g_uart3_irq;
extern void my_strcpy(u8 *dst, u8 *src,u16 len);
extern unsigned short my_CRC(u8 *data, int size);
	

#endif
















