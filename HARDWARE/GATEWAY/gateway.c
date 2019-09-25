#include "gateway.h"

void gateway_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
      
    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX      GPIOA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart1 NVIC 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //  0-3;
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          
    NVIC_Init(&NVIC_InitStructure);  
  
   //USART ≈‰÷√

    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 

    USART_Init(USART1, &USART_InitStructure); 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);                
}

void gateway_send_Data(u8 *buf,u16 len)
{
    u16 t;
    for(t=0;t<len;t++)    
    {           
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //????,??????   
        USART_SendData(USART1,buf[t]); 
    }     
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);        
}


void USART1_IRQHandler(void)                
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
			Res =USART_ReceiveData(USART1);            
	}
		
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
	{
		USART_ReceiveData(USART1);
		USART_ClearFlag(USART1,USART_FLAG_ORE);
	}
	USART_ClearFlag(USART1,USART_IT_RXNE);
}


