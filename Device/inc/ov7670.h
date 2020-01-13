#ifndef __OV7670_H
#define __OV7670_H
	 
#ifdef __cplusplus
 extern "C" {
#endif 
	 
void OV7670_Gpio_Init();
void SCCB_SID_change_in();//引脚切换为输入
void SCCB_SID_change_out();//引脚切换为输出

void FIFO_Reset_Read_Addr(void);//FIFO读数据复位
	 
void startSCCB(void);
void stopSCCB(void);
void noAck(void);
unsigned char getAck();
unsigned char SCCBwriteByte(unsigned char dat);
unsigned char SCCBreadByte(void);
	 
void FIFO_Reset_Read_Addr(void);

unsigned char wr_Sensor_Reg(unsigned char regID, unsigned char regDat);
unsigned char rd_Sensor_Reg(unsigned char regID, unsigned char *regDat);
unsigned char Sensor_init(void);
	 
#ifdef __cplusplus
}
#endif


#endif 
