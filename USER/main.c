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

 int main(void)
 {	
	 u8 yy;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	//uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	//printf("Let's begin\r\n");
	 
	display_init(9600);
	while(1)
	{
	/*	if(g_uart3_irq!=0)
		{
			display_sendbyte(0X01);
			g_uart3_irq = 0;
		}*/
		__set_PRIMASK(1);  // �ر����ж�
		
		yy = g_gps_buf[0];
		// RS485_SendByte(0X01);	
//			temp=USART_ReceiveData(USART2);
//		if(temp != 0x00)
//		{
//		delay_ms(1099);
//		}
		__set_PRIMASK(0);    //�����ж�
		
		
		delay_ms(10);
	}
 }

