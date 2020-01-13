#include "discern.h"
#include "delay.h"
#include "lcd.h"
#include "ov7670.h" 

#include "algorithm.h"


void CameraScan(void) {                                               //摄像头扫描测试
	vu16 a = 0,b = 0;
	
	for(a = 0; a < 240; a++) {                                          //各行跳变点计数，数组清零
		TableChangePoint_240[a] = 0;
	}
	Min_blue = 320;                                                     //初始化记录蓝色车牌区域的值
	Max_blue = 0;
	
	LCD_SetWindows(0,0,320,240);                                        //设置显示窗口
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);                           //标志：数据写入

	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 0);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 1);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==0);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 0);
	
	FIFO_Reset_Read_Addr(); 
 
	
	for(a = 0; a < 240; a++) {
		for(b = 0; b < 320; b++) {
			GPIOC->BRR = 1<<4;
			AA = GPIOA->IDR;						
			GPIOC->BSRR = 1<<4;
			
			GPIOC->BRR = 1<<4;
			BB = GPIOA->IDR&0x00ff;								
			GPIOC->BSRR = 1<<4;	
			
			color = (AA<<8)| BB;

			R = color>>11;
			G = (color>>5)&0x3f;
			B = color&0x1f;
			
			if((R > Red_Vlaue) && (G >= Green_Value) && (B >= Blue_Value)) {                             //二值化,高阈值：25.55.25，较合适阈值（21,47,21）
				color = 0xffff;
			} else {
				color = 0x0000;
			}
			
			if(color != color_save) {                                              //跳变点
				TableChangePoint_240[a]++;		                                       //该行跳变点计数+1
			}
			color_save = color;                                                    //保存像素值，供下一次判断和比较
			
			color = (AA<<8) | BB;                                                  //还原色彩
					
			LCD_DATA_PORT->ODR = color;
			GPIOC->BRR = 1<<11;
			GPIOC->BSRR = 1<<11;
		}
	}
	ChangePoint_Show_240();                                                     //240方向跳变点显示
	ChangePoint_Analysis_240();	                                                //跳变点分析  获得车牌高度
}

void CameraDiscern(void) {                                                    //摄像头扫描
	vu8 i = 0;

	CameraScan();
	//返回flag_MaxMinCompare，Min_ChangePoint_240，Max_ChangePoint_240
	if(flag_MaxMinCompare == 1) {  
		//跳变点筛选成功  即高度合理
//			BEEP_ON;        
//			delayms(500);
//			BEEP_OFF;
		
		//分析显示车牌左右边界
		 ChangePoint_Analysis_Blue();                                             //320蓝色区域分析,采用读取像素，得结果Min_blue,Max_blue  分析车牌左右边界
		if(Min_blue > Max_blue) {
			flag_MaxMinCompare=0;                                                   //进行合理性判断1
		}
			
		if((Min_blue > 290) || (Max_blue > 290)) {
			flag_MaxMinCompare = 0;                                                  //进行合理性判断2
		}	
	}
	if(flag_MaxMinCompare == 1) {                                                //跳变点筛选成功  左右边界获取成功 并且合理
		ChangePoint_Analysis_320();                                                //蓝色区域中，320跳变点分析,获得：TableChangePoint_320[b]结果 左右边界内二值化
		ChangePoint_Show_320();                                                    //320方向跳变点显示
		i = SegmentationChar(); 
		
		if(i == 8) {                                                               //字符分割,返回分割的字符个数，用于判断合法性
			CharacterRecognition();                                                  //字符识别	
		} else {
			LCD_Fill(0x6666);                                                        //黑屏，显示Measure Faill
			LCD_ShowChar(8*1,200,'M',0);
			LCD_ShowChar(8*2,200,'e',0);
			LCD_ShowChar(8*3,200,'a',0);
			LCD_ShowChar(8*4,200,'s',0);
			LCD_ShowChar(8*5,200,'u',0);
			LCD_ShowChar(8*6,200,'r',0);
			LCD_ShowChar(8*7,200,'e',0);
			
			LCD_ShowChar(8*9,200,'F',0);
			LCD_ShowChar(8*10,200,'a',0);
			LCD_ShowChar(8*11,200,'i',0);
			LCD_ShowChar(8*12,200,'l',0);
			LCD_ShowChar(8*13,200,'l',0);

			delay_ms(800);
		}
	}
}

