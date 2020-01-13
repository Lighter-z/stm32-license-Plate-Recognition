#include "algorithm.h"
#include "delay.h"
#include "lcd.h"
#include "ShowChar.h"
#include "string.h"
#include <stdio.h>


void ChangePoint_Show_240(void) {                                     //240方向跳变点显示
	vu16 a = 0,b = 0;
	for(a = 0; a < 240; a++) {                                          //建立参考线10、20、30
		LCD_DrawPoint(10,a,0x63<<5);                                      //10
		LCD_DrawPoint(20,a,0x63<<5);                                      //20
		LCD_DrawPoint(30,a,0x63<<5);                                      //30
	}
	
	for(a = 0; a < 240; a++) {                                          //显示对应的横向跳变点	
		LCD_DrawPoint(TableChangePoint_240[a],a,0xf800);                  //跳变点显示，红色标记
		if(TableChangePoint_240[a] >= 12) {		                            //跳变点个数（阈值）设定       阈值调节3-（1）
			for(b = 35; b < 40; b++) {	                                    //显示达到阈值标准的点
				LCD_DrawPoint(b,a,0x63<<5);                                   //Green			
			}
		}
	}
}


void ChangePoint_Analysis_240(void) {                                 //240跳变点分析  获取高度
	vu16 a = 0,b = 0;
	Min_ChangePoint_240 = 240;
	Max_ChangePoint_240 = 0;
	
	for(a = 0; a < 240; a++) {                                          //240扫描	，获取上下限值	：Min_ChangePoint_240，Max_ChangePoint_240		
		while(TableChangePoint_240[a]<12) {							              		//阈值调节3-（2）
			a++;
		}
		Min_ChangePoint_240 = a;                                          //上边界
		while(TableChangePoint_240[a] >= 12) {								           	//阈值调节3-（3）
			a++;
		}
		Max_ChangePoint_240 = a;                                           //下边界
		if(Max_ChangePoint_240 - Min_ChangePoint_240 >= 15) {              //连续性阈值   	//阈值调节2-（1）
			a=240;
		}
	}
	Min_ChangePoint_240 = Min_ChangePoint_240 - 3;                       //向上微调3像素
	Max_ChangePoint_240 = Max_ChangePoint_240 + 2;                       //向下微调2像素
	for(a = 30; a < 280; a++) {                                          //显示上界限	
		LCD_DrawPoint(a, Max_ChangePoint_240, 0x001f);
	}
	for(a = 30; a < 280; a++) {                                           //显示下界限		
		LCD_DrawPoint(a, Min_ChangePoint_240, 0x001f);
	}
	for(a = 30; a < 280; a++) {                                           //显示50,参考50像素位置处，车牌位置不要超过这根线，免得不能字符的归一化处理			
		LCD_DrawPoint(a,Min_ChangePoint_240+50,0xf800);
	}
	flag_MaxMinCompare = 1;
	if(Min_ChangePoint_240 > Max_ChangePoint_240) {                       //判断合法性1：最小值>最大值
		flag_MaxMinCompare = 0;
	}
	if(Min_ChangePoint_240 == 240 || Max_ChangePoint_240 == 0) {           //判断合法性2：值没有重新赋值
		flag_MaxMinCompare = 0;
	}
	if(Max_ChangePoint_240-Min_ChangePoint_240 < 15) {                   	//判断合法性3：			//阈值调节2-（2）
		flag_MaxMinCompare = 0;
	}
}

static void RGB_HSV(vu16 num) {                                          //RGB565转HSV
	float max = 0.00,min = 0.00;
	vu8 r = 0,g = 0,b = 0;
	
	r = (num>>11)*255/31;g=((num>>5)&0x3f)*255/63;b=(num&0x1f)*255/31;
	
	max = r;
	min = r;
	
	if(g >= max)max = g;
	if(b >= max)max = b;
	if(g <= min)min = g;
	if(b <= min)min = b;
	
	V = 100 * max / 255;                                                    //转换为百分比
	S = 100 * (max - min) / max;                                            //扩大100倍显示
	if(max == r)H =(g - b) / (max - min) * 60;
	if(max == g)H = 120 + (b - r) / (max - min) * 60;
	if(max == b)H = 240 + (r - g) / (max - min) * 60;
	if(H<0) H=H+360;
}

