#ifndef __GPS_H
#define __GPS_H
#include "sys.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200

extern char *ggaString; 
extern char weidu[20] ;
extern char weidu_counter ;
extern char jingdu[20] ;
extern char jingdu_counter ;
extern char haiba[20] ;
extern char haiba_counter ;
extern char shijian[20] ;
extern char shijian_counter ;

extern void gps_init(u32 bound);
extern void gps_senddata(u8 *buf,u16 len);

//extern const int gps_datalen;
//extern u8 g_gps_buf[];


#endif




