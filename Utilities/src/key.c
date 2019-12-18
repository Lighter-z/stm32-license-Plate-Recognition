#include "stm32f10x.h"
#include "key.h"

void Key_init(void)	//初始化 KEY
{
    GPIO_InitTypeDef gpio_init_struct;//结构体
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //初始化 KEY 时钟   ！！！！！！！！！！！！！！！
		RCC_APB2PeriphClockCmd(KEY0_RCC_PERIPH, ENABLE);
		RCC_APB2PeriphClockCmd(KEY1_RCC_PERIPH, ENABLE);
	
    /* 配置按键 (key0) GPIO 为浮空输入，速度为 50MHz */
    gpio_init_struct.GPIO_Pin = KEY0_GPIO_PIN;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;（上拉）----浮空和上拉可行,对于按键来说，设置上拉更好，不用外加上拉电阻
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);
    
		/* 配置按键 (key1) GPIO 为浮空输入，速度为 50MHz */
		gpio_init_struct.GPIO_Pin = KEY1_GPIO_PIN;
		gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;//（上拉）-
		GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);
	
//     /* 配置按键 (key2) GPIO 为浮空输入，速度为 50MHz */
//     gpio_init_struct.GPIO_Pin = KEY2_GPIO_PIN;
// 		gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;//（上拉）----浮空和上拉可行,对于按键来说，设置上拉更好，不用外加上拉电阻
//     GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct);
}