void ChangePoint_Analysis_Blue(void) {                                    //320蓝色区域分析,采用读取像素，得结果Min_blue,Max_blue
	vu16 a = 0, b = 0, num_color = 0;
	vu16 min_320 = 0, max_320 = 0;                                          //各行的最小、最大值
	
	Min_blue = 0;
	Max_blue = 320;
	min_320 = 320;
	max_320 = 0;
	
	for(a = Min_ChangePoint_240; a < Max_ChangePoint_240; a++) {            //在上下边界扫描	
		for(b = 30; b < 290; b++) {                                           //不用到320    for(b=30;b<320;b++)
			num_color = LCD_ReadPoint(b, a);                                    //读取像素，代码优化速度有待提升 ？扫描方法也可优化，以提升速度
			RGB_HSV(num_color);                                                 //RGB565转HSV
			if(250 > H && H > 190 && 60 > S && S > 15 && 100 > V && V > 45) {   // 和 蓝色HSV 阈值比较
				if(b < min_320) {                                                 //获得横轴的Min和Max值，即蓝色车牌的左右边界
					min_320 = b;                                                    //得到左边界
				}
				if(b > max_320) {
					max_320 = b;                                                    //得到右边界
				}
			}
		}
	}
	Min_blue = min_320;                                                     //获取各行的最大值//修正一点
	Max_blue = max_320 - 5;                                                 //获取各行的最小值//修正一点
	
	for(a = Min_ChangePoint_240; a < Max_ChangePoint_240; a++) {            //显示左界限		
		LCD_DrawPoint(Min_blue, a, 0xf8);                                     //LCD_DrawPoint(Min_blue,a,0xf800);
	}
	for(a = Min_ChangePoint_240; a < Max_ChangePoint_240; a++) {            //显示右界限	
		LCD_DrawPoint(Max_blue, a, 0xf800);
	}
}



void ChangePoint_Analysis_320(void) {								                      //蓝色区域中，320跳变点分析,获得TableChangePoint_320[b]结果
																																					//(先二值化，再判断白点个数，=0则是分割线）
	vu16 a = 0, b = 0,num_color = 0;
	vu8 R1 = 0, G1 = 0, B1 = 0;
	vu8 Mid_ChangePoint_240 = 0;
	vu8 max_R = 0, max_G = 0, max_B = 0, min_R = 0, min_G = 0, min_B = 0;
	vu8 mid_R = 0, mid_G = 0, mid_B = 0;
	
	max_R = 0;
	max_G = 0;
	max_B = 0;
	min_R = 30;
	min_G = 60;
	min_B = 30;
	
	Mid_ChangePoint_240 = (Min_ChangePoint_240 + Max_ChangePoint_240) / 2;
	
	for(b = Min_blue; b < Max_blue; b++) {																	                     //左右边界
		num_color = LCD_ReadPoint(b, Mid_ChangePoint_240);                                         //读取像素，代码优化速度有待提升 ？扫描方法也可优化，以提升速度
		R1 = num_color >> 11;
		G1 = (num_color >> 5) & 0x3F;
		B1 = num_color & 0x1F;
		if((R1 > 10) && (G1 > 25) && (B1 > 15) && (R1 <= 30) && (G1 <= 60) && (B1 <= 30)) {        //二值化,高阈值：25.55.25，较合适阈值（21,47,21）
			if(max_R < R1) max_R = R1;                                                               //获得最大值和最小值
			if(max_G < G1) max_G = G1;
			if(max_B < B1) max_B = B1;
			
			if(min_R > R1) min_R = R1;
			if(min_G > G1) min_G = G1;
			if(min_B > B1) min_B = B1;		
		}
	}
	mid_R = (max_R + min_R) / 2;
	mid_G = (max_G + min_G) / 2;
	mid_B = (max_B + min_B) / 2;
 
	for(b = 0; b < 320; b++) {                                                                  //各行跳变点计数，数组清零
		TableChangePoint_320[b] = 0;
	}
	for(a = Min_ChangePoint_240;a < Max_ChangePoint_240; a++) {
		for(b = Min_blue + 1; b < Max_blue; b++) {
			num_color = LCD_ReadPoint(b, a);                                                        //读取像素，代码优化速度有待提升 ？扫描方法也可优化，以提升速度
			R1 = num_color >> 11;
			G1 = (num_color >> 5) & 0x3F;
			B1 = num_color & 0x1F;
			if((R1 >= mid_R) && (G1 >= mid_G) && (B1 >= mid_B)) {                                   //二值化,高阈值：25.55.25，较合适阈值（21,47,21）
				LCD_DrawPoint(b, a, 0xffff);
				TableChangePoint_320[b]++;                                                            //白色，跳变点+1
			} else {
				LCD_DrawPoint(b, a, 0x0000);
			}
		}
	}
}


