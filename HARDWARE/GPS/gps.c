#include "gps.h"
#include "delay.h"
#include "comman.h"

const int gps_datalen =100;
u8 g_gps_buf[gps_datalen];


char weidu[20] = {0x00};
char weidu_counter = 0;
char jingdu[20] = {0x00};
char jingdu_counter = 0;
char haiba[20] = {0x00};
char haiba_counter = 0;
char shijian[20] = {0x00};
char shijian_counter = 0;
char *ggaString = "$GNGGA,000001,3112.518576,N,12127.901251,E,1,8,1,0,M,-32,M,3,0*4B\r\n\r\n"; 

static BOOL qxwz_tranFlag = FALSE;  		//是否转换为字符串的标志 
static char qxwz_RxCounter = 0;
static char qxwz_RxBuffer[USART_REC_LEN] = {0x00};      	//接收缓冲,最大BUF_MAX个字节
static char index = 0;

void gps_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
      
    //USART1_TX   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Usart1 NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);   
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     
    NVIC_Init(&NVIC_InitStructure);  
  
   //USART
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);                   
}
void gps_senddata(u8 *buf,u16 len)
{
		u16 t;
    for(t=0;t<len;t++)     
    {           
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
        USART_SendData(USART1,buf[t]); 
    }     
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);        
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);  	
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
	u8 gps_head[6] = {'$','G','P','G','G','A'};
	static u8 recTemp[6] = {0x00,0x00,0x00,0x00,0x00,0x00};  //接收缓存
	static u8 i = 0;
	static BOOL checkFlag = FALSE;
	static BOOL recFlag   = FALSE;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
			USART_ClearITPendingBit(USART1, USART_IT_RXNE); //清空中断标志位
			Res =USART_ReceiveData(USART1);									//读取接收到的数据
	
	
		if(qxwz_tranFlag == FALSE)
		{
				if(checkFlag == FALSE){
		if(Res == '$'){
			recTemp[i++] = Res;
		}else if( (recTemp[0] == '$') && (i != 0) ){
			recTemp[i++] = Res; 
			if(i == 6) //当成功接收到了以'$'开头的6个字符时，标志位变为TRUE，进行判断
			{
				if(checkArray(recTemp,data1) == 1 ){
					recFlag = TRUE;
					checkFlag = TRUE;
					for(i = 0;i < 5;i++)
					{
						qxwz_RxBuffer[qxwz_RxCounter++] = recTemp[i];
					}
				}else{
					checkFlag = FALSE;
				}
				i = 0; //清空缓存
			}
		}
	}
	if(recFlag == TRUE){
							if(Res != 0x0D)
							{
								qxwz_RxBuffer[qxwz_RxCounter++] = Res;
								if(Res == ',')
								{
									index++;
								}
								else
								switch(index)
								{
									case 1: 
										shijian[shijian_counter++] = Res;
									break;
									case 2: 
									case 3: 
										weidu[weidu_counter++] = Res;
									break;
									case 4: 
									case 5: 
										jingdu[jingdu_counter++] = Res;
									break;
									case 9:  
										haiba[haiba_counter++] = Res;
									break;
								}
							}else{
								qxwz_RxBuffer[qxwz_RxCounter] 		= 0x0D;
								qxwz_RxBuffer[qxwz_RxCounter + 1] = 0x0A;
					//			qxwz_RxBuffer[qxwz_RxCounter + 2] = 0x0D;
					//			qxwz_RxBuffer[qxwz_RxCounter + 3] = 0x0A;
								weidu_counter = 0;
								jingdu_counter = 0;
								haiba_counter = 0;
								shijian_counter = 0;
								qxwz_RxCounter = 0X00;
								recFlag = FALSE;
								checkFlag = FALSE;
								qxwz_tranFlag = TRUE;  //一帧数据接收完毕，可以转换为字符串
								index = 0;
							}
						}
			}else{
				qxwz_tranFlag = FALSE;
				ggaString = qxwz_RxBuffer; //确保每次更新都为一帧完整数据
				
		}   		 
   } 
} 

/*void USART1_IRQHandler(void)                   
{
    u8 Res;
		u8 USART1_RX_BUF[gps_datalen]; 
		u8 USART1_RX_CNT=0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    {
				Res =USART_ReceiveData(USART1);      
				if(USART1_RX_CNT<gps_datalen)
				{
						USART1_RX_BUF[USART1_RX_CNT]=Res;         
						USART1_RX_CNT++;                                     
				}
				else
				{
					my_strcpy(g_gps_buf,USART1_RX_BUF,gps_datalen);
					USART1_RX_CNT = 0;
				}
				
				
				
				
				
				
				
    }
    // 溢出-如果发生溢出需要先读SR,再度DR寄存器则可清除不断入中断的问题
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
    {
        USART_ReceiveData(USART1);
        USART_ClearFlag(USART1,USART_FLAG_ORE);
    }
    USART_ClearFlag(USART1,USART_IT_RXNE); 
}
*/

