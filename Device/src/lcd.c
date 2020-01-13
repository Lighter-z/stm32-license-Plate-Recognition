#include "stm32f10x.h"
#include "lcd.h"
#include "delay.h" 
#include "board.h"
#include "font.h"


static void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	//初始化时钟
	RCC_APB2PeriphClockCmd(LCD_CS_RCC | LCD_RD_RCC | LCD_WR_RCC | LCD_RS_RCC | LCD_REST_RCC |
													LCD_DATA_RCC | RCC_APB2Periph_AFIO, ENABLE);
	/* 配置 JLINK 为 SW 模式 */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//PB3 and PB4 --> gpio function ok

	/* 配置 CS 为推挽输出 */       
	gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_AF_PP
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_Pin = LCD_CS_PIN;
	GPIO_Init(LCD_CS_PORT, &gpio_init_struct);
	
	/* 配置 RD 为推挽输出 *///
	gpio_init_struct.GPIO_Pin = LCD_RD_PIN;
	GPIO_Init(LCD_RD_PORT, &gpio_init_struct);

	/* 配置 WR 为推挽输出 */
	gpio_init_struct.GPIO_Pin = LCD_WR_PIN;
	GPIO_Init(LCD_WR_PORT, &gpio_init_struct);

	/* 配置 RS 为推挽输出 */
	gpio_init_struct.GPIO_Pin = LCD_RS_PIN;
	GPIO_Init(LCD_RS_PORT, &gpio_init_struct);

	/* 配置 REST 为推挽输出 */
	gpio_init_struct.GPIO_Pin = LCD_REST_PIN;
	GPIO_Init(LCD_REST_PORT, &gpio_init_struct);
	
	/* 配置 LCD DATA 为推挽输出 */
	gpio_init_struct.GPIO_Pin = LCD_DATA_PIN;
	GPIO_Init(LCD_DATA_PORT, &gpio_init_struct);
		
	GPIO_WriteBit(LCD_RD_PORT, LCD_RD_PIN,1);
	GPIO_WriteBit(LCD_WR_PORT, LCD_WR_PIN,1);
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);
	GPIO_WriteBit(LCD_CS_PORT, LCD_CS_PIN,0);//片选，拉低有效
}
static void LCD_Writ_Bus(unsigned short bus_data)//16位
{
	LCD_DATA_PORT->ODR = bus_data;
	GPIO_WriteBit(LCD_WR_PORT, LCD_WR_PIN,0);
	GPIO_WriteBit(LCD_WR_PORT, LCD_WR_PIN,1);
}
static void LCD_Write_COM(u16 bus_data)	
{	
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,0);
	LCD_Writ_Bus(bus_data);
}
static void LCD_Write_DATA(u16 bus_data)	
{
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);
	LCD_Writ_Bus(bus_data);
}
void LCD_Init(void)
{
	LCD_GPIO_Init();
	GPIO_WriteBit(LCD_REST_PORT, LCD_REST_PIN,1);//需要一定的延时，保证初始化进行
  delayms(10);	
	GPIO_WriteBit(LCD_REST_PORT, LCD_REST_PIN,0);
	delayms(10);
	GPIO_WriteBit(LCD_REST_PORT, LCD_REST_PIN,1);
	delayms(10);
	LCD_Write_COM(0xCF);  
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0xC1); 
	LCD_Write_DATA(0X30); 
	LCD_Write_COM(0xED);  
	LCD_Write_DATA(0x64); 
	LCD_Write_DATA(0x03); 
	LCD_Write_DATA(0X12); 
	LCD_Write_DATA(0X81); 
	LCD_Write_COM(0xE8);  
	LCD_Write_DATA(0x85); 
	LCD_Write_DATA(0x10); 
	LCD_Write_DATA(0x7A); 
	LCD_Write_COM(0xCB);  
	LCD_Write_DATA(0x39); 
	LCD_Write_DATA(0x2C); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x34); 
	LCD_Write_DATA(0x02); 
	LCD_Write_COM(0xF7);  
	LCD_Write_DATA(0x20); 
	LCD_Write_COM(0xEA);  
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 
	LCD_Write_COM(0xC0);    //Power control 
	LCD_Write_DATA(0x1B);   //VRH[5:0] 
	LCD_Write_COM(0xC1);    //Power control 
	LCD_Write_DATA(0x01);   //SAP[2:0];BT[3:0] 
	LCD_Write_COM(0xC5);    //VCM control 
	LCD_Write_DATA(0x30); 	 //3F
	LCD_Write_DATA(0x30); 	 //3C
	LCD_Write_COM(0xC7);    //VCM control2 
	LCD_Write_DATA(0XB7); 
	LCD_Write_COM(0x36);    // Memory Access Control 
	LCD_Write_DATA(0x48); 
	LCD_Write_COM(0x3A);   
	LCD_Write_DATA(0x55); 
	LCD_Write_COM(0xB1);   
	LCD_Write_DATA(0x00);   
	LCD_Write_DATA(0x1A); 
	LCD_Write_COM(0xB6);    // Display Function Control 
	LCD_Write_DATA(0x0A); 
	LCD_Write_DATA(0xA2); 
	LCD_Write_COM(0xF2);    // 3Gamma Function Disable 
	LCD_Write_DATA(0x00); 
	LCD_Write_COM(0x26);    //Gamma curve selected 
	LCD_Write_DATA(0x01); 
	LCD_Write_COM(0xE0);    //Set Gamma 
	LCD_Write_DATA(0x0F); 
	LCD_Write_DATA(0x2A); 
	LCD_Write_DATA(0x28); 
	LCD_Write_DATA(0x08); 
	LCD_Write_DATA(0x0E); 
	LCD_Write_DATA(0x08); 
	LCD_Write_DATA(0x54); 
	LCD_Write_DATA(0XA9); 
	LCD_Write_DATA(0x43); 
	LCD_Write_DATA(0x0A); 
	LCD_Write_DATA(0x0F); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x00); 		 
	LCD_Write_COM(0XE1);    //Set Gamma 
	LCD_Write_DATA(0x00); 
	LCD_Write_DATA(0x15); 
	LCD_Write_DATA(0x17); 
	LCD_Write_DATA(0x07); 
	LCD_Write_DATA(0x11); 
	LCD_Write_DATA(0x06); 
	LCD_Write_DATA(0x2B); 
	LCD_Write_DATA(0x56); 
	LCD_Write_DATA(0x3C); 
	LCD_Write_DATA(0x05); 
	LCD_Write_DATA(0x10); 
	LCD_Write_DATA(0x0F); 
	LCD_Write_DATA(0x3F); 
	LCD_Write_DATA(0x3F); 
	LCD_Write_DATA(0x0F); 
	LCD_Write_COM(0x2B); 
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x01);
	LCD_Write_DATA(0x3f);
	LCD_Write_COM(0x2A); 
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0xef);	 
	LCD_Write_COM(0x11); //Exit Sleep
	delay_ms(120);
	LCD_Write_COM(0x29); //display on	
	//设置LCD属性参数
	LCD_Write_COM(0x36);
	LCD_Write_DATA(0x6C); //0x0a
}