void ChangePoint_Show_320(void) {                                                             //320方向跳变点显示
	vu16 a = 0,b = 0;
	for(a = 0;a < 320; a++) {                                                                   //显示对应的横向跳变点	
		if(TableChangePoint_320[a] == 0) {
			LCD_DrawPoint(a, 0, 0x001F);                                                            //跳变点显示，红色标记
		} else {
			LCD_DrawPoint(a, TableChangePoint_320[a], 0xf800);                                      //跳变点显示，红色标记
		}
	}
}

//字符分割,返回分割的字符个数，用于判断合法性
vu8 SegmentationChar(void) {
	vu16 a = 0, b = 0;
	vu8 i = 0;                                                                                  //统计分割的字符个数，不为9说明分割有误
	 
	for(b = Max_blue; b > Min_blue; b--) {     																                  // 左右界线的扫描 
		if(TableChangePoint_320[b] == 0) {       														                      //间隙分割  根据HSV比较 跳变点为0 代表空隙
			for(a = Min_ChangePoint_240; a < Max_ChangePoint_240 ; a++) {  					                //划线--调试用 车牌高度一样的线
 				LCD_DrawPoint(b,a+1,0x001f);
			}
			i++;
			b--;
			while(TableChangePoint_320[b] == 0) {        														         	    //划过线后，找到跳变点不为0的地方
				b--;
				if(b <= Min_blue) break;
			}
		}
	}
	i--;
	LCD_ShowNum(30,220,i,2);																											         		//显示分割的字符个数+1，8是正常值
	return i;
}



