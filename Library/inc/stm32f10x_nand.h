/**
  ******************************************************************************
  * @file    stm32f10x_nand.h
  * @author  leitek (leitek.taobao.com)
  * @version V1.0.0
  * @brief   stm32f10x fsmc nand driver.
  ******************************************************************************
  * @copy
  *
  * For non-commercial research and private study only.
  *
  * COPYRIGHT leitek.taobao.com
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_NAND_H
#define __STM32F10x_NAND_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define NAND1_FSMC_BANK2            ((uint32_t)0x70000000)
#define NAND2_FSMC_BANK3            ((uint32_t)0x80000000)
#define SRAM3_FSMC_BANK1            ((uint32_t)0x68000000)     

/** 
  * @brief  NAND Area definition  
  */  
#define NAND_CMD_AREA               ((uint32_t)(1<<16))      /* A16 = CLE high */
#define NAND_ADDR_AREA              ((uint32_t)(1<<17))      /* A17 = ALE high */
#define NAND_DATA_AREA              ((uint32_t)0x00000000) 

/** 
  * @brief  FSMC NAND memory command  
  */  
#define NAND_CMD_AREA_A            ((uint8_t)0x00)
#define NAND_CMD_AREA_B            ((uint8_t)0x01)
#define NAND_CMD_AREA_C            ((uint8_t)0x50)

#define NAND_CMD_WRITE0            ((uint8_t)0x80)
#define NAND_CMD_WRITE_TRUE1       ((uint8_t)0x10)

#define NAND_CMD_ERASE_1ST_CYCLE   ((uint8_t)0x60)
#define NAND_CMD_ERASE_2ND_CYCLE   ((uint8_t)0xD0)

#define NAND_CMD_READID            ((uint8_t)0x90)
#define NAND_CMD_STATUS            ((uint8_t)0x70)
#define NAND_CMD_LOCK_STATUS       ((uint8_t)0x7A)
#define NAND_CMD_RESET             ((uint8_t)0xFF)

#define NAND_VALID_ADDRESS         ((uint32_t)0x00000100)
#define NAND_INVALID_ADDRESS       ((uint32_t)0x00000200)
#define NAND_TIMEOUT_ERROR         ((uint32_t)0x00000400)
#define NAND_BUSY                  ((uint32_t)0x00000000)
#define NAND_ERROR                 ((uint32_t)0x00000001)
#define NAND_READY                 ((uint32_t)0x00000040)

void NAND_send_cmd(uint8_t nand_cmd);
void NAND_send_addr(uint32_t nand_addr);
void NAND_send_3_addr(uint32_t nand_addr);
uint8_t NAND_read_data(void);
void NAND_write_data(uint8_t NAND_data);
uint32_t NAND_GetStatus(void);

#ifdef __cplusplus
}
#endif

#endif /*__STM32F10x_NAND_H */

/****************************** leitek.taobao.com *****************************/
