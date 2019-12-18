/**
  ******************************************************************************
  * @file    stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

#if (!defined LTK_FREERTOS && !defined LTK_UCOS)
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
   //ltk_increase_systick_timer();		//by me
}
#endif

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles WWDG interrupt request.
  * @param  None
  * @retval None
  */
void WWDG_IRQHandler(void)
{
}

/**
  * @brief  This function handles PVD interrupt request.
  * @param  None
  * @retval None
  */
void PVD_IRQHandler(void)
{
}

/**
  * @brief  This function handles Tamper interrupt request.
  * @param  None
  * @retval None
  */
void TAMPER_IRQHandler(void)
{
}

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
}

/**
  * @brief  This function handles Flash interrupt request.
  * @param  None
  * @retval None
  */
void FLASH_IRQHandler(void)
{
}

/**
  * @brief  This function handles RCC interrupt request.
  * @param  None
  * @retval None
  */
void RCC_IRQHandler(void)
{
}

/**
  * @brief  This function is the handler of exti interrupt.
  * @param  exti_line, exti line of interrupt
  * @retval None
  */
void exti_handler(uint32_t exti_line)
{
    #ifdef LTK_KEY_EXTI
    if(EXTI_GetITStatus(exti_line) != RESET)
    {
        /* Toggle LED0 */
        ltk_led_toggle(LTK_LED0);
        /* Clear the Key Button EXTI line pending bit */
        EXTI_ClearITPendingBit(exti_line);
    }
    #endif
    
    #ifdef LTK_IWDG
    if(EXTI_GetITStatus(exti_line) != RESET)
    {
        IWDG_ReloadCounter();
        EXTI_ClearITPendingBit(exti_line);
    }
    #endif
    
    #ifdef LTK_TOUCHSCREEN
    pen_state_t *pen_st;
    if(EXTI_GetITStatus(exti_line) != RESET)
    {       
        pen_st = ltk_get_penstate();
        pen_st->force_adjust = 1;
        EXTI_ClearITPendingBit(exti_line);
    }
    #endif
}

/**
  * @brief  This function handles External interrupt Line 0 request..
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
}

/**
  * @brief  This function handles External interrupt Line 1 request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
}

/**
  * @brief  This function handles External interrupt Line 2 request.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
    exti_handler(EXTI_Line2);
}

/**
  * @brief  This function handles External interrupt Line 3 request.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
    exti_handler(EXTI_Line3);
}

/**
  * @brief  This function handles External interrupt Line 4 request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Channel1 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel1_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Channel2 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel2_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Channel3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel3_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Channel4 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel4_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Channel5 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel5_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Channel6 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel6_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA1 Channel7 interrupt request.
  * @param  None
  * @retval None
  */
void DMA1_Channel7_IRQHandler(void)
{
}


/**
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval None
  */
void ADC1_2_IRQHandler(void)
{
    #ifdef LTK_ADC
    set_adc_value(ADC_GetConversionValue(ADC_NUM));
    #endif
}

/**
  * @brief  This function handles USB High Priority or CAN TX interrupts request.
  * @param  None
  * @retval None
  */
void USB_HP_CAN1_TX_IRQHandler(void)
{
    while(1)
    {
    }
}

/**
  * @brief  This function handles USB Low Priority or CAN RX0 interrupts request.
  * @param  None
  * @retval None
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    #ifdef LTK_USB
    USB_Istr();
    #endif
}

/**
  * @brief  This function handles CAN RX1 interrupt request.
  * @param  None
  * @retval None
  */
void CAN_RX1_IRQHandler(void)
{
}

/**
  * @brief  This function handles CAN SCE interrupt request.
  * @param  None
  * @retval None
  */
