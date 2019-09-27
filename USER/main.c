#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	 
#include "stmflash.h"
#include "myiic.h"
#include "display.h"
#include "comman.h"
#include "gps.h"
#include "sensor.h"
#include "gateway.h"
#include "battery.h"
#include "stm32f10x_iwdg.h"


unsigned short crc_result;
u8 g_qianwei,g_baiwei,g_shiwei,g_gewei;

int g_crc_sum;
u8 g_crc_value[4];
int g_crc_conut;

int g_ad_value = 0;

/*sensor---USART3
	gps---USART2
	gate---USART1
	display---USART4
*/
void IWDG_Configuration(void);
	
int main(void)
{	
	u8 i=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	IWDG_Configuration();
	gps_init(4800);
	sensor_init(4800);
	display_init(57600);
	gateway_Init(9600);
	battery_init();
	display_senddata(led_init_cmd,30);  //清除 led 静态文字
	delay_ms(1000);
	delay_ms(1000);	
	delay_ms(1000);
	delay_ms(1000);	
	delay_ms(1000);	
//while(1);
	while(1)
	{
		//1、查询传感器值
		/*
		sensor_senddata(wind_direction_cmd,8);
		delay_ms(1000);
		sensor_senddata(wind_speed_cmd,8);
		delay_ms(1000);
		sensor_senddata(box_cmd,8);
		delay_ms(1000);
		*/
		
		//2、发送数据到led
		sensor_senddata(box_cmd,8); //查询温湿度
		delay_ms(1000);
		delay_ms(1000);
		inttohex_four(g_box_temp,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //temp
		led_temp_humidity_cmd[69] = g_gewei;
		led_temp_humidity_cmd[67] = g_shiwei;
		led_temp_humidity_cmd[66] = g_baiwei;
		led_temp_humidity_cmd[65] = g_qianwei;
		
		inttohex_four(g_box_humidity,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //humidity
		led_temp_humidity_cmd[85] = g_gewei;
		led_temp_humidity_cmd[83] = g_shiwei;
		led_temp_humidity_cmd[82] = g_baiwei;
		led_temp_humidity_cmd[81] = g_qianwei;
		
		crc_result=my_CRC(led_temp_humidity_cmd,90);
		//led_temp_humidity_cmd[90] = crc_result &0x00ff;
		//led_temp_humidity_cmd[91] = crc_result>>8 &0x00ff;
		
		g_crc_sum = 90;
		g_crc_conut =escape_processing(crc_result,g_crc_value);
		for(i=0;i<g_crc_conut;i++)
		{
			led_temp_humidity_cmd[g_crc_sum] = g_crc_value[i];
			g_crc_sum++;
		}
		led_temp_humidity_cmd[g_crc_sum] = 0x5A;
		g_crc_sum++;
		display_senddata(led_temp_humidity_cmd,g_crc_sum);

		delay_ms(1000);
		IWDG_ReloadCounter();
		
		sensor_senddata(box_cmd,8);  //查询PM2.5 PM10
		delay_ms(1000);
		delay_ms(1000);
		inttohex_three(g_box_pm25,&g_baiwei,&g_shiwei,&g_gewei);  //PM25
		led_PM25_PM10_cmd[67] = g_gewei;
		led_PM25_PM10_cmd[66] = g_shiwei;
		led_PM25_PM10_cmd[65] = g_baiwei;
		
		inttohex_three(g_box_pm10,&g_baiwei,&g_shiwei,&g_gewei);  //PM10
		led_PM25_PM10_cmd[84] = g_gewei;
		led_PM25_PM10_cmd[83] = g_shiwei;
		led_PM25_PM10_cmd[82] = g_baiwei;
		
		
		crc_result=my_CRC(led_PM25_PM10_cmd,92);
		g_crc_sum = 92;
		g_crc_conut =escape_processing(crc_result,g_crc_value);
		for(i=0;i<g_crc_conut;i++)
		{
			led_PM25_PM10_cmd[g_crc_sum] = g_crc_value[i];
			g_crc_sum++;
		}
		//led_PM25_PM10_cmd[92] = crc_result &0x00ff;
		
		//led_PM25_PM10_cmd[93] = crc_result>>8 &0x00ff;
		//led_PM25_PM10_cmd[94] = 0x5A;
		led_PM25_PM10_cmd[g_crc_sum] = 0x5A;
		g_crc_sum++;
		display_senddata(led_PM25_PM10_cmd,g_crc_sum);

		IWDG_ReloadCounter();
		sensor_senddata(wind_direction_cmd,8);  //查询风向
		delay_ms(1000);
		sensor_senddata(wind_speed_cmd,8); //查询风速
		delay_ms(1000);
		delay_ms(1000);
		inttohex_four(g_wind_speed,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //speed
		led_speed_direction_cmd[68] = g_gewei;
		led_speed_direction_cmd[66] = g_shiwei;
		led_speed_direction_cmd[65] = g_baiwei;
		led_speed_direction_cmd[64] = g_qianwei;
		
		inttohex_four(g_wind_direction,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //direction
		led_speed_direction_cmd[84] = g_gewei;
		led_speed_direction_cmd[83] = g_shiwei;
		led_speed_direction_cmd[82] = g_baiwei;
		led_speed_direction_cmd[81] = g_qianwei;
		
		crc_result=my_CRC(led_speed_direction_cmd,90);
		//led_temp_humidity_cmd[90] = crc_result &0x00ff;
	//	led_temp_humidity_cmd[91] = crc_result>>8 &0x00ff;
		g_crc_sum = 90;
		g_crc_conut =escape_processing(crc_result,g_crc_value);
		for(i=0;i<g_crc_conut;i++)
		{
			led_speed_direction_cmd[g_crc_sum] = g_crc_value[i];
			g_crc_sum++;
		}
		led_speed_direction_cmd[g_crc_sum] = 0x5A;
		g_crc_sum++;
		display_senddata(led_speed_direction_cmd,g_crc_sum);

		delay_ms(1000);
		IWDG_ReloadCounter();
		sensor_senddata(box_cmd,8);  //查询噪声 大气压
		delay_ms(1000);
		delay_ms(1000);
		inttohex_four(g_box_noise,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //noise
		led_noise_atmosphere_cmd[69] = g_gewei;
		led_noise_atmosphere_cmd[67] = g_shiwei;
		led_noise_atmosphere_cmd[66] = g_baiwei;
		led_noise_atmosphere_cmd[65] = g_qianwei;
		
		inttohex_four(g_box_atmosphere,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //atmosphere
		led_noise_atmosphere_cmd[86] = g_gewei;
		led_noise_atmosphere_cmd[84] = g_shiwei;
		led_noise_atmosphere_cmd[83] = g_baiwei;
		led_noise_atmosphere_cmd[82] = g_qianwei;
		
		crc_result=my_CRC(led_noise_atmosphere_cmd,90);
		g_crc_sum = 90;
		g_crc_conut =escape_processing(crc_result,g_crc_value);
		for(i=0;i<g_crc_conut;i++)
		{
			led_noise_atmosphere_cmd[g_crc_sum] = g_crc_value[i];
			g_crc_sum++;
		}
		led_noise_atmosphere_cmd[g_crc_sum] = 0x5A;
		g_crc_sum++;
		display_senddata(led_noise_atmosphere_cmd,g_crc_sum);
		

		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		IWDG_ReloadCounter();
		
		
		//inttohex_four(hfh,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //jingdu
		led_jingdu_weidu_cmd[70] = jingdu[4];
		led_jingdu_weidu_cmd[69] = jingdu[3];
		led_jingdu_weidu_cmd[67] = jingdu[2];
		led_jingdu_weidu_cmd[66] = jingdu[1];
		led_jingdu_weidu_cmd[65] = jingdu[0];
		
		//inttohex_four(hfh,&g_qianwei,&g_baiwei,&g_shiwei,&g_gewei);  //weidu
		led_jingdu_weidu_cmd[85] = weidu[3];
		led_jingdu_weidu_cmd[84] = weidu[2];
		led_jingdu_weidu_cmd[82] = weidu[1];
		led_jingdu_weidu_cmd[81] = weidu[0];
		
		crc_result=my_CRC(led_jingdu_weidu_cmd,90);
	//	led_jingdu_weidu_cmd[90] = crc_result &0x00ff;
	//	led_jingdu_weidu_cmd[91] = crc_result>>8 &0x00ff;
		g_crc_sum = 90;
		g_crc_conut =escape_processing(crc_result,g_crc_value);
		for(i=0;i<g_crc_conut;i++)
		{
			led_jingdu_weidu_cmd[g_crc_sum] = g_crc_value[i];
			g_crc_sum++;
		}
		led_jingdu_weidu_cmd[g_crc_sum] = 0x5A;
		g_crc_sum++;
		display_senddata(led_jingdu_weidu_cmd,g_crc_sum);

		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		IWDG_ReloadCounter();
	
		g_ad_value = battery_read();
		inttohex_three(g_ad_value,&g_baiwei,&g_shiwei,&g_gewei);  //battery
		led_battery_cmd[71] = g_gewei;
		led_battery_cmd[70] = g_shiwei;
		led_battery_cmd[69] = g_baiwei;
		
		crc_result=my_CRC(led_battery_cmd,73);
		g_crc_sum = 73;
		g_crc_conut =escape_processing(crc_result,g_crc_value);
		for(i=0;i<g_crc_conut;i++)
		{
			led_battery_cmd[g_crc_sum] = g_crc_value[i];
			g_crc_sum++;
		}
		led_battery_cmd[g_crc_sum] = 0x5A;
		g_crc_sum++;
		display_senddata(led_battery_cmd,g_crc_sum);
		
	
		
		delay_ms(1000);
		IWDG_ReloadCounter();
		
		//3、推送数据到网关
		gateway_send_Data(gate_init_cmd,10);
		
		
		
		
		
		USART3_RX_CNT = 0;  //一轮循环后清零，防止错误状态出不起来
	}
}
void IWDG_Configuration(void) 
{
    //使能寄存器 写功能
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
     /* 设置预分频 ,40K/256=156HZ(6.4ms)*/  
    IWDG_SetPrescaler(IWDG_Prescaler_256);    /*喂狗时间 20S/6.4MS=3125 注意不能大于 xfff*/  
    IWDG_SetReload(3125);// 20s时间
    IWDG_ReloadCounter(); //喂狗
		IWDG_Enable(); //使能独立看门狗
}




//#include "usart.h"	
//	__set_PRIMASK(1);  // 关闭总中断
//	__set_PRIMASK(0);    //打开总中断
	//uart_init(9600);	 //串口初始化为9600
	//printf("Let's begin\r\n");


//	crc_result=my_CRC(led_init_cmd,27);
//	
//	uuuu=led_humidity_temp_cmd[69];
//	uuuu=led_humidity_temp_cmd[85];
//	

//	//		//1、查询传感器值
//		sensor_senddata(wind_direction_cmd,8);
//		delay_ms(1000);
//		sensor_senddata(wind_speed_cmd,8);
//		delay_ms(1000);
//		sensor_senddata(box_cmd,8);
//		delay_ms(1000);





