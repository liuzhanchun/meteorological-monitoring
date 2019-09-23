#include "sensor.h"
#include "delay.h"
#include "comman.h"

int g_wind_direction = 0;
int g_wind_speed = 0;
int g_box_temp = 0;
int g_box_humidity = 0;
int g_box_noise = 0;
int g_box_pm25 = 0;
int g_box_pm10 = 0;
int g_box_atmosphere = 0;

/*RE��GPIOC5�ܽ�����*/   
#define Set_RE  GPIO_SetBits(GPIOC,GPIO_Pin_5);			 
#define Clr_RE  GPIO_ResetBits(GPIOC,GPIO_Pin_5);
/*DE��GPIOC4�ܽ�����*/
#define Set_DE  GPIO_SetBits(GPIOC,GPIO_Pin_4);
#define Clr_DE  GPIO_ResetBits(GPIOC,GPIO_Pin_4);


//void DelayNuS(unsigned int i)
//{
//        unsigned char t = 0;
//        for(;i>0;i--)
//        {
//                for(t=0;t<2;t++)
//                {
//                }
//        }
//} 

void sensor_init(u32 bound){
  //GPIO�˿�����
	
    
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	/* ʹ�� USART3 ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ������ʱ��	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* USART3 ʹ��IO�˿����� */    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIOA
	

  //Usart3 NVIC ����
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
  /* Enable the USART3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* USART3 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = bound;	//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);  //��ʼ��USART3
	
  /*ʹ�ܴ���3�ķ��ͺͽ����ж�*/
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);// USART3ʹ��
	USART_ClearFlag(USART3,USART_FLAG_TC);
	
}
 /*����һ���ֽ�����*/
 void sensor_sendbyte(unsigned char SendData)
{	   
	Set_DE;
	Set_RE;
	delay_ms(1);
	
	USART_SendData(USART3,SendData);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	
	delay_ms(1);
	Clr_DE;
	Clr_RE; 	    
} 

//void sensor_senddata(u8 *buf,u16 len)
//{
//	u16 t;
//		Set_DE;
//	Set_RE;
//	//delay_ms(10);
//	for(t=0;t<len;t++)     
//	{           

//	
//	USART_SendData(USART3,buf[t]);
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//	
//	
//	}     
//		//delay_ms(100);
//	Clr_DE;
//	Clr_RE; 
//	

//}

void sensor_senddata(u8 *buf,u16 len)
{
	u16 t;
	Set_DE;
	Set_RE;
	delay_ms(1);
	for(t=0;t<len;t++)     
	{           
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
			USART_SendData(USART3,buf[t]); 
	}     
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
	
	delay_ms(1);
	Clr_DE;
	Clr_RE; 
}


u8 USART3_RX_CNT=0;
u8 USART3_RX_BUF[30]={0x00}; 
void USART3_IRQHandler(void)
{ 
	u8 Res;

  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    Res = USART_ReceiveData(USART3);
		
		USART3_RX_BUF[USART3_RX_CNT]=Res;         
		USART3_RX_CNT++;   
		
		//��������
		if((USART3_RX_BUF[0]==0x0a)&&(USART3_RX_CNT==9))
		{
			g_wind_direction = USART3_RX_BUF[5]*0x100+USART3_RX_BUF[6];
			USART3_RX_CNT = 0;
		}
		//��������
		if((USART3_RX_BUF[0]==0x0b)&&(USART3_RX_CNT==7))
		{
			g_wind_speed = USART3_RX_BUF[3]*0x100+USART3_RX_BUF[4];
			USART3_RX_CNT = 0;
		}
		
		//����box
		if((USART3_RX_BUF[0]==0x03)&&(USART3_RX_CNT==17))
		{
			g_box_humidity = USART3_RX_BUF[3]*0x100+USART3_RX_BUF[4];
			g_box_temp= USART3_RX_BUF[5]*0x100+USART3_RX_BUF[6];
			g_box_noise = USART3_RX_BUF[7]*0x100+USART3_RX_BUF[8];
			g_box_pm25 = USART3_RX_BUF[9]*0x100+USART3_RX_BUF[10];
			g_box_pm10 = USART3_RX_BUF[11]*0x100+USART3_RX_BUF[12];
			g_box_atmosphere = USART3_RX_BUF[13]*0x100+USART3_RX_BUF[14];
			USART3_RX_CNT = 0;
		}
		
//		if(USART3_RX_CNT>20)
//		{
//			USART3_RX_CNT = 0;
//		}
//		
	}
	// ���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE) == SET)
	{
			USART_ReceiveData(USART3);
			USART_ClearFlag(USART3,USART_FLAG_ORE);
	}		
	USART_ClearFlag(USART3,USART_IT_RXNE); 
	//delay_ms(2);
}

