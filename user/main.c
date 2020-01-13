#include "stm32f10x.h"
#include "stm32f10x_it.h" 

#include "key.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "ov7670.h"
#include "rcc.h"
#include "ShowChar.h"
#include "discern.h"

extern vu8 Red_Vlaue, Green_Value ,Blue_Value;//阈值


int main(void)
{  
	unsigned int scan_time = 0;
 
	STM32_Clock_Init(16);                                    //初始化时钟

	LCD_Init();	
	Key_Init();	                                             //初始化 KEY1 PA8
	OV7670_GPIO_Init();                                      //OV7670引脚初始化，放在串口初始化前面
	//USART1_init();                                         //初始化串口	

	TIM3_Configuration();                                    //10Khz的计数频率，计数到5000为500ms  
	LCD_Fill(0x6666);		
	while(!Sensor_Init());
	
	LCD_Fill(0xF800);
	delayms(100);

	scan_time = 2;
	
  //二值化阈值	
	Red_Vlaue = 24;
	Green_Value = 53;
	Blue_Value = 24;

	while(1)
	{
		if(scan_time <= 1) {
			CameraDiscern();                                    //车牌识别
		}
		if(scan_time > 1) {
			CameraScan();                                       //摄像头扫描测试
			LCD_ShowNum(30,220,21 - scan_time, 2); 
			while(GPIO_ReadInputDataBit(KEY1_PORT,KEY1_PIN)==0) //按键按下
			{
				LCD_Fill(0x00);                                   //黑屏
				Show_Title();                                     //显示标题
				Show_Card(0);                                     //显示第几组车牌
				Show_Card(1);
				Show_Card(2);
				Show_Card(3);
				Show_Card(4);
				delay_ms(5000);
			}
		}
		if(scan_time == 20) {
			 scan_time = 0;
		}
		scan_time++;
	} 
}


