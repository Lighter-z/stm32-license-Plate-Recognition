#include "stm32f10x.h"
#include "led.h"

void Led_init(void)	//初始化 LED
{
		GPIO_InitTypeDef gpio_init_struct;//结构体
		
    RCC_APB2PeriphClockCmd(LED1_RCC_PERIPH, ENABLE);//初始化 LED 时钟 
	   RCC_APB2PeriphClockCmd(BEEP_RCC_PERIPH, ENABLE);//初始化 LED 时钟 
	    
    /* 配置 LED1 GPIO 为推挽输出，速度为 50MHz */
    gpio_init_struct.GPIO_Pin = LED1_GPIO_PIN;			
    gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);
  
	    /* 配置BEEP GPIO 为推挽输出，速度为 50MHz */
    gpio_init_struct.GPIO_Pin = BEEP_GPIO_PIN;			
    gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_GPIO_PORT, &gpio_init_struct);
}

