#include "stm32f10x.h"
#include "usart.h"
#include "stdio.h"
 

 
 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
 
void USART1_init(void)//初始化串口
{
	GPIO_InitTypeDef   gpio_init_struct;
	USART_InitTypeDef  usart_init_struct;//结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//初始化串口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	//配置串口(UART)用到的 GPIO
	/* 配置按键串口的 TX 线为推挽式复用功能，速度为 50MHz */
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init_struct.GPIO_Pin = GPIO_Pin_9;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init_struct);

	/* 配置按键串口的 RX 线为浮空输入，速度为 50MHz */
	gpio_init_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_init_struct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &gpio_init_struct);

	/* 串口配置如下:
		 - 波特率     = 115200  
		 - 数据位长度 = 8 Bits
		 - 停止位长度 = 1 Bit
		 - 奇偶校验位:  无
		 - 硬件流控:    无 (RTS 和 CTS 信号线)
		 - 发送和接收:  使能
	*/
	usart_init_struct.USART_BaudRate = 115200;
	usart_init_struct.USART_WordLength = USART_WordLength_8b;
	usart_init_struct.USART_StopBits = USART_StopBits_1;
	usart_init_struct.USART_Parity = USART_Parity_No;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART 配置 */
	USART_Init(USART1, &usart_init_struct);
	USART_Cmd(USART1, ENABLE);	//使能 USART1
}

void USART1_SendChar(int32_t ch)
{
	/* 等待输出结束 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{
	}
	USART_SendData(USART1, (uint8_t) ch);
}
u16 USART1_ReceiveChar(void)
{
	/* 等待有输入 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{
	}
	return USART_ReceiveData(USART1);
}

void USART1_SendNum(u32 num)//发送四位数数字
{
	u8 ge,shi,bai,qian;

	ge=num%10;
	shi=num%100/10;
	bai=num%1000/100;
	qian=num/1000;

	USART1_SendChar(qian+0x30);
	USART1_SendChar(bai+0x30);
	USART1_SendChar(shi+0x30);
	USART1_SendChar(ge+0x30);

	USART1_SendChar(0x0d);//回车键，换行
	USART1_SendChar(0x0a);
}
void USART1_SendNum_0d0a(u32 num)//发送四位数数字
{
	u8 ge,shi,bai,qian;

	ge=num%10;
	shi=num%100/10;
	bai=num%1000/100;
	qian=num/1000;

	USART1_SendChar(qian+0x30);
	USART1_SendChar(bai+0x30);
	USART1_SendChar(shi+0x30);
	USART1_SendChar(ge+0x30);

	USART1_SendChar(' ');//回车键，换行

}