static void Normalized(vu16 character_boundary_right, vu16 character_boundary_left)         //归一化 24*50
{
	vu16 a = 0,b = 0,e = 0;
	vu16 num = 0;                                                                             //保存读取像素
	vu8 Mo = 0, Yu = 0;                                                                       //取整和取模
	vu8 num1 = 0, num2 = 0, num3 = 0;
	vu8 Mo_1 = 0;                                                                             //
	vu8 Min_240 = 0, Max_240 = 0;                                                             //框紧字符后的上下限
	
	if((character_boundary_right - character_boundary_left) < 25) {
		//框紧字符
		Min_240 = Min_ChangePoint_240 + 1;																								  		//车牌高度下限
		Max_240 = Max_ChangePoint_240 - 1;																								  		//车牌高度上限
		while(Min_240++) {                                                                      //框紧后，得到: Min_240
			for(b = character_boundary_left + 1; b < character_boundary_right; b++) {             //character_boundary_left → character_boundary_right   
				num = LCD_ReadPoint(b, Min_240);
				if(num) {
					break;
				}
			}
			if(num) {
				break;
			}
		}
		while(Max_240--) {                                                                       //框紧后，得到: Max_240
			for(b = character_boundary_left + 1; b < character_boundary_right; b++) {              //character_boundary_left → character_boundary_right     
				num = LCD_ReadPoint(b,Max_240);
				if(num) {
					break;
				}
			}
			if(num) {
				break;
			}
		}
		Min_240 -= 1;
		Max_240 += 2;
		LCD_DrawPoint(character_boundary_left, Min_240, 0xffff); 
		LCD_DrawPoint(character_boundary_right, Max_240, 0xffff); 
		
		//显示复制的图片
		num3 = 0;
		for(a = Min_240 + 1; a < Max_240; a++) {																								 //高度
			num2=0;
			for(b = character_boundary_left + 1; b < character_boundary_right; b++) {              //character_boundary_left → character_boundary_right   宽度               +1
				num = LCD_ReadPoint(b, a);
				LCD_DrawPoint(271 - (character_boundary_right- character_boundary_left - 1) + num2, 191 + num3, num);                                 //复制像素值  显示
				num2++;
			}
			num3++;
		}
		
		num3 = 0;
		
		//图片放大算法：最近邻插值 先将宽度放大，后将长度放大  宽24*高50
		//将原有图宽度的像素点数与目标24像素点进行相减
		Mo = (24 - (character_boundary_right - character_boundary_left - 1)) / (character_boundary_right - character_boundary_left - 1);          //取模 即可以将放大后比原图宽度多的像素点插在每个原图像素点的个数为Mo
		Yu = (24-(character_boundary_right - character_boundary_left - 1)) % (character_boundary_right - character_boundary_left - 1);            //取余
		if(Yu != 0) {
			Mo_1=24/Yu;//平均Mo_1个像素，插有一个像素， Yu：代表需要在原图的宽度上插Yu个像素点才能达到目标的24个像素  
		}
// 		LCD_ShowNum(30,20,Mo,3);//显示模		<调试用>
// 		LCD_ShowNum(70,20,Yu,3);//显示余
// 		LCD_ShowNum(100,20,(k1-kk1),3);//显示差值

		for(a = Min_240 + 1; a < Max_240; a++) {                                                                                                  //↓    //宽放大为24像素    高扫描
			num2 = 0;
			Yu=(24-(character_boundary_right - character_boundary_left - 1)) % (character_boundary_right - character_boundary_left - 1);            //取余
			
			for(b = character_boundary_left + 1; b < character_boundary_right; b++) {                                                               //character_boundary_left → character_boundary_right                 +1
				num = LCD_ReadPoint(b, a);
				LCD_DrawPoint(271 + num2,191 + num3, num);
				num2++;
				Mo = (24-(character_boundary_right - character_boundary_left - 1)) / (character_boundary_right - character_boundary_left - 1);        //取模
				while(Mo) {
					LCD_DrawPoint(271 + num2, 191 + num3, num);
					Mo--;
					num2++;
				}
				if(Yu != 0)                                                                                                                           //横轴拉长
				{	
					if(((num2+1) % Mo_1==0) && (num2!=1)) {                                                                                             //该插入的地方 
						LCD_DrawPoint(271 + num2, 191 + num3, num);
						Yu--;
						num2++;
					}
				}
			}
			num3++;
		}
		LCD_DrawPoint(271,191,0x07E0);																																																						//标记点，4个顶角
		LCD_DrawPoint(271,240,0x07E0);
		LCD_DrawPoint(295,191,0x07E0);
		LCD_DrawPoint(295,240,0x07E0);
		//纵轴拉长
		if((Max_240 - Min_240) < 50) {
			Mo = (50 - (Max_240 - Min_240 + 1)) / (Max_240 - Min_240 + 1);                                       													          //取模
			Yu = (50 - (Max_240 - Min_240 + 1)) % (Max_240 - Min_240 + 1);                                         													        //取余
			Mo_1 = 50 / Yu;
			
// 			LCD_ShowNum(30,170,Mo,3);//					<调试用>
// 			LCD_ShowNum(70,170,Yu,3);//
// 			LCD_ShowNum(100,170,Max_ChangePoint_240-Min_ChangePoint_240,3);//
			
			num2 = 0;
			for(a = 0; a < (Max_240 - Min_240); a++) {                                                                                              //↓//复制图像,考虑范围是否需要进行修正？
				for(b = 271; b <= 295; b++) {                                                                                                         //271开始复制，295才结束
					num = LCD_ReadPoint(b, a + 191);
					LCD_DrawPoint(b + 25, 191 + num2, num);                                                                                             //复制像素值
				}
				num2++;
				while(Mo) {
					for(b = 271; b <= 295; b++) {                                                                                                       //271开始复制，295才结束
						num = LCD_ReadPoint(b, a + 191);
						LCD_DrawPoint(b + 25, 191 + num2 + a, num);																																												//复制像素值
					}
					Mo--;
					num2++;
				}
				if(Yu != 0) {
					if((((num2 + 1) % Mo_1) == 0)&& (num2 != 1)) {
						for(b = 271; b <= 295; b++) {           																                                                          //271开始复制，295才结束
							num = LCD_ReadPoint(b, a + 191);
							LCD_DrawPoint(b + 25, 191 + num2, num);																																												  //复制像素值
						}
						Yu--;
						num2++;
					}
				}					
			}
		}
		LCD_DrawPoint(320,191,0xf800);																																											                     //标记点，1个顶角
	}
}

