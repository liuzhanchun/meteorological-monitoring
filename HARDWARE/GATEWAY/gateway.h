#ifndef __GATEWAY_H
#define __GATEWAY_H
#include "sys.h"

extern void gateway_Init(u32 bound);
extern void gateway_send_Data(u8 *buf,u16 len);

#endif
