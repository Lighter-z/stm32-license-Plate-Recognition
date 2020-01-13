#include "stm32f10x.h"
#include "delay.h"

void delay(u32 z)//延时函数us
{
	u32 x,y;
	for(x=z;x>0;x--)
		for(y=500;y>0;y--);
}
void delay_ms(u32 z)//24000,换成STM32了==10000，速度慢了？
{
	u32 x,y;
	for(x=z;x>0;x--)							//仿真y=15000时，delams(1000)=1.04 s
		for(y=15000;y>0;y--);
}
void delay_us(u32 z)//20
{
	u32 x,y;
	for(x=z;x>0;x--)
		for(y=20;y>0;y--);
}

void delayms(u16 nms)      
{  
	 u32 temp;  
	 SysTick->LOAD = 9000*nms;  
	 SysTick->VAL=0X00;//?????
	 SysTick->CTRL=0X01;//??,???????,???????
	do 
	 {  
			temp=SysTick->CTRL;//???????? 
	 }
while((temp&0x01)&&(!(temp&(1<<16))));//??????
    SysTick->CTRL=0x00; //?????
    SysTick->VAL =0X00; //?????
}  
void delayus(u32 nus) 
{ 
	 u32 temp; 
	 SysTick->LOAD = 9*nus; 
	 SysTick->VAL=0X00;//?????
	 SysTick->CTRL=0X01;//??,???????,???????
 do 
 { 
		temp=SysTick->CTRL;//????????
 }
		while((temp&0x01)&&(!(temp&(1<<16))));//??????
			SysTick->CTRL=0x00; //?????
			SysTick->VAL =0X00; //?????
} 