void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_Write_COM(0x2a);	
	LCD_Write_DATA(xStar>>8);
	LCD_Write_DATA(0x00FF&xStar);		
	LCD_Write_DATA(xEnd>>8);
	LCD_Write_DATA(0x00FF&xEnd);
	LCD_Write_COM(0x2b);	
	LCD_Write_DATA(yStar>>8);
	LCD_Write_DATA(0x00FF&yStar);		
	LCD_Write_DATA(yEnd>>8);
	LCD_Write_DATA(0x00FF&yEnd);	
	LCD_Write_COM(0x2C);//开始写入GRAM				
}  
void LCD_Fill(unsigned short color)
{  	
	unsigned short i,j;			
	LCD_SetWindows(0,0,320,240);//设置显示窗口
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);//标志：数据写入
	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		LCD_Writ_Bus(color);	//写入数据 	 
	}
}
void LCD_DrawPoint(u16 x,u16 y,u16 color)//在指定位置写入一个像素点数据
{
	LCD_SetWindows(x,y,x,y);//设置光标位置 
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);//标志：数据写入
	LCD_Writ_Bus(color); 	    
} 
void LCD_ShowChar(u16 x,u16 y, u8 num,u8 mode)//X坐标，Y坐标，字符，填充模式
{  
	u8 temp;
	u8 pos,t;    
 	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
		for(pos=0;pos<18;pos++)
		{
			//temp=asc2_1206[num][pos];//调用1206字体，较小的字体
			temp=asc2_1608[num][pos];		 //调用1608字体，较大的字体
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)LCD_DrawPoint(x+t,y+pos,0xffff);//画一个点 ，白色点0xffff
				else LCD_DrawPoint(x+t,y+pos,0x0000);//背景色，黑色0x0000
				temp>>=1; 
			}		
		}	
	}
	else//叠加方式
	{
		for(pos=0;pos<20;pos++)
		{
// 			temp=asc2_1206[num][pos];//调用1206字体，较小的字体
			temp=asc2_1608[num][pos];		 //调用1608字体，较大的字体
			for(t=0;t<10;t++)
			{                 
				if(temp&0x01)LCD_DrawPoint(x+t,y+pos,0xffff);//画一个点    
				temp>>=1; 
			}		
		}
	}
	LCD_SetWindows(0,0,320,240);//恢复窗口为全屏    	   	 	  
} 
u32 mypow(u8 m,u8 n)//m^n函数
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}		
void LCD_ShowNumPoint(u16 x,u16 y,u16 num)//显示4位数+2位小数点
{
	LCD_ShowChar(x,y,num/10000+0x30,0);
	LCD_ShowChar(x+8*1,y,num/1000%10+0x30,0);
	LCD_ShowChar(x+8*2,y,num/100%10+0x30,0);
	LCD_ShowChar(x+8*3,y,'.',0);
	LCD_ShowChar(x+8*4,y,num/10%10+0x30,0);
	LCD_ShowChar(x+8*5,y,num%10+0x30,0);
}
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)//X坐标，Y坐标，num(0~4294967295)，长度
{         	
	u8 t,temp;				   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;//取位
	 	LCD_ShowChar(x+t*8,y,temp+'0',0); //t*11:调节数字各位的间距
	}
} 
//读取个某点的颜色值  
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
    u16 r,g,b;
    if(x>=320||y>=240)return 0;    //超过了范围,直接返回           
