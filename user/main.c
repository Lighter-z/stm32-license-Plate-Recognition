#include "stm32f10x.h"
#include "stm32f10x_it.h"
//#include "led.h"
#include "key.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "ov7670.h"
#include "string.h"
#include "bsp_esp8266.h"
////
#include "discern.h"

//#define  WIFI  
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
vu8 cur_status=0;
vu8 LED_flag=0;
//vu32 a=0;
//vu32 b=0;
vu16 AA=0,BB=0;
vu16 color=0;
vu16 color_save=0;//保存一个像素的值
vu8 R=0,G=0,B=0;//颜色分量
vu8 TableChangePoint_240[240];//跳变点240个
vu8 Max_ChangePoint_240=0,Min_ChangePoint_240=0,Max_bChangePoint=0,Min_bChangePoint=0;//跳变点纵轴始、末坐标,跳变点横轴始、末坐标
vu8 a_Continue=0,b_Continue=0;//记录纵、横轴突变点的连续性
vu8 flag_aMax=0;//末值更新标志
vu8 Max_aChangePoint_reset=0,Min_aChangePoint_reset=0;//修正后的上下限
vu16 Length_card=0,Width_card=0;//车牌的长和宽
vu8 Max_aChangePoint_reset_1=0,Min_aChangePoint_reset_1=0;//保存上次的数据
vu8 flag_MaxMinCompare=0;//Max_aChangePoint_reset_1和Max_aChangePoint_reset的标志
vu8 TableChangePoint_320[320];//纵向跳变点320个
float V=0.00,S=0.00,H=0.00;//定义HSV值
vu16 Min_blue=0;
vu16 Max_blue=0;//定义车牌蓝色区域的横向最大值和最小值
vu16 k1=0,kk1=0,k2=0,kk2=0,k3=0,kk3=0,k4=0,kk4=0,k5=0,kk5=0,k6=0,kk6=0,k7=0,kk7=0,k8=0,kk8=0;//八个字符边界
extern vu8 Table[6300];//所有字符集 （10+26）*150 = 5400 字节
extern vu8 talble_0[150];//字符3,测试用
extern vu8 table_yu[32];//渝字
extern vu8 table_min[32];//闽字
extern vu8 table_lu[32];//鲁字
extern vu8 table_zhe[32];//浙字
extern vu8 table_shan[32];//陕字
extern vu8 table_cuan[32];//川字
vu8 R_a=0,G_a=0,B_a=0;//阈值

