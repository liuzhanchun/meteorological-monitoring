#include "myiic.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define MY_I2C_ADDRESS    0x3c 

u8 compass_data[0xff];
//初始化IIC
void I2C1_Init(void)
{
	int i=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	// enable APB1 peripheral clock for I2C1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	// enable clock for SCL and SDA pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* setup SCL and SDA pins
	 * You can connect I2C1 to two different
	 * pairs of pins:
	 * 1. SCL on PB6 and SDA on PB7 
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出，实现线与逻辑
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// configure I2C1 
	I2C_InitStructure.I2C_ClockSpeed = 90000;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = MY_I2C_ADDRESS;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress= I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);

		//setup interrupts
	I2C_ITConfig(I2C1, I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF, ENABLE);

	
	// Configure the I2C event priority
	NVIC_InitStructure.NVIC_IRQChannel                   = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// enable I2C1
	I2C_Cmd(I2C1, ENABLE);
	
	for(i=0;i<0xff;i++)
	{
		compass_data[i]=i;
	}
	
}
//Clear ADDR by reading SR1, then SR2
void I2C_clear_ADDR(I2C_TypeDef* I2Cx) {
	I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR);
	((void)(I2Cx->SR2));
}

//Clear STOPF by reading SR1, then writing CR1
void I2C_clear_STOPF(I2C_TypeDef* I2Cx) {
	I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF);
	I2C_Cmd(I2Cx, ENABLE);
}
uint8_t data = 0;
void I2C1_EV_IRQHandler(void) {
//        GPIO_SetBits(GPIOD, GREEN); //Show that we got here
	
		//printf("I2C1_EV_IRQHandler\r\n");
	
		KV1=0;
		//Clear AF from slave-transmission end
		if(I2C_GetITStatus(I2C1, I2C_IT_AF)) {
			I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
		}
		//Big state machine response, since doesn't actually keep state
		switch(I2C_GetLastEvent(I2C1)) {
			//SLAVE
			//Receive
			case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED: //EV1
				I2C_clear_ADDR(I2C1);
				break;
			case I2C_EVENT_SLAVE_BYTE_RECEIVED: //EV2
				//Read it, so no one is waiting, clears BTF if necessary
				data = I2C_ReceiveData(I2C1);
				//Do something with it
				if(data==0xa0)
				{
					data =0;
				}
				if(I2C_GetFlagStatus(I2C1, I2C_FLAG_DUALF)) {//Secondary Receive
				} else if(I2C_GetFlagStatus(I2C1, I2C_FLAG_GENCALL)) {//General Receive
				} else {//Normal
				}
				break;
			case I2C_EVENT_SLAVE_STOP_DETECTED: //End of receive, EV4
				I2C_clear_STOPF(I2C1);
				break;

			//Transmit
			case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED: //EV1
				I2C_clear_ADDR(I2C1);
				//Send first byte
			
			
				I2C_SendData(I2C1, compass_data[data]);
				data++;
			
				if(data>0xff)
				{
					data =0;
				}
				break;
			case I2C_EVENT_SLAVE_BYTE_TRANSMITTED: //EV3
				//Determine what you want to send
				//data = 5;
				if(I2C_GetFlagStatus(I2C1, I2C_FLAG_DUALF)) {//Secondary Transmit
				} else if(I2C_GetFlagStatus(I2C1, I2C_FLAG_GENCALL)) {//General Transmit
				} else {//Normal
				}
				//Read flag and write next byte to clear BTF if present
				I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF);
				I2C_SendData(I2C1, ++data);
				break;
			case I2C_EVENT_SLAVE_ACK_FAILURE://End of transmission EV3_2
				//TODO: Doesn't seem to be getting reached, so just
				//check at top-level
				I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
				break;
			//Alternative Cases for address match
			case I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED: //EV1
				break;
			case I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED: //EV1
				break;
			case I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED: //EV1
				break;


			//MASTER
			case I2C_EVENT_MASTER_MODE_SELECT: //EV5, just sent start bit
				break;
			//Receive
			case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED: //EV6, just sent addr    
				break;
			case I2C_EVENT_MASTER_BYTE_RECEIVED: //EV7
				break;
			//Transmit
			case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: //EV6, just sent addr     
				break;
			case I2C_EVENT_MASTER_BYTE_TRANSMITTING: //EV8, about to send data
				break;
			case I2C_EVENT_MASTER_BYTE_TRANSMITTED: //EV8_2, just sent data
				break;

			//Alternative addressing stuff, not going to worry about
			case I2C_EVENT_MASTER_MODE_ADDRESS10: //EV9
				break;
			default:
				//How the FUCK did you get here?
				//I should probably raise some error, but fuck it,
				//it's late
				break;
		}
}

void I2C1_ER_IRQHandler(void) {
 //       GPIO_SetBits(GPIOD, RED);
//	LED3=0;
		//Can't use nice switch statement, because no fxn available
		if(I2C_GetITStatus(I2C1,        I2C_IT_SMBALERT)) {
		} else if(I2C_GetITStatus(I2C1, I2C_IT_TIMEOUT)) {
		} else if(I2C_GetITStatus(I2C1, I2C_IT_PECERR)) {
		} else if(I2C_GetITStatus(I2C1, I2C_IT_OVR)) {
			//Overrun
			//CLK stretch disabled and receiving
			//DR has not been read, b4 next byte comes in
			//effect: lose byte
			//should:clear RxNE and transmitter should retransmit

			//Underrun
			//CLK stretch disabled and I2C transmitting
			//haven't updated DR since new clock
			//effect: same byte resent
			//should: make sure discarded, and write next
		} else if(I2C_GetITStatus(I2C1, I2C_IT_AF)) {
			//Detected NACK
			//Transmitter must reset com
				//Slave: lines released
				//Master: Stop or repeated Start must must be generated
				//Master = MSL bit
			//Fixup
			I2C_ClearITPendingBit(I2C1, I2C_IT_AF);
		} else if(I2C_GetITStatus(I2C1, I2C_IT_ARLO)) {
			//Arbitration Lost
			//Goes to slave mode, but can't ack slave address in same transfer
			//Can after repeat Start though
		} else if(I2C_GetITStatus(I2C1, I2C_IT_BERR)) {
			//Bus Error
			//In slave mode: data discarded, lines released, acts like restart
			//In master mode: current transmission continues
		}
}