//     LCD_SetCursor(x,y);
	LCD_SetWindows(x,y,x,y);
//     LCD_WR_REG(0X2E);    //9341/6804/5310发送读GRAM指令
    LCD_Write_COM(0X2E);	
    GPIOB->CRL=0X88888888; //PB0-7  上拉输入
    GPIOB->CRH=0X88888888; //PB8-15 上拉输入
    GPIOB->ODR=0XFFFF;     //全部输出高
 
//     LCD_RS_SET;
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS_PIN,1);
//     LCD_CS_CLR;     
	GPIO_WriteBit(LCD_CS_PORT, LCD_CS_PIN,0);//片选，拉低有效
    //读取数据(读GRAM时,第一次为假读)   
//     LCD_RD_CLR; 
	GPIO_WriteBit(LCD_RD_PORT, LCD_RD_PIN,0);
    delay_us(100);//延时1us                    
//     LCD_RD_SET;
	GPIO_WriteBit(LCD_RD_PORT, LCD_RD_PIN,1);
    //dummy READ
//     LCD_RD_CLR;           
	GPIO_WriteBit(LCD_RD_PORT, LCD_RD_PIN,0);
    delay_us(100);//延时1us                    
//     r=DATAIN;   //实际坐标颜色
	r=GPIOB->IDR;	
//     LCD_RD_SET;
	GPIO_WriteBit(LCD_RD_PORT, LCD_RD_PIN,1);
 
//         LCD_RD_CLR;   
	GPIO_WriteBit(LCD_RD_PORT, LCD_RD_PIN,0);
//         b=DATAIN;//读取蓝色值    
	b=GPIOB->IDR;	
//         LCD_RD_SET;
	GPIO_WriteBit(LCD_RD_PORT, LCD_RD_PIN,1);
        g=r&0XFF;//对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
        g<<=8;
//     LCD_CS_SET;
// 	GPIO_WriteBit(LCD_CS_PORT, LCD_CS_PIN,1);//片选，拉低有效
    GPIOB->CRL=0X33333333;       //PB0-7  上拉输出
    GPIOB->CRH=0X33333333;       //PB8-15 上拉输出
    GPIOB->ODR=0XFFFF;           //全部输出高  
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510需要公式转换一下

}