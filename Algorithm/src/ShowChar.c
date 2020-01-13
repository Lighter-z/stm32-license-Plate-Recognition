#include "ShowChar.h"
#include "lcd.h"
#include "ov7670.h"
#include "string.h" 


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

vu8 tim3_num = 0;                                                     //TIM3分钟计时

void Data_LCD_Display(void) {																					//常规显示
	vu16 a = 0,b = 0;
	LCD_SetWindows(0,0,320,240);																				//设置显示窗口  
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);														//标志：数据写入

	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 0);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 1);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==0);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 0);
	
	FIFO_Reset_Read_Addr(); 
 
	for (a = 0; a < 240; a++) {
		for(b = 0; b < 320; b++) {
			GPIOC->BRR = 1<<4;
			AA = GPIOA->IDR;						
			GPIOC->BSRR = 1<<4;
			
			GPIOC->BRR = 1<<4;
			BB = GPIOA->IDR&0x00ff;								
			GPIOC->BSRR = 1<<4;	
			
			color = (AA<<8)|BB;
			
			LCD_DATA_PORT->ODR = color;

			GPIOC->BRR = 1<<11;
			GPIOC->BSRR = 1<<11;
		}							
	}	 
}

void Show_Title(void) {																								 //显示标题
	LCD_ShowChar(35+8*0,10,'N',0);
	LCD_ShowChar(35+8*1,10,'u',0);
	LCD_ShowChar(35+8*2,10,'m',0);
	LCD_ShowChar(35+8*3,10,'b',0);
	LCD_ShowChar(35+8*4,10,'e',0);
	LCD_ShowChar(35+8*5,10,'r',0);
	
	LCD_ShowChar(35+8*9,10,'T',0);
	LCD_ShowChar(35+8*10,10,'i',0);
	LCD_ShowChar(35+8*11,10,'m',0);
	LCD_ShowChar(35+8*12,10,'e',0);

	
	LCD_ShowChar(35+8*17,10,'P',0);
	LCD_ShowChar(35+8*18,10,'r',0);
	LCD_ShowChar(35+8*19,10,'i',0);
	LCD_ShowChar(35+8*20,10,'c',0);
	LCD_ShowChar(35+8*21,10,'e',0);
}


void WordShow(vu8 num,vu16 x,vu16 y) {																 //显示汉字16*16
	vu16 a = 0,b = 0,e = 0,num1 = 0;
	vu8 table1[32] = {0};
	if(num == 1) {
		for(a = 0; a< 32; a++) {
			table1[a]= table_yu[a];	
		}		
	}else if(num == 2) {
		for(a = 0; a < 32; a++) {
			table1[a] = table_min[a];	
		}		
	}else if(num == 3) {
		for(a = 0; a < 32; a++) {
			table1[a] = table_lu[a];	
		}		
	}else if(num == 4) {
		for(a = 0; a < 32; a++) {
			table1[a] = table_zhe[a];	
		}		
	}else if(num==5) {
		for(a = 0; a < 32; a++) {
			table1[a] = table_shan[a];	
		}		
	}else if(num == 6) {
		for(a = 0; a < 32; a++) {
			table1[a] = table_cuan[a];	
		}		
	}
	for(a = 0; a < 16; a++) {
		for(b = 0; b < 16; b++) {
			if(table1[b/8+a*2]&(1<<(7-b%8))) {
				num1=0xffff;
			} else {
				num1=0x0000;
			}
			LCD_DrawPoint(b+x,a+y,num1);                                         //画点
		}				
	}	
}

void PictureToString(void) {                                              //图片->数组table_picture   150字节   
	vu16 a = 0,b = 0,num1 = 0;
	
	for(a = 0; a < 150; a++) {                                              //归零
		table_picture[a] = 0x00;	
	}	
	for(a = 0; a< 50; a++) {                                                //50行
		for(b = 0; b < 24; b++) {                                             //24排
			num1 = LCD_ReadPoint(b+296,a+191);
			if(num1 == 0xffff) {
				table_picture[b/8+a*3]|=(1<<(7-b%8));                             //(1<<(7-b%8))   将"1" 变换在单字节中的相应位置上。  
			}
		}				
	}
}

