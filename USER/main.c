#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "stmflash.h"
#include "myiic.h"
#include "rs485.h"
#include "usart.h"	
#include "comman.h"
 int main(void)
 {	
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	printf("Let's begin\r\n");
	 
	rs485_init(9600);
	while(1)
	{
		if(g_uart3_irq!=0)
		{
			RS485_SendByte(0X01);
			g_uart3_irq = 0;
		}
		// RS485_SendByte(0X01);	
//			temp=USART_ReceiveData(USART2);
//		if(temp != 0x00)
//		{
//		delay_ms(1099);
//		}
		delay_ms(10);
	}
 }

