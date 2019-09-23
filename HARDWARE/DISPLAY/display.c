#include "display.h"
#include "delay.h"

/*RE跟GPIOC3管脚相连*/   
#define Set_RE_display  GPIO_SetBits(GPIOC,GPIO_Pin_3);			 
#define Clr_RE_display  GPIO_ResetBits(GPIOC,GPIO_Pin_3);
/*DE跟GPIOC2管脚相连*/
#define Set_DE_display  GPIO_SetBits(GPIOC,GPIO_Pin_2);
#define Clr_DE_display  GPIO_ResetBits(GPIOC,GPIO_Pin_2);


void display_init(u32 bound){
  //GPIO端口设置
	
    
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	/* 使能 USART4 时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能外设时钟	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* USART4 使用IO端口配置 */    //tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);    
                             
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;  //rx
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);   //初始化GPIOA
	

  //Usart4 NVIC 配置
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  /* Enable the USART4 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* USART4 工作模式配置 */
	USART_InitStructure.USART_BaudRate = bound;	//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(UART4, &USART_InitStructure);  //初始化USART4
	
  /*使能串口4的发送和接收中断*/
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
	USART_Cmd(UART4, ENABLE);// USART4使能
	USART_ClearFlag(UART4,USART_FLAG_TC);
	
}
 /*发送一个字节数据*/
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
    
	// 溢出-如果发生溢出需要先读SR,再度DR寄存器则可清除不断入中断的问题
	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE) == SET)
	{
			USART_ReceiveData(UART4);
			USART_ClearFlag(UART4,USART_FLAG_ORE);
	}		
	USART_ClearFlag(UART4,USART_IT_RXNE); 
    
} 

///*RE跟GPIOC5管脚相连*/   
//#define Set_RE  GPIO_SetBits(GPIOC,GPIO_Pin_5);			 
//#define Clr_RE  GPIO_ResetBits(GPIOC,GPIO_Pin_5);
///*DE跟GPIOC4管脚相连*/
//#define Set_DE  GPIO_SetBits(GPIOC,GPIO_Pin_4);
//#define Clr_DE  GPIO_ResetBits(GPIOC,GPIO_Pin_4);


//void display_init(u32 bound){
//  //GPIO端口设置
//	
//    
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	 
//	/* 使能 USART3 时钟*/
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能外设时钟	 
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	
//	/* USART3 使用IO端口配置 */    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);    
//  
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
//  GPIO_Init(GPIOB, &GPIO_InitStructure);   //初始化GPIOA
//	

//  //Usart3 NVIC 配置
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
//	/* USART3 工作模式配置 */
//	USART_InitStructure.USART_BaudRate = bound;	//波特率设置：9600
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
//	USART_Init(USART3, &USART_InitStructure);  //初始化USART3
//	
//  /*使能串口3的发送和接收中断*/
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	USART_Cmd(USART3, ENABLE);// USART3使能
//	USART_ClearFlag(USART3,USART_FLAG_TC);
//	
//}
// /*发送一个字节数据*/
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
