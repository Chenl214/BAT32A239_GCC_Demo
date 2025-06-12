/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    tim4_user.c
* @brief   This file implements device driver for Timer 4 module.
* @version 1.0.0
* @date    2020/10/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "BAT32A239.h"
#include "tim8.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//void IRQ17_Handler(void) __attribute__((alias("tm81_channel10_14_interrupt")));
//void IRQ18_Handler(void) __attribute__((alias("tm81_channel11_15_interrupt")));
//void IRQ19_Handler(void) __attribute__((alias("tm81_channel12_16_interrupt")));
//void IRQ20_Handler(void) __attribute__((alias("tm81_channel13_17_interrupt")));
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* For TM81 pulse measurement */
volatile uint32_t g_tm81_ch0_width = 0UL;
volatile uint32_t g_tm81_ch1_width = 0UL;
volatile uint32_t g_tm81_ch2_width = 0UL;
volatile uint32_t g_tm81_ch3_width = 0UL;
volatile uint32_t g_tm81_ch4_width = 0UL;
volatile uint32_t g_tm81_ch5_width = 0UL;
volatile uint32_t g_tm81_ch6_width = 0UL;
volatile uint32_t g_tm81_ch7_width = 0UL;
volatile uint32_t g_inttm10Taken = 0UL;
volatile uint32_t g_inttm11Taken = 0UL;
volatile uint32_t g_inttm12Taken = 0UL;
volatile uint32_t g_inttm13Taken = 0UL;
volatile uint32_t g_inttm14Taken = 0UL;
volatile uint32_t g_inttm15Taken = 0UL;
volatile uint32_t g_inttm16Taken = 0UL;
volatile uint32_t g_inttm17Taken = 0UL;
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: tm81_channel10_interrupt
* @brief  This function INTTM10 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel10_interrupt(void)
{
	  
      INTC_ClearPendingIRQ(TM10_IRQn);    /* clear INTTM10 interrupt flag */
      g_inttm10Taken++;
      if (1U == (TM81->TSR10 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch0_width = (uint32_t)(TM81->TDR10 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch0_width = (uint32_t)(TM81->TDR10 + 1UL);
      }
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tm81_channel14_interrupt
* @brief  This function INTTM14 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel14_interrupt(void)
{
	  
		  INTC_ClearPendingIRQ(TM14_IRQn);    /* clear INTTM14 interrupt flag */
      g_inttm14Taken++;
      if (1U == (TM81->TSR14 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch4_width = (uint32_t)(TM81->TDR14 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch4_width = (uint32_t)(TM81->TDR14 + 1UL);
      }	
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tm81_channel11_interrupt
* @brief  This function INTTM11 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel11_interrupt(void)
{
	  
      INTC_ClearPendingIRQ(TM11_IRQn);    /* clear INTTM11 interrupt flag */
      g_inttm11Taken++;
      if (1U == (TM81->TSR11 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch1_width = (uint32_t)(TM81->TDR11 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch1_width = (uint32_t)(TM81->TDR11 + 1UL);
      }
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tm81_channel15_interrupt
* @brief  This function INTTM15 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel15_interrupt(void)
{
	  
      INTC_ClearPendingIRQ(TM15_IRQn);    /* clear INTTM15 interrupt flag */
      g_inttm15Taken++;
      if (1U == (TM81->TSR15 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch5_width = (uint32_t)(TM81->TDR15 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch5_width = (uint32_t)(TM81->TDR15 + 1UL);
      }
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tm81_channel12_interrupt
* @brief  This function INTTM12 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel12_interrupt(void)
{
	  
	    INTC_ClearPendingIRQ(TM12_IRQn);    /* clear INTTM12 interrupt flag */
      g_inttm12Taken++;
      if (1U == (TM81->TSR12 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch2_width = (uint32_t)(TM81->TDR12 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch2_width = (uint32_t)(TM81->TDR12 + 1UL);
      }
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tm81_channel16_interrupt
* @brief  This function INTTM16 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel16_interrupt(void)
{
	 
      INTC_ClearPendingIRQ(TM16_IRQn);    /* clear INTTM13 interrupt flag */
      g_inttm16Taken++;
      if (1U == (TM81->TSR16 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch6_width = (uint32_t)(TM81->TDR16 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch6_width = (uint32_t)(TM81->TDR16 + 1UL);
      }			
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tm81_channel13_interrupt
* @brief  This function INTTM13 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel13_interrupt(void)
{
	  
	    INTC_ClearPendingIRQ(TM13_IRQn);    /* clear INTTM13 interrupt flag */
      g_inttm13Taken++;
      if (1U == (TM81->TSR13 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch3_width = (uint32_t)(TM81->TDR13 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch3_width = (uint32_t)(TM81->TDR13 + 1UL);
      }
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: tm81_channel17_interrupt
* @brief  This function INTTM17 interrupt service routine.
* @param  None
* @return None
***********************************************************************************************************************/
void tm81_channel17_interrupt(void)
{
	  
      INTC_ClearPendingIRQ(TM17_IRQn);    /* clear INTTM13 interrupt flag */
      g_inttm17Taken++;
      if (1U == (TM81->TSR17 & _0001_TM8_OVERFLOW_OCCURS))    /* overflow occurs */
      {
          g_tm81_ch7_width = (uint32_t)(TM81->TDR17 + 1UL) + 0x10000UL;
      }
      else
      {
          g_tm81_ch7_width = (uint32_t)(TM81->TDR17 + 1UL);
      }			
		
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
