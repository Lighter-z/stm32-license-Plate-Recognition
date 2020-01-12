/*芯片引脚硬件配置*/
#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#define KEY1_PORT              GPIOA
#define KEY1_PIN               GPIO_Pin_8
#define KEY2_PIN               GPIO_Pin_12
#define KEY1_RCC			         RCC_APB2Periph_GPIOA


/* BEEP -> PD2		*/
#define BEEP_GPIO_PIN       GPIO_Pin_2
#define BEEP_GPIO_PORT      GPIOD
#define BEEP_RCC_PERIPH     RCC_APB2Periph_GPIOD	 
#define BEEP_ON             GPIO_WriteBit(BEEP_GPIO_PORT, BEEP_GPIO_PIN,1);	
#define BEEP_OFF            GPIO_WriteBit(BEEP_GPIO_PORT, BEEP_GPIO_PIN,0);			
/* LED1 -> PA13 */
#define LED1_GPIO_PIN       GPIO_Pin_13
#define LED1_GPIO_PORT      GPIOC
#define LED1_RCC_PERIPH     RCC_APB2Periph_GPIOC

//-----------------------------LCD_REST  = P1^1;待用
/* LCD CS -> PC9 */
#define LCD_CS_PORT         GPIOC
#define LCD_CS_PIN          GPIO_Pin_9
#define LCD_CS_RCC          RCC_APB2Periph_GPIOC

/* LCD RD -> PC10 */
#define LCD_RD_PORT         GPIOC
#define LCD_RD_PIN          GPIO_Pin_10
#define LCD_RD_RCC          RCC_APB2Periph_GPIOC

/* LCD WR -> PC11 */
#define LCD_WR_PORT         GPIOC
#define LCD_WR_PIN          GPIO_Pin_11
#define LCD_WR_RCC          RCC_APB2Periph_GPIOC

/* LCD RS -> PC12 */
#define LCD_RS_PORT         GPIOC
#define LCD_RS_PIN          GPIO_Pin_12
#define LCD_RS_RCC          RCC_APB2Periph_GPIOC

/* LCD REST -> PC8 */
#define LCD_REST_PORT       GPIOC
#define LCD_REST_PIN        GPIO_Pin_8
#define LCD_REST_RCC        RCC_APB2Periph_GPIOC
/* LCD DATA -> PB ALL */
#define LCD_DATA_PIN        GPIO_Pin_All
#define LCD_DATA_PORT       GPIOB
#define LCD_DATA_RCC        RCC_APB2Periph_GPIOB
//-----------------------------OV7670摄像头定义

#define OV7670_RRST_PORT              GPIOC
#define OV7670_RRST_PIN               GPIO_Pin_13
#define OV7670_RRST_RCC     					RCC_APB2Periph_GPIOC

#define FIFO_WR_PORT                  GPIOC
#define FIFO_WR_PIN                   GPIO_Pin_1
#define FIFO_WR_RCC     			    		RCC_APB2Periph_GPIOC

#define FIFO_RRST_PORT           	    GPIOC
#define FIFO_RRST_PIN                 GPIO_Pin_2
#define FIFO_RRST_RCC     				  	RCC_APB2Periph_GPIOC

#define FIFO_OE_PORT                  GPIOC
#define FIFO_OE_PIN           		    GPIO_Pin_3
#define FIFO_OE_RCC     							RCC_APB2Periph_GPIOC

#define FIFO_RCLK_PORT            	  GPIOC
#define FIFO_RCLK_PIN            		  GPIO_Pin_4
#define FIFO_RCLK_RCC     						RCC_APB2Periph_GPIOC

#define FIFO_WRST_PORT                GPIOC
#define FIFO_WRST_PIN                 GPIO_Pin_5
#define FIFO_WRST_RCC     				  	RCC_APB2Periph_GPIOC

#define SCCB_SIC_PORT                 GPIOC
#define SCCB_SIC_PIN                  GPIO_Pin_6
#define SCCB_SIC_RCC     	            RCC_APB2Periph_GPIOC

#define SCCB_SID_PORT                 GPIOC
#define SCCB_SID_PIN                  GPIO_Pin_7
#define SCCB_SID_RCC			            RCC_APB2Periph_GPIOC
//----------------------------PA8位并口数据
/* LCD DATA -> PB ALL */
#define OV7670_DATA_PIN        GPIO_Pin_All
#define OV7670_DATA_PORT       GPIOA
#define OV7670_DATA_RCC        RCC_APB2Periph_GPIOA


#ifdef __cplusplus
}
#endif

#endif 