vu8 table_picture[150];//定义保存图片的数组
vu8 table_char[36]={0,1,2,3,4,5,6,7,8,9,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',};
vu8 table_char_char[36]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',};
	vu8 table_card[5][8]={	//保存5个车牌的二维数组
{0,0,0,0,0,0,0,0},		//最后一位保存时间
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
};
vu8 tim3_num=0;//TIM3分钟计时

vu8 table_cardMeasure[7];//测量的车牌结果
void Show_Card(vu8 i);//显示第几组车牌
void Show_Title();//显示标题

void MYRCC_DeInit(void)//复位并配置向量表
{										 
	NVIC_InitTypeDef NVIC_InitStructure;	
	RCC->APB1RSTR = 0x00000000;//复位结束			 
	RCC->APB2RSTR = 0x00000000; 
	  
	RCC->AHBENR = 0x00000014;  //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //关闭所有中断
	/* Enable the TIM3 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}
void Stm32_Clock_Init(vu8 PLL)//系统时钟初始化函数  pll:选择的倍频数，从2开始，最大值为16	
{
	unsigned char temp=0;   
	MYRCC_DeInit();		  //复位并配置向量表
	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON
	while(!(RCC->CR>>17));//等待外部时钟就绪
	RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1;
	PLL-=2;//抵消2个单位
	RCC->CFGR|=PLL<<18;   //设置PLL值 2~16
	RCC->CFGR|=1<<16;	  //PLLSRC ON 
	FLASH->ACR|=0x32;	  //FLASH 2个延时周期

	RCC->CR|=0x01000000;  //PLLON
	while(!(RCC->CR>>25));//等待PLL锁定
	RCC->CFGR|=0x00000002;//PLL作为系统时钟	 
	while(temp!=0x02)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		temp&=0x03;
	}    
}

//void LED()//LED指示灯-提示
//{
//	GPIO_WriteBit(LED1_GPIO_PORT, LED1_GPIO_PIN,LED_flag>>7);
//	LED_flag=~LED_flag;
//}
void Data_LCD_Display()//常规显示
{
	vu16 a=0,b=0;
	LCD_SetWindows(0,0,320,240);//设置显示窗口  
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);//标志：数据写入

	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 0);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 1);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==0);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 0);
	
	FIFO_Reset_Read_Addr(); 
 
	
	for (a=0;a<240;a++)
	{
		for(b=0;b<320;b++)
		{
			GPIOC->BRR =1<<4;
			AA=GPIOA->IDR;						
			GPIOC->BSRR =1<<4;
			
			GPIOC->BRR =1<<4;
			BB=GPIOA->IDR&0x00ff;								
			GPIOC->BSRR =1<<4;	
			
			color=(AA<<8)|BB;
			
			LCD_DATA_PORT->ODR = color;

			GPIOC->BRR =1<<11;
			GPIOC->BSRR =1<<11;
		}							
	}	 
}

void StringToPicture()//数组->图片
{
	vu16 a=0,b=0,e=0,num1=0;
	
	for(a=0;a<50;a++)//50排
	{
		for(b=0;b<24;b++)//24行
		{
			if(talble_0[b/8+a*3]&(1<<(7-b%8)))
			{
				num1=0xffff;
			}
			else
			{
				num1=0x0000;
			}
			LCD_DrawPoint(b+296,a+191,num1);//画点
		}				
	}	
}


void Data_LCD_ColorChange_Test()//摄像头扫描测试
{
	vu16 a=0,b=0;
	
	for(a=0;a<240;a++)//各行跳变点计数，数组清零
	{
		TableChangePoint_240[a]=0;
	}
	Min_blue=320;//初始化记录蓝色车牌区域的值
	Max_blue=0;
	
	LCD_SetWindows(0,0,320,240);//设置显示窗口
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);//标志：数据写入

	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 0);
	GPIO_WriteBit(FIFO_WRST_PORT, FIFO_WRST_PIN, 1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 1);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==0);
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==1);
	GPIO_WriteBit(FIFO_WR_PORT, FIFO_WR_PIN, 0);
	
	FIFO_Reset_Read_Addr(); 
 
	
	for (a=0;a<240;a++)
	{
		for(b=0;b<320;b++)
		{
			GPIOC->BRR =1<<4;
			AA=GPIOA->IDR;						
			GPIOC->BSRR =1<<4;
			
			GPIOC->BRR =1<<4;
			BB=GPIOA->IDR&0x00ff;								
			GPIOC->BSRR =1<<4;	
			
			color=(AA<<8)|BB;

			R=color>>11;
			G=(color>>5)&0x3f;
			B=color&0x1f;
			
			if((R>R_a) && (G>=G_a) && (B>=B_a))//二值化,高阈值：25.55.25，较合适阈值（21,47,21）
			{
				color=0xffff;
			}
			else
			{
				color=0x0000;
			}
			
			if(color!=color_save)//跳变点
			{
				TableChangePoint_240[a]++;		//该行跳变点计数+1
			}
			color_save=color;//保存像素值，供下一次判断和比较
			
			color=(AA<<8)|BB;//还原色彩
					
			LCD_DATA_PORT->ODR = color;
			GPIOC->BRR =1<<11;
			GPIOC->BSRR =1<<11;
		}
	}
	ChangePoint_Show_240();//240方向跳变点显示
	ChangePoint_Analysis_240();	//跳变点分析  获得车牌高度
}

void Data_LCD_ColorChange()//摄像头扫描
{
	vu8 i=0;

	Data_LCD_ColorChange_Test();
	//返回flag_MaxMinCompare，Min_ChangePoint_240，Max_ChangePoint_240
	if(flag_MaxMinCompare==1)//跳变点筛选成功  即高度合理
	{
			BEEP_ON;
			delayms(500);
			BEEP_OFF;
		//分析显示车牌左右边界
		 ChangePoint_Analysis_Blue();//320蓝色区域分析,采用读取像素，得结果Min_blue,Max_blue  分析车牌左右边界
		if(Min_blue>Max_blue) 
			flag_MaxMinCompare=0;//进行合理性判断1
		if((Min_blue>290)||(Max_blue>290)) 
			flag_MaxMinCompare=0;//进行合理性判断2
	}
	if(flag_MaxMinCompare==1)//跳变点筛选成功  左右边界获取成功 并且合理
	{
		ChangePoint_Analysis_320();//蓝色区域中，320跳变点分析,获得：TableChangePoint_320[b]结果 左右边界内二值化
		ChangePoint_Show_320();//320方向跳变点显示
		i=SegmentationChar(); 
		
		if(i==8)//字符分割,返回分割的字符个数，用于判断合法性
		{
			ZhiFuShiBie();//字符识别	
		}
		else
		{
			LCD_Fill(0x6666);//黑屏，显示Measure Faill
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

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
//		LED();
		if(tim3_num==60)
		{
			if(table_card[0][0]!=0)//第1组计时
			{
				table_card[0][7]++;
			}		
			if(table_card[1][0]!=0)//第2组计时
			{
				table_card[1][7]++;
			}
			if(table_card[2][0]!=0)//第3组计时
			{
				table_card[2][7]++;
			}	
			if(table_card[3][0]!=0)//第4组计时
			{
				table_card[3][7]++;
			}	
			if(table_card[4][0]!=0)//第5组计时
			{
				table_card[4][7]++;
			}
			tim3_num=0;
		}		
	}			
	tim3_num++;
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
void TIM3_Configuration(void)
	{
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* ---------------------------------------------------------------
	TIM3CLK 即PCLK1=36MHz
	TIM3CLK = 36 MHz, Prescaler = 7200, TIM3 counter clock = 5K,即改变一次为5K,周期就为10K
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到10000为1000ms
	TIM_TimeBaseStructure.TIM_Prescaler =(12800-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* Enables the Update event for TIM3 */
	//TIM_UpdateDisableConfig(TIM3,ENABLE); 	//使能 TIM3 更新事件 
	
	/* TIM IT enable */
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
}

int main(void)
{  
	unsigned int num=0;
 
	Stm32_Clock_Init(16);//初始化时钟
//	Led_init();			//初始化 LED	
	Lcd_Gpio_Init();
	LCD_Init();	
	Key_init();	//初始化 KEY1 PA8
	OV7670_Gpio_Init();//OV7670引脚初始化，放在串口初始化前面
	GPIO_WriteBit(FIFO_OE_PORT, FIFO_OE_PIN, 0);
	//USART1_init();//初始化串口	
	ESP8266_Init();      //8266初始化 波特率115200
#ifdef WIFI	
	ESP8266_AT_Test (); //AT指令测试
	ESP8266_Net_Mode_Choose (STA_AP); //设置模式 
	ESP8266_Enable_MultipleId ( ENABLE );  //设置为多链接模式
	ESP8266_StartOrShutServer(ENABLE,"8080","100"); //开启服务器模式，端口8080
#endif 
	TIM3_Configuration();//10Khz的计数频率，计数到5000为500ms  
	LCD_Fill(0x6666);		
	printf("Welecom\r\n");
	while(!Sensor_init());
	
	LCD_Fill(0xF800);
	delayms(100);
	num=2;
//二值化阈值	
	R_a=24;
	G_a=53;
	B_a=24;

	while(1)
	{
			if(num<=1)
			{
				Data_LCD_ColorChange();//车牌测定
			}
			if(num>1)
			{
				Data_LCD_ColorChange_Test();//摄像头扫描测试
				LCD_ShowNum(30,220,21-num,2);//
				while(GPIO_ReadInputDataBit(KEY1_PORT,KEY1_PIN)==0)
				{
					LCD_Fill(0x00);//黑屏
					Show_Title();//显示标题
					Show_Card(0);//显示第几组车牌
					Show_Card(1);
					Show_Card(2);
					Show_Card(3);
					Show_Card(4);
					delay_ms(5000);
				}
			}
		if(num==20)
		{
			 num=0;
		}
			 num++;
	}
 
}


