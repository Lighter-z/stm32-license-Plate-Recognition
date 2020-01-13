#ifndef __SHOW_CHAR_H
#define __SHOW_CHAR_H
	 
#include "stm32f10x.h"
#include "value.h"

void Show_Title(void);
void WordShow(vu8 num,vu16 x,vu16 y);
void PictureToString(void);
void StringToPicture(void);
void Show_Card(vu8 i);
void TIM3_Configuration(void);


#endif