void CAN_SCE_IRQHandler(void)
{
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
    #ifdef LTK_TOUCHSCREEN
    pen_state_t *pen_st;
    #ifdef LTK_FREERTOS
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    #endif
    if(EXTI_GetITStatus(TS_PEN_EXTI_LINE) != RESET)
    {
        EXTI_ClearITPendingBit(TS_PEN_EXTI_LINE);
        pen_st = ltk_get_penstate();
        pen_st->pen_pressed = KEY_DOWN;
        #ifdef LTK_FREERTOS
        /* 'Give' the semaphore to unblock the task. */
        xSemaphoreGiveFromISR(binary_sem_ts, &xHigherPriorityTaskWoken);
    
        /* Giving the semaphore may have unblocked a task - if it did and the
        unblocked task has a priority equal to or above the currently executing
        task then xHigherPriorityTaskWoken will have been set to pdTRUE and
        portEND_SWITCHING_ISR() will force a context switch to the newly unblocked
        higher priority task.
    
        NOTE: The syntax for forcing a context switch within an ISR varies between
        FreeRTOS ports.  The portEND_SWITCHING_ISR() macro is provided as part of
        the Cortex M3 port layer for this purpose.  taskYIELD() must never be called
        from an ISR! */
        portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
        #endif
    }
    #endif
}

/**
  * @brief  This function handles TIM1 Break interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_BRK_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 overflow and update interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_UP_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 Trigger and commutation interrupts request.
  * @param  None
  * @retval None
  */
void TIM1_TRG_COM_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM1 capture compare interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_CC_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM4 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C1 Event interrupt request, tx, rx 
  *         buffer and number of bytes will be changed.
  * @param  None
  * @retval None
  */
void I2C1_EV_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C1 Error interrupt request.
  * @param  None
  * @retval None
  */
void I2C1_ER_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C2 Event interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_EV_IRQHandler(void)
{
}

/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_ER_IRQHandler(void)
{
}

/**
  * @brief  This function handles SPI1 global interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
}

/**
  * @brief  This function handles SPI2 global interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
}

/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
}

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
}

/**
  * @brief  This function handles USART3 global interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
}

/**
  * @brief  This function handles RTCAlarm interrupt request.
  * @param  None
  * @retval None
  */
void RTCAlarm_IRQHandler(void)
{
}

/**
  * @brief  This function handles USBWakeUp interrupt request.
  * @param  None
  * @retval None
  */
void USBWakeUp_IRQHandler(void)
{
    #ifdef LTK_USB
    EXTI_ClearITPendingBit(EXTI_Line18);
    #endif
}

/**
  * @brief  This function handles TIM8 Break interrupt request.
  * @param  None
  * @retval None
  */
void TIM8_BRK_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM8 overflow and update interrupt 
  * @param  None
  * @retval None
  */
void TIM8_UP_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM8 Trigger and commutation interrupts request.
  * @param  None
  * @retval None
  */
void TIM8_TRG_COM_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM8 capture compare interrupt request.
  * @param  None
  * @retval None
  */
void TIM8_CC_IRQHandler(void)
{
}

/**
  * @brief  This function handles ADC3 global interrupt request.
  * @param  None
  * @retval None
  */
void ADC3_IRQHandler(void)
{
}

/**
  * @brief  This function handles FSMC global interrupt request.
  * @param  None
  * @retval None
  */
void FSMC_IRQHandler(void)
{
}

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
}

/**
  * @brief  This function handles SPI3 global interrupt request.
  * @param  None
  * @retval None
  */
void SPI3_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART4 global interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
}

/**
  * @brief  This function handles UART5 global interrupt request.
  * @param  None
  * @retval None
  */
void UART5_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM6 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM6_IRQHandler(void)
{
}

/**
  * @brief  This function handles TIM7 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM7_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA2 Channel 1 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Channel1_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA2 Channel2 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Channel2_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA2 Channel3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Channel3_IRQHandler(void)
{
}

/**
  * @brief  This function handles DMA2 Channel 4 and DMA2 Channel 5
  * @param  None
  * @retval None
  */
void DMA2_Channel4_5_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
