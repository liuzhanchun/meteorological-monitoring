#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "stmflash.h"
#include "myiic.h"
#include "display.h"
//#include "usart.h"	
#include "comman.h"
#include "gps.h"
#include "sensor.h"


int main(void)
{	

	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	gps_init(4800);
	sensor_init(4800);
	 
	//uart_init(9600);	 //串口初始化为9600
	//printf("Let's begin\r\n");
	 
//	display_init(9600);
	//__set_PRIMASK(1); 
	while(1)
	{
		
		sensor_senddata(wind_direction_cmd,8);
		delay_ms(1000);
		sensor_senddata(wind_speed_cmd,8);
		delay_ms(1000);
		sensor_senddata(box_cmd,8);
		delay_ms(1000);
		
	/*	if(g_uart3_irq!=0)
		{
			display_sendbyte(0X01);
			g_uart3_irq = 0;
		}*/
	//	__set_PRIMASK(1);  // 关闭总中断
		
		//yy = g_gps_buf[0];
		//gps_senddata(yy,5);
		// RS485_SendByte(0X01);	
//			temp=USART_ReceiveData(USART2);
//		if(temp != 0x00)
//		{
//		delay_ms(1099);
//		}
	//	__set_PRIMASK(0);    //打开总中断
		
		delay_ms(1000);
		
		USART3_RX_CNT = 0;  //一轮循环后清零，防止错误状态出不起来
	}
 }

