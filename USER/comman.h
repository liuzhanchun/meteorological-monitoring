#ifndef __COMMAN_H
#define __COMMAN_H
#include "sys.h"

typedef enum
{
	FALSE = 0,
	TRUE
	
}BOOL; 

extern u8 wind_direction_cmd[8];
extern u8 wind_speed_cmd[8];
extern u8 box_cmd[8];

extern u8 gate_init_cmd[10];
extern u8 led_init_cmd[30];
extern u8 led_PM25_PM10_cmd[100];
extern u8 led_temp_humidity_cmd[100];
extern u8 led_speed_direction_cmd[100];
extern u8 led_noise_atmosphere_cmd[100];
extern u8 led_jingdu_weidu_cmd[100];
extern u8 led_crc[3];

extern int  escape_processing(unsigned short crc,u8 * escape_crc);

extern int g_uart3_irq;
extern void my_strcpy(u8 *dst, u8 *src,u16 len);
extern unsigned short my_CRC(u8 *data, int size);
extern void inttohex_four(int value,u8 *qian_wei,u8 *bai_wei,u8 *shi_wei,u8 *ge_wei);
extern void inttohex_three(int value,u8 *bai_wei,u8 *shi_wei,u8 *ge_wei);


#endif
















