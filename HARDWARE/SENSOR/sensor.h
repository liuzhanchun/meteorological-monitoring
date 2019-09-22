#ifndef __SENSOR_H
#define __SENSOR_H
#include "sys.h"

extern int g_wind_direction;
extern int g_wind_speed;
extern int g_box_temp;
extern int g_box_humidity;
extern int g_box_noise;
extern int g_box_pm25;
extern int g_box_pm10;
extern int g_box_atmosphere;

extern void sensor_init(u32 bound);
extern void sensor_senddata(u8 *buf,u16 len);
extern u8 USART3_RX_CNT;
#endif