void StringToPicture(void) {                                              //数组->图片
	vu16 a = 0,b = 0,e = 0,num1 = 0;
	
	for(a = 0; a < 50; a++) {                                               //50排                              
		for(b = 0;b < 24; b++) {                                              //24行
			if(talble_0[b/8+a*3]&(1<<(7-b%8))) {
				num1=0xffff;
			} else {
				num1=0x0000;
			}
			LCD_DrawPoint(b+296,a+191,num1);                                    //画点
		}				
	}	
}

void Show_Card(vu8 i) {                                                   //显示第几组车牌
	vu16 t0=0;
	//显示汉字
	if(table_card[i][0] != 0) {
		WordShow(table_card[i][0],9,i*16+50);//
	}
	
	if(table_card[i][1] < 10) {
		LCD_ShowNum(25,i*16+50,table_char[table_card[i][1]],1);
	} else {
		LCD_ShowChar(25,i*16+50,table_char[table_card[i][1]],0);
	}
	
	LCD_ShowChar(33,i*16+50,'.',0);													                	//点
	
	if(table_card[i][2] < 10) {
		LCD_ShowNum(41,i*16+50,table_char[table_card[i][2]],1);
	} else {
		LCD_ShowChar(41,i*16+50,table_char[table_card[i][2]],0);
	}
	
	if(table_card[i][3] < 10) {
		LCD_ShowNum(49,i*16+50,table_char[table_card[i][3]],1);
	} else {
		LCD_ShowChar(49,i*16+50,table_char[table_card[i][3]],0);
	}
	
	if(table_card[i][4] < 10) {
		LCD_ShowNum(57,i*16+50,table_char[table_card[i][4]],1);
	} else {
		LCD_ShowChar(57,i*16+50,table_char[table_card[i][4]],0);
	}
	
	if(table_card[i][5] < 10) {
		LCD_ShowNum(65,i*16+50,table_char[table_card[i][5]],1);
	} else {
		LCD_ShowChar(65,i*16+50,table_char[table_card[i][5]],0);
	}
	
	if(table_card[i][6] < 10) {
		LCD_ShowNum(73,i*16+50,table_char[table_card[i][6]],1);
	} else {
		LCD_ShowChar(73,i*16+50,table_char[table_card[i][6]],0);
	}
	
	t0 = table_card[i][7];
	LCD_ShowNum(100,i*16+50,t0,6);                                            //显示时间

	if(t0 < 60) {
		LCD_ShowNumPoint(168,i*16+50,t0*8);
	} else {
		LCD_ShowNumPoint(168,i*16+50,t0/60*500+t0%60*8);
	}
	
}

//定时器3中断服务程序	 
void TIM3_IRQHandler(void) { 		    		  			    
	if(TIM3->SR&0X0001) {                                                    //溢出中断
		if(tim3_num == 60) {
			if(table_card[0][0] != 0) {                                          //第1组计时
				table_card[0][7]++;
			}	
			
			if(table_card[1][0] != 0) {                                          //第2组计时
				table_card[1][7]++;
			}
			
			if(table_card[2][0] != 0) {                                          //第3组计时
				table_card[2][7]++;
			}	
			
			if(table_card[3][0] != 0) {                                          //第4组计时
				table_card[3][7]++;
			}	
			
			if(table_card[4][0] != 0) {                                          //第5组计时
				table_card[4][7]++;
			}
			tim3_num = 0;
		}		
	}			
	tim3_num++;
	TIM3->SR&=~(1<<0);                                                        //清除中断标志位 	    
}

void TIM3_Configuration(void) {
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* ---------------------------------------------------------------
	TIM3CLK 即PCLK1=36MHz
	TIM3CLK = 36 MHz, Prescaler = 7200, TIM3 counter clock = 5K,即改变一次为5K,周期就为10K
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2000;                                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到10000为1000ms
	TIM_TimeBaseStructure.TIM_Prescaler = (12800-1);                             //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                 //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                              //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* Enables the Update event for TIM3 */
	//TIM_UpdateDisableConfig(TIM3,ENABLE); 	//使能 TIM3 更新事件 
	
	/* TIM IT enable */
	TIM_ITConfig(               //使能或者失能指定的TIM中断
		TIM3,                     //TIM3
		TIM_IT_Update  |          //TIM 中断源
		TIM_IT_Trigger,           //TIM 触发中断源 
		ENABLE                    //使能
		);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);    //使能TIMx外设
}






