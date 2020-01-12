/**
  ******************************************************************************
  * 文件: ltk_usart.h
  * 作者: LeiTek (leitek.taobao.com)
  * 版本: V1.0.0
  * 描述: 串口中间件头文件
  ******************************************************************************
  *
  *                  版权所有 (C): LeiTek (leitek.taobao.com)
  *                                www.leitek.com
  *
  ******************************************************************************
  */
  
/* 防止递归包含该头文件 ------------------------------------------------------*/
#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

void USART1_init(void);//初始化串口
void USART1_SendChar(int32_t ch);
void USART1_SendNum(u32 num);
u16  USART1_ReceiveChar(void);


#ifdef USART1_IRQ
void ltk_usart_nvic_init(void);
#endif

#ifdef __cplusplus
}
#endif


#endif /* __LTK_USART_H */

/****************************** leitek.taobao.com *****************************/
