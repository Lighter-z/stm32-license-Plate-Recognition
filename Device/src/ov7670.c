#include "stm32f10x.h"
#include "board.h"
#include "ov7670.h" 

#define OV7670_REG_NUM  184

#define DELAYTIME 2//9倍频时延时“1”，16倍频时延时“2”

void OV7670_GPIO_Init(void)
{
  GPIO_InitTypeDef gpio_init_struct;//结构体
		
	RCC_APB2PeriphClockCmd(FIFO_WR_RCC, ENABLE);//初始化时钟
	RCC_APB2PeriphClockCmd(FIFO_RRST_RCC, ENABLE);//初始化时钟
	RCC_APB2PeriphClockCmd(FIFO_OE_RCC, ENABLE);//初始化时钟
	RCC_APB2PeriphClockCmd(FIFO_RCLK_RCC, ENABLE);//初始化时钟
	RCC_APB2PeriphClockCmd(FIFO_WRST_RCC, ENABLE);//初始化时钟
	RCC_APB2PeriphClockCmd(SCCB_SIC_RCC, ENABLE);//初始化时钟
	RCC_APB2PeriphClockCmd(SCCB_SID_RCC, ENABLE);//初始化时钟
	RCC_APB2PeriphClockCmd(OV7670_RRST_RCC, ENABLE);//初始化时钟
	
	gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;

	gpio_init_struct.GPIO_Pin = FIFO_WR_PIN;
	GPIO_Init(FIFO_WR_PORT, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin = FIFO_RRST_PIN;
	GPIO_Init(FIFO_RRST_PORT, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin = FIFO_OE_PIN;
	GPIO_Init(FIFO_OE_PORT, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin = FIFO_RCLK_PIN;
	GPIO_Init(FIFO_RCLK_PORT, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin = FIFO_WRST_PIN;
	GPIO_Init(FIFO_WRST_PORT, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin = SCCB_SIC_PIN;
	GPIO_Init(SCCB_SIC_PORT, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin = SCCB_SID_PIN;
	GPIO_Init(SCCB_SID_PORT, &gpio_init_struct);

	gpio_init_struct.GPIO_Pin = OV7670_RRST_PIN;
	GPIO_Init(OV7670_RRST_PORT, &gpio_init_struct);
	
	RCC_APB2PeriphClockCmd(OV7670_DATA_RCC, ENABLE);//初始化时钟
	
	gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;//输入上拉
	
	gpio_init_struct.GPIO_Pin = OV7670_DATA_PIN;//OV7670数据口引脚初始化
	GPIO_Init(OV7670_DATA_PORT, &gpio_init_struct);
	
	GPIO_WriteBit(FIFO_OE_PORT, FIFO_OE_PIN, 0);
}
static void Sccb_Sid_Change_In(void)//引脚切换为输入
{
	GPIO_InitTypeDef gpio_init_struct;//结构体

	gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;//输入上拉
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;

	gpio_init_struct.GPIO_Pin = SCCB_SID_PIN;
	GPIO_Init(SCCB_SID_PORT, &gpio_init_struct);
}
static void Sccb_Sid_Change_Out(void)//引脚切换为输出
{
	GPIO_InitTypeDef gpio_init_struct;//结构体
	
	gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_Pin = SCCB_SID_PIN;
	GPIO_Init(SCCB_SID_PORT, &gpio_init_struct);
}
void FIFO_Reset_Read_Addr(void)//FIFO 读数据复位，通过直接操作寄存器来提高速度
{				
	GPIOC->BRR =1<<2;	//	RRST=0
	GPIOC->BRR =1<<4;	//	RCLK= 0
	GPIOC->BSRR =1<<4;	//  RCLK=1
	GPIOC->BRR =1<<4;	//	RCLK=0
	GPIOC->BSRR =1<<2;	//  RRST=1
	GPIOC->BSRR =1<<4;	//	RCLK=1
}
static void Start_Sccb(void)//SCCB设置，类似I2C
{
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 1);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 1);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 0);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 0);
	delay(DELAYTIME);
}

static void Stop_Sccb(void)//stop命令,SCCB的停止信号
{
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 0);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 1);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 1);
	delay(DELAYTIME);  
}
static void No_Ack(void)//noAck,用于连续读取中的最后一个结束周期
{
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 1);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 1);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 0);
	delay(DELAYTIME);
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 0);
	delay(DELAYTIME);
}
static u8 Get_Ack(void) 
{
	u8 Error;

	Sccb_Sid_Change_In();																			
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 1);
	delay(DELAYTIME);

	GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 1);
	delay(DELAYTIME);

	Error= GPIO_ReadInputDataBit(SCCB_SID_PORT, SCCB_SID_PIN);
	delay(DELAYTIME);

	GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 0);
	delay(DELAYTIME);

	Sccb_Sid_Change_Out();//输出
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 0);

	return !Error;
}