static vu8 MoShiShiBie_All(vu8 begin,vu8 end)																																																//字符匹配，模式识别,选择性匹配begin-end
{
	vu16 num_save = 0;
	vu8  a = 0, b = 0, e = 0, a_save = 0, st1 = 0, st2 = 0, s1 = 0, s2 = 0;
	int num1 = 0;
	
	for(a = begin; a < end; a++) {																					                         																					//36
		num1 = 0; 
		for(b = 0; b < 150; b++) { 																																																							//每个字符包含了150个字节字模数据： 即像素24*50=1200。 1200/8=150字节。
					st1 = table_picture[b];																																																					  //得到的图片装换的 数组
					st2 = Table[150 * a + b];
					for(e = 0; e < 8; e++) {																																																					//逐个字节逐个位进行比较	
						s1 = st1 & (1 << e);
						s2 = st2 & (1 << e);
						if(s1 == s2) num1++;                                                                                                            //相同则增加
						if(s1 != s2) num1--;                                                                                                            //不同则减少
					}
		}
		if(num_save < num1) {
			num_save = num1;
			a_save = a;
		}
		
		LCD_ShowNum(50, 220, a, 2);			                                                                                                      	//显示匹配的字符是"a"			<调试用>
		if(num1 < 0) {
			LCD_ShowNum(70, 220, 0, 4);	                                                                                                   	  		//显示匹配的正确像素数       <调试用>
		} else {
		LCD_ShowNum(70, 220, num1, 4);	                                                                                                   			//显示匹配的正确像素数     <调试用> 
		}		
		LCD_ShowNum(120, 220, num_save, 4);                                                                                                   	//匹配的最大值显示         <调试用> 	
	}
	return a_save;
}


