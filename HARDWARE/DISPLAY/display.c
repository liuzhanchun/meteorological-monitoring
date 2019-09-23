#include "display.h"
#include "delay.h"

/*RE��GPIOC3�ܽ�����*/   
#define Set_RE_display  GPIO_SetBits(GPIOC,GPIO_Pin_3);			 
#define Clr_RE_display  GPIO_ResetBits(GPIOC,GPIO_Pin_3);
/*DE��GPIOC2�ܽ�����*/
#define Set_DE_display  GPIO_SetBits(GPIOC,GPIO_Pin_2);
#define Clr_DE_display  GPIO_ResetBits(GPIOC,GPIO_Pin_2);


void display_init(u32 bound){
  //GPIO�˿�����
	
    
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	/* ʹ�� USART4 ʱ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ������ʱ��	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* USART4 ʹ��IO�˿����� */    //tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
                             
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  //rx
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);   //��ʼ��GPIOA
	

  //Usart4 NVIC ����
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  /* Enable the USART4 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* USART4 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = bound;	//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(UART4, &USART_InitStructure);  //��ʼ��USART4
	
  /*ʹ�ܴ���4�ķ��ͺͽ����ж�*/
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
	USART_Cmd(UART4, ENABLE);// USART4ʹ��
	USART_ClearFlag(UART4,USART_FLAG_TC);
	
}
 /*����һ���ֽ�����*/
 void display_sendbyte(unsigned char SendData)
{	   
	Set_DE_display;
	Set_RE_display;
	delay_ms(1);
	USART_SendData(UART4,SendData);
	while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
	delay_ms(1);
	Clr_DE_display;
	Clr_RE_display; 	    
} 
extern void display_senddata(u8 *buf,u16 len)
{
	u16 t;
	Set_DE_display;
	Set_RE_display;
	delay_ms(1);
	for(t=0;t<len;t++)     
	{           
			while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
			USART_SendData(UART4,buf[t]); 
	}     
	while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);  
	
	delay_ms(1);
	Clr_DE_display;
	Clr_RE_display; 
}

void UART4_IRQHandler(void)                    //UART4 Receive Interrupt 
{
    u8 Res;
    
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
    {    
        Res =USART_ReceiveData(UART4);//(USART1->DR);    //????????    
    
    }
    
//    if( USART_GetITStatus(UART4, USART_IT_TC) == SET )
//    {
//        USART_ClearFlag(UART4, USART_FLAG_TC);
//    }    
    
	// ���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����
	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE) == SET)
	{
			USART_ReceiveData(UART4);
			USART_ClearFlag(UART4,USART_FLAG_ORE);
	}		
	USART_ClearFlag(UART4,USART_IT_RXNE); 
    
} 

///*RE��GPIOC5�ܽ�����*/   
//#define Set_RE  GPIO_SetBits(GPIOC,GPIO_Pin_5);			 
//#define Clr_RE  GPIO_ResetBits(GPIOC,GPIO_Pin_5);
///*DE��GPIOC4�ܽ�����*/
//#define Set_DE  GPIO_SetBits(GPIOC,GPIO_Pin_4);
//#define Clr_DE  GPIO_ResetBits(GPIOC,GPIO_Pin_4);


//void display_init(u32 bound){
//  //GPIO�˿�����
//	
//    
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	 
//	/* ʹ�� USART3 ʱ��*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ������ʱ��	 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	/* USART3 ʹ��IO�˿����� */    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);    
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
//  GPIO_Init(GPIOB, &GPIO_InitStructure);   //��ʼ��GPIOA
//	

//  //Usart3 NVIC ����
//  /* Configure the NVIC Preemption Priority Bits */  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
// 
//  /* Enable the USART3 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	
//	/* USART3 ����ģʽ���� */
//	USART_InitStructure.USART_BaudRate = bound;	//���������ã�9600
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
//	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
//	USART_Init(USART3, &USART_InitStructure);  //��ʼ��USART3
//	
//  /*ʹ�ܴ���3�ķ��ͺͽ����ж�*/
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	USART_Cmd(USART3, ENABLE);// USART3ʹ��
//	USART_ClearFlag(USART3,USART_FLAG_TC);
//	
//}
// /*����һ���ֽ�����*/
// void display_sendbyte(unsigned char SendData)
//{	   
//	Set_DE;
//	Set_RE;
//	delay_ms(1);
//	USART_SendData(USART3,SendData);
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
//	delay_ms(1);
//	Clr_DE;
//	Clr_RE; 	    
//} 
//extern void display_senddata(u8 *buf,u16 len)
//{
//	u16 t;
//	Set_DE;
//	Set_RE;
//	delay_ms(1);
//	for(t=0;t<len;t++)     
//	{           
//			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
//			USART_SendData(USART3,buf[t]); 
//	}     
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
//	
//	delay_ms(1);
//	Clr_DE;
//	Clr_RE; 
//}
