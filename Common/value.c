#include "value.h"
#include "stm32f10x.h"

vu8 cur_status = 0;
vu8 LED_flag = 0;

vu16 AA = 0,BB = 0;
vu16 color=0;
vu16 color_save=0;//保存一个像素的值
vu8 R=0,G=0,B=0;//颜色分量
vu8 TableChangePoint_240[240];//跳变点240个
vu8 Max_ChangePoint_240=0,Min_ChangePoint_240=0,Max_bChangePoint=0,Min_bChangePoint=0;//跳变点纵轴始、末坐标,跳变点横轴始、末坐标
vu8 a_Continue=0,b_Continue=0;//记录纵、横轴突变点的连续性
vu8 flag_aMax=0;//末值更新标志
vu8 Max_aChangePoint_reset=0,Min_aChangePoint_reset=0;//修正后的上下限
vu16 Length_card=0,Width_card=0;//车牌的长和宽
vu8 Max_aChangePoint_reset_1=0,Min_aChangePoint_reset_1=0;//保存上次的数据
vu8 flag_MaxMinCompare=0;//Max_aChangePoint_reset_1和Max_aChangePoint_reset的标志
vu8 TableChangePoint_320[320];//纵向跳变点320个
float V=0.00,S=0.00,H=0.00;//定义HSV值
vu16 Min_blue=0;
vu16 Max_blue=0;//定义车牌蓝色区域的横向最大值和最小值

//八个字符边界
vu16 character_boundary_right_1 = 0,character_boundary_left_1 = 0;
vu16 character_boundary_right_2 = 0,character_boundary_left_2 = 0;
vu16 character_boundary_right_3 = 0,character_boundary_left_3 = 0;
vu16 character_boundary_right_4 = 0,character_boundary_left_4 = 0;
vu16 character_boundary_right_5 = 0,character_boundary_left_5 = 0;
vu16 character_boundary_right_6 = 0,character_boundary_left_6 = 0;
vu16 character_boundary_right_7 = 0,character_boundary_left_7 = 0;
vu16 character_boundary_right_8 = 0,character_boundary_left_8 = 0;

vu8 Red_Vlaue = 0, Green_Value = 0 ,Blue_Value = 0;//阈值

vu8 table_picture[150];//定义保存图片的数组
vu8 table_char[36]={0,1,2,3,4,5,6,7,8,9,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',};
	
vu8 table_char_char[36]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',};
vu8 table_card[5][8]={	//保存5个车牌的二维数组
{0,0,0,0,0,0,0,0},		//最后一位保存时间
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
};


vu8 table_cardMeasure[7];//测量的车牌结果