static u8 Sccb_Write_Byte(u8 dat)//写入一个字节的数据到SCCB
{
	u8 i;
	for(i=0;i<8;i++)
	{
		GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, (((dat<<i)&0x80))>>7);
		delay(DELAYTIME);

		GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 1);
		delay(DELAYTIME);

		GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 0);	 
		delay(DELAYTIME);
	}
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 0);

	return Get_Ack();
}
static u8 Sccb_Read_Byte(void)//一个字节数据读取并且返回
{
	u8 i,rbyte=0;

	Sccb_Sid_Change_In();
	for(i=0;i<8;i++)
	{
		GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 1);
		delay(DELAYTIME);

		if(GPIO_ReadInputDataBit(SCCB_SID_PORT, SCCB_SID_PIN)) rbyte|=(0x80>>i);
		delay(DELAYTIME);

		GPIO_WriteBit(SCCB_SIC_PORT, SCCB_SIC_PIN, 0);	 
		delay(DELAYTIME);
	} 

	Sccb_Sid_Change_Out();//输出
	GPIO_WriteBit(SCCB_SID_PORT, SCCB_SID_PIN, 0);
	return rbyte;
}

uc8 OV7670_reg[OV7670_REG_NUM][2]=//OV7670 传感器寄存器、初始化相关设置
{	 
// 		/*以下为OV7670 QVGA RGB565参数  */
// 		{0x3a, 0x04},//dummy
// 		{0x40, 0x10},//565
// 		{0x12, 0x14},//@QVGA=0x14;VGA=0x04?
// 		{0x32, 0x80},//HREF control	bit[2:0] HREF start 3 LSB	 bit[5:3] HSTOP HREF end 3LSB
// 		{0x17, 0x16},//              HSTART start high 8-bit MSB
// 		{0x18, 0x04},//5	 HSTOP end high 8-bit
// 		{0x19, 0x02},
// 		{0x1a, 0x7b},//0x7a,
// 		{0x03, 0x06},//0x0a,
// 		{0x0c, 0x0c},
// 		{0x15, 0x02},
// 		{0x3e, 0x00},//10
// 		{0x70, 0x00},
// 		{0x71, 0x01},
// 		{0x72, 0x11},
// 		{0x73, 0x09},//
// 		{0xa2, 0x02},//15
// 		{0x11, 0x00}, //fenpin--03（普通）--00(较快)--80（花了）---分频(me)
// 		{0x7a, 0x20},
// 		{0x7b, 0x1c},
// 		{0x7c, 0x28},
// 		{0x7d, 0x3c},//20
// 		{0x7e, 0x55},
// 		{0x7f, 0x68},
// 		{0x80, 0x76},
// 		{0x81, 0x80},
// 		{0x82, 0x88},
// 		{0x83, 0x8f},
// 		{0x84, 0x96},
// 		{0x85, 0xa3},
// 		{0x86, 0xaf},
// 		{0x87, 0xc4},//30
// 		{0x88, 0xd7},
// 		{0x89, 0xe8},
// 		{0x13, 0xe0},
// 		{0x00, 0x00},//AGC
// 		{0x10, 0x00},
// 		{0x0d, 0x00},
// 		{0x14, 0x20},//0x38, limit the max gain
// 		{0xa5, 0x05},
// 		{0xab, 0x07},
// 		{0x24, 0x75},//40
// 		{0x25, 0x63},
// 		{0x26, 0xA5},
// 		{0x9f, 0x78},
// 		{0xa0, 0x68},
// 		{0xa1, 0x03},//0x0b,
// 		{0xa6, 0xdf},//0xd8,
// 		{0xa7, 0xdf},//0xd8,
// 		{0xa8, 0xf0},
// 		{0xa9, 0x90},
// 		{0xaa, 0x94},//50
// 		{0x13, 0xe5},
// 		{0x0e, 0x61},
// 		{0x0f, 0x4b},
// 		{0x16, 0x02},
// 		{0x1e, 0x37},//0x07,
// 		{0x21, 0x02},
// 		{0x22, 0x91},
// 		{0x29, 0x07},
// 		{0x33, 0x0b},
// 		{0x35, 0x0b},//60
// 		{0x37, 0x1d},
// 		{0x38, 0x71},
// 		{0x39, 0x2a},
// 		{0x3c, 0x78},
// 		{0x4d, 0x40},
// 		{0x4e, 0x20},
// 		{0x69, 0x5d},
// 		{0x6b, 0x40},//PLL
// 		{0x74, 0x19},
// 		{0x8d, 0x4f},
// 		{0x8e, 0x00},//70
// 		{0x8f, 0x00},
// 		{0x90, 0x00},
// 		{0x91, 0x00},
// 		{0x92, 0x00},//0x19,//0x66
// 		{0x96, 0x00},
// 		{0x9a, 0x80},
// 		{0xb0, 0x84},
// 		{0xb1, 0x0c},
// 		{0xb2, 0x0e},
// 		{0xb3, 0x82},//80
// 		{0xb8, 0x0a},
// 		{0x43, 0x14},
// 		{0x44, 0xf0},
// 		{0x45, 0x34},
// 		{0x46, 0x58},
// 		{0x47, 0x28},
// 		{0x48, 0x3a},
// 		{0x59, 0x88},
// 		{0x5a, 0x88},
// 		{0x5b, 0x44},//90
// 		{0x5c, 0x67},
// 		{0x5d, 0x49},
// 		{0x5e, 0x0e},
// 		{0x64, 0x04},
// 		{0x65, 0x20},
// 		{0x66, 0x05},
// 		{0x94, 0x04},
// 		{0x95, 0x08},
// 		{0x6c, 0x0a},
// 		{0x6d, 0x55},
// 		{0x4f, 0x80},
// 		{0x50, 0x80},
// 		{0x51, 0x00},
// 		{0x52, 0x22},
// 		{0x53, 0x5e},
// 		{0x54, 0x80},
// 		//{0x54, 0x40},//110
// 		{0x6e, 0x11},//100
// 		//{0x13, 0xe7},																	//自己加的--me//AWB、AGC、AGC Enable and ...
// 		{0x6f, 0x9f},//0x9e for advance AWB												9f
// 		{0x55, 0x00},//亮度																0x00
// 		{0x56, 0x50},//对比度													初始值：0x20
// 		{0x57, 0x40},//0x40,  change according to Jim's request	      
{0x3a, 0x04},//dummy
{0x40, 0x10},//565   
{0x12, 0x14},//QVGA,RGB

{0x32, 0x80},//HREF control	bit[2:0] HREF start 3 LSB	bit[5:3] HSTOP HREF end 3LSB
{0x17, 0x16},//HSTART start high 8-bit MSB         
{0x18, 0x04},//5 HSTOP end high 8-bit
{0x19, 0x02},
{0x1a, 0x7b},//0x7a,
 	{0x03, 0x06},//0x0a,???ú?±·??ò????

{0x0c, 0x0c},
{0x15, 0x02},//0x00
{0x3e, 0x00},//10
{0x70, 0x00},
{0x71, 0x01},
{0x72, 0x11},
{0x73, 0x09},//
        
{0xa2, 0x02},//15
{0x11, 0x00},//
{0x7a, 0x20},
{0x7b, 0x1c},
{0x7c, 0x28},
        
{0x7d, 0x3c},//20
{0x7e, 0x55},
{0x7f, 0x68},
{0x80, 0x76},
{0x81, 0x80},
        
{0x82, 0x88},
{0x83, 0x8f},
{0x84, 0x96},
{0x85, 0xa3},
{0x86, 0xaf},
        
{0x87, 0xc4},//30
{0x88, 0xd7},
{0x89, 0xe8},
{0x13, 0xe0},
{0x00, 0x00},//AGC
        
{0x10, 0x00},
{0x0d, 0x00},//
{0x14, 0x20},//0x38, limit the max gain
{0xa5, 0x05},
{0xab, 0x07},
        
{0x24, 0x75},//40
{0x25, 0x63},
{0x26, 0xA5},
{0x9f, 0x78},
{0xa0, 0x68},
        
{0xa1, 0x03},//0x0b,
{0xa6, 0xdf},//0xd8,
{0xa7, 0xdf},//0xd8,
{0xa8, 0xf0},
{0xa9, 0x90},
        
{0xaa, 0x94},//50
{0x13, 0xe5},
{0x0e, 0x61},
{0x0f, 0x4b},
{0x16, 0x02},
        
{0x1e, 0x37},//
{0x21, 0x02},
{0x22, 0x91},
{0x29, 0x07},
{0x33, 0x0b},
        
{0x35, 0x0b},//60
{0x37, 0x1d},
{0x38, 0x71},
{0x39, 0x2a},
{0x3c, 0x78},
        
{0x4d, 0x40},
{0x4e, 0x20},
{0x69, 0x5d},
{0x6b, 0x40},//PLL*4=48Mhz
{0x74, 0x19},
{0x8d, 0x4f},
        
{0x8e, 0x00},//70
{0x8f, 0x00},
{0x90, 0x00},
{0x91, 0x00},
{0x92, 0x00},//0x19,//0x66
        
{0x96, 0x00},
{0x9a, 0x80},
{0xb0, 0x84},
{0xb1, 0x0c},
{0xb2, 0x0e},
        
{0xb3, 0x82},//80
{0xb8, 0x0a},
{0x43, 0x14},
{0x44, 0xf0},
{0x45, 0x34},
        
{0x46, 0x58},
{0x47, 0x28},
{0x48, 0x3a},
{0x59, 0x88},
{0x5a, 0x88},
        
{0x5b, 0x44},//90
{0x5c, 0x67},
{0x5d, 0x49},
{0x5e, 0x0e},
{0x64, 0x04},
{0x65, 0x20},
        
{0x66, 0x05},
{0x94, 0x04},
{0x95, 0x08},
{0x6c, 0x0a},
{0x6d, 0x55},
        
        
{0x4f, 0x80},
{0x50, 0x80},
{0x51, 0x00},
{0x52, 0x22},
{0x53, 0x5e},
{0x54, 0x80},//偏色设置，偏绿，偏蓝。。。初始值0x80,车牌用0xA0(160)

{0x09, 0x03},

{0x6e, 0x11},//100
{0x6f, 0x9e},//0x9e for advance AWB
  {0x55, 0x20},//亮度
  {0x56, 0x50},//对比度
  {0x57, 0x40},//0x40,  change according to Jim's request 

{0x6a, 0x40},//--下面部分寄存器的设置，有自动白平衡的功能--me
{0x01, 0x40},
{0x02, 0x40},
{0x13, 0xe7},
{0x15, 0x02},  


{0x58, 0x9e},

{0x41, 0x08},
{0x3f, 0x00},
{0x75, 0x05},
{0x76, 0xe1},
{0x4c, 0x00},
{0x77, 0x01},
{0x3d, 0xc2}, 
{0x4b, 0x09},
{0xc9, 0x60},
{0x41, 0x38},

{0x34, 0x11},
{0x3b, 0x02}, 

{0xa4, 0x89},
{0x96, 0x00},
{0x97, 0x30},
{0x98, 0x20},
{0x99, 0x30},
{0x9a, 0x84},
{0x9b, 0x29},
{0x9c, 0x03},
{0x9d, 0x4c},
{0x9e, 0x3f},
{0x78, 0x04},

{0x79, 0x01},
{0xc8, 0xf0},
{0x79, 0x0f},
{0xc8, 0x00},
{0x79, 0x10},
{0xc8, 0x7e},
{0x79, 0x0a},
{0xc8, 0x80},
{0x79, 0x0b},
{0xc8, 0x01},
{0x79, 0x0c},
{0xc8, 0x0f},
{0x79, 0x0d},
{0xc8, 0x20},
{0x79, 0x09},
{0xc8, 0x80},
{0x79, 0x02},
{0xc8, 0xc0},
{0x79, 0x03},
{0xc8, 0x40},
{0x79, 0x05},
{0xc8, 0x30},
{0x79, 0x26}, 
{0x09, 0x00}, 
};

