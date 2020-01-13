#ifndef __VALUE_H
#define __VALUE_H

#include "stm32f10x.h"

extern vu8 cur_status;
extern vu8 LED_flag;

extern vu16 AA,BB;
extern vu16 color;
extern vu16 color_save;//保存一个像素的值
extern vu8 R,G,B;//颜色分量
extern vu8 TableChangePoint_240[240];//跳变点240个
extern vu8 Max_ChangePoint_240,Min_ChangePoint_240,Max_bChangePoint,Min_bChangePoint;//跳变点纵轴始、末坐标,跳变点横轴始、末坐标
extern vu8 a_Continue,b_Continue;//记录纵、横轴突变点的连续性
extern vu8 flag_aMax;//末值更新标志
extern vu8 Max_aChangePoint_reset,Min_aChangePoint_reset;//修正后的上下限
extern vu16 Length_card,Width_card;//车牌的长和宽
extern vu8 Max_aChangePoint_reset_1,Min_aChangePoint_reset_1;//保存上次的数据
extern vu8 flag_MaxMinCompare;//Max_aChangePoint_reset_1和Max_aChangePoint_reset的标志
extern vu8 TableChangePoint_320[320];//纵向跳变点320个
extern float V,S,H;//定义HSV值
extern vu16 Min_blue;
extern vu16 Max_blue;//定义车牌蓝色区域的横向最大值和最小值
//extern vu16 k1,kk1,k2,kk2,k3,kk3,k4,kk4,k5,kk5,k6,kk6,k7,kk7,k8,kk8;//八个字符边界
extern vu16 character_boundary_right_1,character_boundary_left_1;
extern vu16 character_boundary_right_2,character_boundary_left_2;
extern vu16 character_boundary_right_3,character_boundary_left_3;
extern vu16 character_boundary_right_4,character_boundary_left_4;
extern vu16 character_boundary_right_5,character_boundary_left_5;
extern vu16 character_boundary_right_6,character_boundary_left_6;
extern vu16 character_boundary_right_7,character_boundary_left_7;
extern vu16 character_boundary_right_8,character_boundary_left_8;

extern vu8 Red_Vlaue, Green_Value ,Blue_Value;//阈值

extern vu8 table_picture[150];//定义保存图片的数组
extern vu8 table_char[36];
	
extern vu8 table_char_char[36];
extern vu8 table_card[5][8];//保存5个车牌的二维数组

extern vu8 table_cardMeasure[7];//测量的车牌结果


#endif
