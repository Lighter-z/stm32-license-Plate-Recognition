  
/* 防止递归包含该头文件 ------------------------------------------------------*/

#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
	 
void Led_init(void);	//初始化 LED
	 
#ifdef __cplusplus
}
#endif


#endif 