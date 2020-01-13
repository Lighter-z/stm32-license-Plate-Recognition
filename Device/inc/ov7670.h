#ifndef __OV7670_H
#define __OV7670_H
	 
#ifdef __cplusplus
 extern "C" {
#endif 
	 
void OV7670_GPIO_Init(void);

void FIFO_Reset_Read_Addr(void);//FIFO¶ÁÊý¾Ý¸´Î»

unsigned char Sensor_Init(void);
	 
#ifdef __cplusplus
}
#endif


#endif 
