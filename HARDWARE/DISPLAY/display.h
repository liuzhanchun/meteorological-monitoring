#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "sys.h"

extern void display_init(u32 bound);            		
extern void display_sendbyte(unsigned char SendData);
extern void display_senddata(u8 *buf,u16 len);

//extern void display_init(u32 bound);            		
//extern void display_sendbyte(unsigned char SendData);
//extern void display_senddata(u8 *buf,u16 len);

#endif
















