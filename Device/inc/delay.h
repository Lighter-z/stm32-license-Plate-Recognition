#include "stm32f10x.h"
  
/* 防止递归包含该头文件 ------------------------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
 extern "C" {
#endif 

void delayms(u16 z);
void delayus(u32 z);
	 
void delay_ms(u32 z);
void delay_us(u32 z);
#ifdef __cplusplus
}
#endif


#endif 