void CharacterRecognition(void)                                                                                                             //字符识别
{
	vu16 a = 0, b = 0, e = 0;
	vu16 i = 0, u = 0;
	vu8 Result = 0;                                                                    //识别结果
  vu8 temp[50] = {0}, temp1[50] = {0};

	for(b = Max_blue - 1; b > Min_blue; b--) {                                         //由右至左识别，获取各个字符的character_boundary_right,character_boundary_left值
		while(TableChangePoint_320[b] == 0) b--;                                         //取第1个字符
		character_boundary_right_1 = b + 1;                                              //+1 找到字符的左右边界
		while(TableChangePoint_320[b] > 0) b--;
		character_boundary_left_1 = b;                                                  //找到字符的左右边界
		if((character_boundary_right_1 - character_boundary_left_1) < 4) {							//省略低于三个像素的位置
			while(TableChangePoint_320[b] == 0) b--;																			// 重新找一个左右边界
			character_boundary_right_1 = b + 1;																					  //+1
			while(TableChangePoint_320[b] > 0) b--;
			character_boundary_left_1 = b;
		}
		while(TableChangePoint_320[b] == 0) b--;                                        //取第2个字符
		character_boundary_right_2 = b+1;
		while(TableChangePoint_320[b] > 0) b--;
		character_boundary_left_2 = b;
		if((character_boundary_right_2 - character_boundary_left_2) < 4) {             //省略低于3个像素的位置
			while(TableChangePoint_320[b] == 0) b--; 
			character_boundary_right_2 = b+1;                                            //+1
			while(TableChangePoint_320[b] > 0) b--;
			character_boundary_left_2 = b;
		}
		while(TableChangePoint_320[b] == 0) b--;                                         //取第3个字符
		character_boundary_right_3 = b+1;                                              //+1
		while(TableChangePoint_320[b] > 0) b--;
		character_boundary_left_3 = b;
		if((character_boundary_right_3 - character_boundary_left_3) < 4) {             //省略低于3个像素的位置
			while(TableChangePoint_320[b] == 0) b--; 
			character_boundary_right_3 = b+1;                                            //+1
			while(TableChangePoint_320[b] > 0) b--;
			character_boundary_left_3 = b;
		}
		while(TableChangePoint_320[b] == 0) b--;                                       //取第4个字符
		character_boundary_right_4 = b + 1;
		while(TableChangePoint_320[b] > 0) b--;
		character_boundary_left_4 = b;
		if((character_boundary_right_4 - character_boundary_left_4) < 4) {             //省略低于3个像素的位置
			while(TableChangePoint_320[b] == 0) b--; 
			character_boundary_right_4 = b + 1;                                          //+1
			while(TableChangePoint_320[b] > 0) b--;
			character_boundary_left_4 = b;
		}
		while(TableChangePoint_320[b] == 0) b--;                                       //取第5个字符
		character_boundary_right_5 = b + 1;                                            //+1
		while(TableChangePoint_320[b] > 0) b--;
		character_boundary_left_5 = b;
		if((character_boundary_right_5 - character_boundary_left_5) < 4) {             //省略低于3个像素的位置
			while(TableChangePoint_320[b] == 0) b--; 
			character_boundary_right_5 = b + 1;                                          //+1
			while(TableChangePoint_320[b] > 0) b--;
			character_boundary_left_5 = b;
		}
		while(TableChangePoint_320[b] == 0) b--;                                       //取第6个字符
		character_boundary_right_6 = b + 1;
		while(TableChangePoint_320[b]>0) b--;
		character_boundary_left_6 = b;
		while(TableChangePoint_320[b] == 0) b--;                                       //取第7个字符
		character_boundary_right_7 = b+1;//+1
		while(TableChangePoint_320[b] > 0) b--;
		character_boundary_left_7 = b;
		if((character_boundary_right_7 - character_boundary_left_7) < 4) {             //省略低于3个像素的位置
			while(TableChangePoint_320[b] == 0) b--; 
			character_boundary_right_7 = b + 1;                                          //+1
			while(TableChangePoint_320[b] > 0) b--;
			character_boundary_left_7 = b;
		}
		while(TableChangePoint_320[b] == 0) b--;                                       //取第8个字符
		character_boundary_right_8 = b + 1;
 		while(TableChangePoint_320[b] > 0) {
			if(b <= Min_blue) {
				break;
			}
			b--;
		}
		character_boundary_left_8 = b;
		b = Min_blue;                                                                  //以防万一，还满足for循环条件
	}
	for(a = Min_ChangePoint_240; a < Max_ChangePoint_240; a++) {                     //划线  将分割出的左右边界显示出来
		LCD_DrawPoint(character_boundary_right_1, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_1, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_right_2, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_2, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_right_3, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_3, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_right_4, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_4, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_right_5, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_5, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_right_6, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_6, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_right_7, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_7, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_right_8, a+1, 0x001f);
		LCD_DrawPoint(character_boundary_left_8, a+1, 0x001f);
	}
	//归一化处理：大小为24*50
	
	//第1个字符：
	Normalized(character_boundary_right_1, character_boundary_left_1);						  //归一化 24*50
	PictureToString();                                                              //图片->数组
	Result = MoShiShiBie_All(0, 36);                                                //字符匹配，模式识别,返回a,0<= a <36
	if(Result < 10) {
		LCD_ShowNum(240, 220, table_char[Result], 1);
	} else {
		LCD_ShowChar(240, 220, table_char[Result], 0);
	}
	table_cardMeasure[6] = Result;                                                  //保存识别的车牌结果
	
	//第2个字符：
	Normalized(character_boundary_right_2, character_boundary_left_2);						  //归一化 25*50
	PictureToString();                                                              //图片->数组
	Result = MoShiShiBie_All(0, 36);                                                //字符匹配，模式识别
	if(Result < 10) {
		LCD_ShowNum(230, 220, table_char[Result], 1);
	} else {
		LCD_ShowChar(230, 220, table_char[Result], 0);
	}
	table_cardMeasure[5] = Result;                                                  //保存识别的车牌结果
	
	Normalized(character_boundary_right_3, character_boundary_left_3);						  //归一化 25*50
	PictureToString();                                                              //图片->数组
	Result = MoShiShiBie_All(0, 36);                                                //字符匹配，模式识别
	if(Result < 10) {
		LCD_ShowNum(220,220,table_char[Result], 1);
	}
	else {
		LCD_ShowChar(220, 220, table_char[Result], 0);
	}
	table_cardMeasure[4] = Result;                                                  //保存识别的车牌结果
	
	Normalized(character_boundary_right_4, character_boundary_left_4);						  //归一化 25*50
	PictureToString();                                                              //图片->数组
	Result = MoShiShiBie_All(0, 36);                                                //字符匹配，模式识别
	if(Result < 10) {
		LCD_ShowNum(210,220,table_char[Result], 1);
	} else {
		LCD_ShowChar(210,220,table_char[Result], 0);
	}
	table_cardMeasure[3] = Result;                                                  //保存识别的车牌结果
	
	Normalized(character_boundary_right_5, character_boundary_left_5);						  //归一化 25*50
	PictureToString();                                                              //图片->数组
	Result = MoShiShiBie_All(0, 36);                                                //字符匹配，模式识别
	if(Result < 10) {
		LCD_ShowNum(200,220,table_char[Result],1);
	} else {
		LCD_ShowChar(200, 220, table_char[Result], 0);
	}
	table_cardMeasure[2] = Result;                                                  //保存识别的车牌结果
	
	LCD_ShowChar(190, 220, '.', 0);

	Normalized(character_boundary_right_7, character_boundary_left_7);						  //归一化 25*50
	PictureToString();                                                              //图片->数组
	Result = MoShiShiBie_All(10, 36);                                                //字符匹配，模式识别，只匹配字母
	if(Result < 10) {
		LCD_ShowNum(180, 220, table_char[Result], 1);
	} else {
		LCD_ShowChar(180, 220, table_char[Result], 0);
	}
	table_cardMeasure[1] = Result;                                                  //保存识别的车牌结果
	
	Normalized(character_boundary_right_8, character_boundary_left_8);						  //归一化 25*50					最后一个汉字
	PictureToString();                                                              //图片->数组
	Result = MoShiShiBie_All(36, 42);                                               //字符匹配，匹配汉字
	WordShow(Result - 35, 160, 220);                                                //显示汉字
	table_cardMeasure[0] = Result - 35;                                             //保存识别的车牌结果
	
	//识别结束
	//串口发送车牌信息    
	if(Result == 36) {
		sprintf((char*)temp, "识别结果：渝");
	} else if(Result == 37) {
		sprintf((char*)temp, "识别结果：闽");
	} else if(Result == 38) {
		sprintf((char*)temp, "识别结果：沪");
	} else if(Result == 39) {
		sprintf((char*)temp, "识别结果：浙");
	} else if(Result == 40) {
	sprintf((char*)temp, "识别结果：苏");
	} else if(Result == 41) {
		sprintf((char*)temp, "识别结果：粤");
	}
	
	sprintf((char*)temp1, "%c.%c%c%c%c%c\r\n"
		, table_char_char[table_cardMeasure[1]], table_char_char[table_cardMeasure[2]],
	    table_char_char[table_cardMeasure[3]], table_char_char[table_cardMeasure[4]],
    	table_char_char[table_cardMeasure[5]], table_char_char[table_cardMeasure[6]]);

	//先匹配已保存的车牌号
	for(u = 0; u < 5; u++) {
		for(i = 0; i < 7; i++) {
			if(table_card[u][i] != table_cardMeasure[i]) i = 8;                          //退出for循环
		}	
		if(i == 7) { 																																   //匹配成功
			LCD_Fill(0x00);																													     //黑屏
			Show_Title();																													       //显示标题
			Show_Card(u);																													       //显示第几组车牌			
			u = 5;
			while(GPIO_ReadInputDataBit(KEY1_PORT, KEY2_PIN) == 1);
		}
	}
	if(i == 9) {																																	   //无匹配车牌，则保存车牌
		i = 0;
		while(1) {
			if(GPIO_ReadInputDataBit(KEY1_PORT,KEY2_PIN) == 0) break;
			LCD_ShowNum(30,220, i / 100, 2);
			if(i == 300) {                        							           							//保存数据
				for(u = 0; u < 5; u++) {
					if(table_card[u][0] == 0) {
						for(i = 0;i < 7; i++) {
							table_card[u][i] = table_cardMeasure[i];
						}					
						u = 5;                                                                //退出循环
					}
				}
				LCD_Fill(0x00);                                                           //黑屏
				Show_Title();                                                             //显示标题
				Show_Card(0);                                                             //显示第几组车牌
				Show_Card(1);
				Show_Card(2);
				Show_Card(3);
				Show_Card(4);
				while(GPIO_ReadInputDataBit(KEY1_PORT,KEY2_PIN)==1);
				break;
			}
			delay_ms(1);
			i++;
		}
	}
}


