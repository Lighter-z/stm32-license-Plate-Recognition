#include "ShowChar.h"
#include "lcd.h"

extern vu8 table_yu[32];//渝字
extern vu8 table_min[32];//闽字
extern vu8 table_lu[32];//鲁字
extern vu8 table_zhe[32];//浙字
extern vu8 table_shan[32];//陕字
extern vu8 table_cuan[32];//川字

extern vu8 table_picture[150]; //定义保存图片的数组

void WordShow(vu8 num,vu16 x,vu16 y)//显示汉字16*16
{
	vu16 a=0,b=0,e=0,num1=0;
	vu8 table1[32]={0};
	if(num==1)
	{
		for(a=0;a<32;a++)
		{
			table1[a]=table_yu[a];	
		}		
	}
	if(num==2)
	{
		for(a=0;a<32;a++)
		{
			table1[a]=table_min[a];	
		}		
	}
	if(num==3)
	{
		for(a=0;a<32;a++)
		{
			table1[a]=table_lu[a];	
		}		
	}
	if(num==4)
	{
		for(a=0;a<32;a++)
		{
			table1[a]=table_zhe[a];	
		}		
	}
	if(num==5)
	{
		for(a=0;a<32;a++)
		{
			table1[a]=table_shan[a];	
		}		
	}
	if(num==6)
	{
		for(a=0;a<32;a++)
		{
			table1[a]=table_cuan[a];	
		}		
	}
	for(a=0;a<16;a++)
	{
		for(b=0;b<16;b++)
		{
			if(table1[b/8+a*2]&(1<<(7-b%8)))
			{
				num1=0xffff;
			}
			else
			{
				num1=0x0000;
			}
			LCD_DrawPoint(b+x,a+y,num1);//画点
		}				
	}	
}



void PictureToString(void)//图片->数组table_picture   150字节   
{
	
	vu16 a=0,b=0,num1=0;
	for(a=0;a<150;a++)//归零
	{
		table_picture[a]=0x00;	
	}	
	for(a=0;a<50;a++)//50行
	{
		for(b=0;b<24;b++)//24排
		{
			num1=LCD_ReadPoint(b+296,a+191);
			if(num1==0xffff)
			{
				table_picture[b/8+a*3]|=(1<<(7-b%8));   //(1<<(7-b%8))   将"1" 变换在单字节中的相应位置上。  
			}
		}				
	}
}


