/**
  ******************************************************************************
  * @file    stm32f10x_nand.c
  * @author  leitek (leitek.taobao.com)
  * @version V1.0.0
  * @brief   LTK middleware. 
  *          This file provides FSMC NAND driver.
  ******************************************************************************
  * @copy
  *
  * For non-commercial research and private study only.
  *
  * COPYRIGHT leitek.taobao.com
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_fsmc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_nand.h"

/**
  * @brief  send cmd to nand.
  * @param  nand_cmd: the nand command to be sent.  
  * @retval None
  */
void NAND_send_cmd(uint8_t nand_cmd)
{
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_CMD_AREA) = nand_cmd;
}

/**
  * @brief  send address to nand.
  * @param  nand_addr: the nand address to be sent.  
  * @retval None
  */
void NAND_send_addr(uint32_t nand_addr)
{
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_ADDR_AREA) = (uint8_t)(0x00); 
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_ADDR_AREA) = (uint8_t)((nand_addr >>  0) & 0xFF);
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_ADDR_AREA) = (uint8_t)((nand_addr >>  8) & 0xFF);
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_ADDR_AREA) = (uint8_t)((nand_addr >> 16) & 0xFF);
}

/**
  * @brief  send address to nand, for Block erase only.
  * @param  nand_addr: the nand address to be sent.  
  * @retval None
  */
void NAND_send_3_addr(uint32_t nand_addr)
{
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_ADDR_AREA) = (uint8_t)((nand_addr >>  0) & 0xFF);
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_ADDR_AREA) = (uint8_t)((nand_addr >>  8) & 0xFF);
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_ADDR_AREA) = (uint8_t)((nand_addr >> 16) & 0xFF);
}

/**
  * @brief  read data from nand.
  * @param  None
  * @retval the data read from nand
  */
uint8_t NAND_read_data(void)
{
    return *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_DATA_AREA);
}

/**
  * @brief  write data to nand.
  * @param  data to be written
  * @retval None
  */
void NAND_write_data(uint8_t NAND_data)
{
    *(__IO uint8_t *)(NAND1_FSMC_BANK2 | NAND_DATA_AREA) = NAND_data;
}


/**
  * @brief  Reads the NAND memory status using the Read status command. 
  * @param  None
  * @retval The status of the NAND memory. This parameter can be:
  *             - NAND_BUSY: when memory is busy
  *             - NAND_READY: when memory is ready for the next operation
  *             - NAND_ERROR: when the previous operation gererates error
  */
uint32_t NAND_Read_status(void)
{
    uint8_t data = 0x00;
    uint32_t status = NAND_BUSY;
    NAND_send_cmd(NAND_CMD_STATUS);

    data = NAND_read_data();

    if((data & NAND_ERROR) == NAND_ERROR)
    {
        status = NAND_ERROR;
    } 
    else if((data & NAND_READY) == NAND_READY)
    {
        status = NAND_READY;
    }
    else
    {
        status = NAND_BUSY; 
    }

    return (status);
}

/**
  * @brief  Get the NAND operation status.
  * @param  None
  * @retval New status of the NAND operation. This parameter can be:
  *             - NAND_TIMEOUT_ERROR: when the previous operation generate
  *               a Timeout error
  *             - NAND_READY: when memory is ready for the next operation
  */
uint32_t NAND_GetStatus(void)
{
    uint32_t timeout = 0x1000000;
    uint32_t status = NAND_READY;

    status = NAND_Read_status();

    /*!< Wait for a NAND operation to complete or a TIMEOUT to occur */
    while ((status != NAND_READY) &&(timeout != 0x00))
    {
        status = NAND_Read_status();
        timeout --;
    }

    if(timeout == 0x00)
    {
        status =  NAND_TIMEOUT_ERROR;
    }

    /*!< Return the operation status */
    return (status);
}



/****************************** leitek.taobao.com *****************************/
