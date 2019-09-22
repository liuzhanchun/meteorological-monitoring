#include "comman.h"

int g_uart3_irq = 0;
u8 wind_direction_cmd[8] = {0x0a,0x03,0x00,0x00,0x00,0x02,0xc5,0x70};
u8 wind_speed_cmd[8]     = {0x0b,0x03,0x00,0x00,0x00,0x01,0x84,0xa0};
u8 box_cmd[8]            = {0x03,0x03,0x01,0xF4,0x00,0x06,0x84,0x24};

u8 led_init_cmd[30]  ={0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x01,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x51,  //�����̬��ʾ
                       0x02,0x05,0x00,0xA2,0x00,0x01,0x00,0x00,0xD1,0x0F,0x5A};

u8 led_PM25_PM10_cmd[93]={0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x01,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x02,
	0x44,0x00,0xA3,0x06,0x01,0x00,0x00,0x00,0x01,0x3B,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x20,0x00,0x00,0x03,0x00,0x0A,0x00,
	0x00,0x00,0x00,0x02,0x02,0x01,0x00,0x0A,0x0A,0x20,0x00,0x00,0x00,0x50,0x4D,0x32,0x2E,0x35,0xA3,0xBA,
	0x20,0x35,0x34,0xA6,0x01,0xCC,0x67,0x2F,0x6D,0x33,0x50,0x4D,0x31,0x30,0xA3,0xBA,//67
	0x20,0x20,0x37,0x30,0xA6,0x01,0xCC,0x67,0x2F,0x6D,0x33,0xF7};//84
//0xA6,0x01,0x5A

u8 led_humidity_temp_cmd[90]={0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x01,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x51,
	0x02,0x44,0x00,0xA3,0x06,0x01,0x00,0x00,0x00,0x01,0x3B,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x20,0x00,0x00,0x03,0x00,0x0A,
	0x00,0x00,0x00,0x00,0x02,0x02,0x01,0x00,0x0A,0x0A,0x20,0x00,0x00,0x00,0xCE,0xC2,0xB6,0xC8,0xA3,0xBA,
	0x20,0x20,0x32,0x32,0x2E,0x33,0xA1,0xE6,0x20,0x20,0xCA,0xAA,0xB6,0xC8,0xA3,0xBA, //69
	0x20,0x20,0x33,0x33,0x2E,0x35,0x25,0x52,0x48,0x20}; //85
	//0xDC,0xC3,0x5A};
u8 led_speed_direction_cmd[90]={0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x01,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x51,0x02,0x44,0x00,0xA3,0x06,0x01,0x00,0x00,0x00,0x01,0x3B,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x20,0x00,0x00,0x03,0x00,
	0x0A,0x00,0x00,0x00,0x00,0x02,0x02,0x01,0x00,0x0A,0x0A,0x20,0x00,0x00,0x00,0xB7,0xE7,0xCB,0xD9,0xA3,0xBA, 
	0x20,0x20,0x33,0x2E,0x30,0x20,0x6D,0x2F,0x73,0x20,0xB7,0xE7,0xCF,0xF2,0xA3,0xBA, //68
  0x20,0x20,0x32,0x37,0x30,0xA1,0xE3,0x20,0x20,0x20}; //84
//0x7B,0x83,0x5A};
u8 led_noise_atmosphere_cmd[90]={0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x01,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x51,
	0x02,0x44,0x00,0xA3,0x06,0x01,0x00,0x00,0x00,0x01,0x3B,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x20,0x00,0x00,0x03,0x00,0x0A,
	0x00,0x00,0x00,0x00,0x02,0x02,0x01,0x00,0x0A,0x0A,0x20,0x00,0x00,0x00,0xD4,0xEB,0xC9,0xF9,0xA3,0xBA,
	0x20,0x20,0x35,0x32,0x2E,0x35,0x64,0x62,0x20,0x20,0xB4,0xF3,0xC6,0xF8,0xD1,0xB9,0xA3,0xBA, //69
	0x31,0x30,0x31,0x2E,0x33,0x4B,0x50,0x61};  //86
//0xFA,0xD2,0x5A}
u8 led_jingdu_weidu_cmd[90]={0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0xA5,0x01,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x51,
	0x02,0x44,0x00,0xA3,0x06,0x01,0x00,0x00,0x00,0x01,0x3B,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x20,0x00,0x00,0x03,0x00,0x0A,
	0x00,0x00,0x00,0x00,0x02,0x02,0x01,0x00,0x0A,0x0A,0x20,0x00,0x00,0x00,0xBE,0xAD,0xB6,0xC8,0xA3,0xBA,
	0x20,0x31,0x31,0x37,0x2E,0x35,0x36,0x45,0x20,0x20,0xCE,0xB3,0xB6,0xC8,0xA3,0xBA, //70
  0x20,0x33,0x39,0x2E,0x30,0x35,0x4E,0x20,0x20,0x20}; //85
//0xBE,0xC9,0x5A};
	
u8 led_value[10];
u8 led_crc[3]={0x00,0x00,0x5A};

unsigned short tabel_CRC16[256] = {
0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};
void my_strcpy(u8 *dst, u8 *src,u16 len)
{
	int i;
	for(i = 0;i<len;i++)
	{
		*(dst+i) = *(src+i);
	}
}

unsigned short my_CRC(u8 *data, int size) 
{
	unsigned short tempcrc = 0;
	unsigned short temp = 0;
	int i =0;
	for (i=0; i<size; i++)
	{
		temp = ((tempcrc & 0xff) ^ (data[i] & 0xff));
		tempcrc = ((tempcrc>>8) & 0xff) ^ tabel_CRC16[temp];
	}
	return tempcrc;
}