static u8 Write_Sensor_Reg(u8 regID, u8 regDat)//写OV7670寄存器
{
		Start_Sccb(); //发送SCCB 总线开始传输命令
		if(0 == Sccb_Write_Byte(0x42))//写地址
		{	
			Stop_Sccb();//发送SCCB 总线停止传输命令
			return(0);//错误返回
		}
		// UART_Put_Inf("wr_Sensor_Reg:",1);
		if(0 == Sccb_Write_Byte(regID))//积存器ID
		{
			Stop_Sccb();//发送SCCB 总线停止传输命令
			return(0); //错误返回
		}
		// UART_Put_Inf("wr_Sensor_Reg:",2);
		if(0 == Sccb_Write_Byte(regDat))//写数据到积存器
		{
			Stop_Sccb();//发送SCCB 总线停止传输命令
			return(0);//错误返回
		}
		Stop_Sccb();//发送SCCB 总线停止传输命令
		// UART_Put_Inf("wr_Sensor_Reg:",3);
		return(1);//成功返回
}

static u8 Readd_Sensor_Reg(u8 regID, u8 *regDat)//读OV7670寄存器
{
		//通过写操作设置寄存器地址
		Start_Sccb();
		if(0 == Sccb_Write_Byte(0x42))//写地址
		{
			Stop_Sccb();//发送SCCB 总线停止传输命令
			return(0);//错误返回
		}
		if(0 == Sccb_Write_Byte(regID))//积存器ID
		{
			Stop_Sccb();//发送SCCB 总线停止传输命令
			return(0);//错误返回
		}
		Stop_Sccb();//发送SCCB 总线停止传输命令

		//设置寄存器地址后，才是读
		Start_Sccb();
		if(0 == Sccb_Write_Byte(0x43))//读地址
		{
			Stop_Sccb();//发送SCCB 总线停止传输命令
			return(0);//错误返回
		}
		*regDat = Sccb_Read_Byte();//返回读到的值
		No_Ack();//发送NACK命令
		Stop_Sccb();//发送SCCB 总线停止传输命令
		return(1);//成功返回
}
u8 Sensor_Init(void)//摄像头芯片初始化
{
		u8 temp;	
		u8 i = 0;

		temp = 0x80;
		if(0 == Write_Sensor_Reg(0x12,temp)) //Reset SCCB
		{
			return 0 ;//错误返回
		}							
		//printf("Sensor_init:%d\n\r",1);
		if(0 == Readd_Sensor_Reg(0x0b, &temp))//读ID
		{
			return 0 ;//错误返回
		}									
		//printf("Sensor_init:%d\n\r",temp);
		if(temp == 0x73)//OV7670
		{																	
			for(i=0;i<OV7670_REG_NUM;i++)
			{
				//printf("Sensor_init:cycle%d\n\r",i);
				if(0 == Write_Sensor_Reg(OV7670_reg[i][0],OV7670_reg[i][1]))
				{																															
					return 0;//错误返回
				}
			}
		}
		//printf("Sensor_init OK! %d\n\r",0);
		return 1; //ok
} 

