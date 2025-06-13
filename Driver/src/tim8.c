/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    tim4.c
* @brief   This file implements device driver for TM8 module.
* @version 1.0.0
* @date    2020/10/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "BAT32A239.h"
#include "userdefine.h"
#include "tim8.h"
#include "gpio.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name: TM81_IntervalTimer
* @brief  TM81 specified channel operates as interval timer mode.
* @param  ch - specify the channel number
* @param  num - specify the number count clock
* @return None
***********************************************************************************************************************/
void TM81_IntervalTimer(tm8_channel_t ch, uint16_t num)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _00F0_TM8_CKM1_fCLK_15 | _0000_TM8_CKM0_fCLK_0; 

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM8_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
            NVIC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used as interval timer */
            TM81->TMR10 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR10 = num - 1;
            TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
            break;
        case TM8_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
            NVIC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used as interval timer */
            TM81->TMR11 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR11 = num - 1;
            TM81->TOE1 &= ~_0002_TM8_CH1_OUTPUT_ENABLE;
            break;
        case TM8_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
            NVIC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used as interval timer */
            TM81->TMR12 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR12 = num - 1;
            TM81->TOE1 &= ~_0004_TM8_CH2_OUTPUT_ENABLE;
            break;
        case TM8_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
            NVIC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used as interval timer */
            TM81->TMR13 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR13 = num - 1;
            TM81->TOE1 &= ~_0008_TM8_CH3_OUTPUT_ENABLE;
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM81_Channel_Start(ch);

#if 0   
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM8_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM10_IRQn) == 0);
            INTC_ClearPendingIRQ(TM10_IRQn);
            break;
        case TM8_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM11_IRQn) == 0);
            INTC_ClearPendingIRQ(TM11_IRQn);
            break;
        case TM8_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM12_IRQn) == 0);
            INTC_ClearPendingIRQ(TM12_IRQn);
            break;
        case TM8_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM13_IRQn) == 0);
            INTC_ClearPendingIRQ(TM13_IRQn);
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
#endif  
}
/***********************************************************************************************************************
* Function Name: TM81_SquareOutput
* @brief  TM81 specified channel operates as square wave output mode.
* @param  ch - specify the channel number
* @param  num - specify the number of count clock 
* @return None
***********************************************************************************************************************/
void TM81_SquareOutput(tm8_channel_t ch, uint16_t num)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM8_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
            NVIC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used as interval timer */
            TM81->TMR10 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR10 = num - 1;
            TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
            TM81->TOE1 |=  _0001_TM8_CH0_OUTPUT_ENABLE;
            TO10_PORT_SETTING();
            break;
        case TM8_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
            NVIC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used as interval timer */
            TM81->TMR11 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR11 = num - 1;
            TM81->TOM1 &= ~_0002_TM8_CH1_SLAVE_OUTPUT;
            TM81->TOL1 &= ~_0002_TM8_CH1_OUTPUT_LEVEL_L;
            TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;
            TM81->TOE1 |=  _0002_TM8_CH1_OUTPUT_ENABLE;
            TO11_PORT_SETTING();
            break;
        case TM8_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
            NVIC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used as interval timer */
            TM81->TMR12 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR12 = num - 1;
            TM81->TOM1 &= ~_0004_TM8_CH2_SLAVE_OUTPUT;
            TM81->TOL1 &= ~_0004_TM8_CH2_OUTPUT_LEVEL_L;
            TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;
            TM81->TOE1 |=  _0004_TM8_CH2_OUTPUT_ENABLE;
            TO12_PORT_SETTING();
            break;
        case TM8_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
            NVIC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used as interval timer */
            TM81->TMR13 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0000_TM8_TRIGGER_SOFTWARE |
                          _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
            assert_param(IS_TIMER_COUNTER_VALID(num-1));
            TM81->TDR13 = num - 1;
            TM81->TOM1 &= ~_0008_TM8_CH3_SLAVE_OUTPUT;
            TM81->TOL1 &= ~_0008_TM8_CH3_OUTPUT_LEVEL_L;
            TM81->TO1  &= ~_0008_TM8_CH3_OUTPUT_VALUE_1;
            TM81->TOE1 |=  _0008_TM8_CH3_OUTPUT_ENABLE;
            TO13_PORT_SETTING();
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM81_Channel_Start(ch);
}
/***********************************************************************************************************************
* Function Name: TM81_FreqDivider
* @brief  TM81 specified channel operates as frequency divider mode.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @param  num - specify the number of divider
* @return None
***********************************************************************************************************************/
void TM81_FreqDivider(tm8_channel_t ch, tm8_edge_t edge, uint16_t num)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    if(ch == TM8_CHANNEL_0) 
    {
        /* clear interrupt flag */
        INTC_ClearPendingIRQ(TM10_IRQn);
        NVIC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used as interval timer */
        TM81->TMR10 = _0000_TM8_CLOCK_SELECT_CKM0 | _1000_TM8_CLOCK_MODE_TIMN | _0000_TM8_TRIGGER_SOFTWARE |
                      (edge << TMRMN_CIS_Pos) | _0000_TM8_MODE_INTERVAL_TIMER | _0000_TM8_START_INT_UNUSED;
        if(edge == TM8_TI_BOTHEDGE)
        {
            TM81->TDR10 = num - 1;
        }
        else
        {
            TM81->TDR10 = num / 2 - 1;
        }
        TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
        TM81->TOE1 |=  _0001_TM8_CH0_OUTPUT_ENABLE;
        TI10_PORT_SETTING();
        TO10_PORT_SETTING();
    }

    /* Start specify channel */
    TM81_Channel_Start(ch);
}
/***********************************************************************************************************************
* Function Name: TM81_EventCount
* @brief  TM81 specified channel operates as external event counter.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @param  num - specify the number of specifed edge of TI
* @return None
***********************************************************************************************************************/
void TM81_EventCount(tm8_channel_t ch, tm8_edge_t edge, uint16_t num)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _000A_TM8_CKM0_fCLK_10; 

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM8_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
            NVIC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to external event counter */
            TM81->TMR10 = _0000_TM8_CLOCK_SELECT_CKM0 | _1000_TM8_CLOCK_MODE_TIMN | _0000_TM8_TRIGGER_SOFTWARE |
                         (edge << TMRMN_CIS_Pos) | _0006_TM8_MODE_EVENT_COUNT;
            TM81->TDR10 = num - 1;
            TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
            MISC->NFEN2 |= _01_TM8_CH0_NOISE_ON;
            TI10_PORT_SETTING();
            /* enable interrupt */
            INTC_EnableIRQ(TM10_IRQn);
            break;
        case TM8_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
            NVIC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to external event counter */
            TM81->TMR11 = _0000_TM8_CLOCK_SELECT_CKM0 | _1000_TM8_CLOCK_MODE_TIMN | _0000_TM8_TRIGGER_SOFTWARE |
                         (edge << TMRMN_CIS_Pos) | _0006_TM8_MODE_EVENT_COUNT;
            TM81->TDR11 = num - 1;
            TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0002_TM8_CH1_OUTPUT_ENABLE;
            MISC->NFEN2 |= _02_TM8_CH1_NOISE_ON;
            TI11_PORT_SETTING();
            /* enable interrupt */
            INTC_EnableIRQ(TM11_IRQn);
            break;
        case TM8_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
            NVIC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to external event counter */
            TM81->TMR12 = _0000_TM8_CLOCK_SELECT_CKM0 | _1000_TM8_CLOCK_MODE_TIMN | _0000_TM8_TRIGGER_SOFTWARE |
                         (edge << TMRMN_CIS_Pos) | _0006_TM8_MODE_EVENT_COUNT;
            TM81->TDR12 = num - 1;
            TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0004_TM8_CH2_OUTPUT_ENABLE;
            MISC->NFEN2 |= _04_TM8_CH2_NOISE_ON;
            TI12_PORT_SETTING();
            /* enable interrupt */
            INTC_EnableIRQ(TM12_IRQn);
            break;
        case TM8_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
            NVIC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to external event counter */
            TM81->TMR13 = _0000_TM8_CLOCK_SELECT_CKM0 | _1000_TM8_CLOCK_MODE_TIMN | _0000_TM8_TRIGGER_SOFTWARE |
                         (edge << TMRMN_CIS_Pos) | _0006_TM8_MODE_EVENT_COUNT;
            TM81->TDR13 = num - 1;
            TM81->TO1  &= ~_0008_TM8_CH3_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0008_TM8_CH3_OUTPUT_ENABLE;
            MISC->NFEN2 |= _08_TM8_CH3_NOISE_ON;
            TI13_PORT_SETTING();
            /* enable interrupt */
            INTC_EnableIRQ(TM13_IRQn);
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM81_Channel_Start(ch);
 
#if 0		
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM8_CHANNEL_0:
            while(INTC_GetPendingIRQ(TM10_IRQn) == 0);
            INTC_ClearPendingIRQ(TM10_IRQn);
            break;
        case TM8_CHANNEL_1:
            while(INTC_GetPendingIRQ(TM11_IRQn) == 0);
            INTC_ClearPendingIRQ(TM11_IRQn);
            break;
        case TM8_CHANNEL_2:
            while(INTC_GetPendingIRQ(TM12_IRQn) == 0);
            INTC_ClearPendingIRQ(TM12_IRQn);
            break;
        case TM8_CHANNEL_3:
            while(INTC_GetPendingIRQ(TM13_IRQn) == 0);
            INTC_ClearPendingIRQ(TM13_IRQn);
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
 #endif   
}
/***********************************************************************************************************************
* Function Name: TM81_Get_PulsePeriod
* @brief  This function measures the pulse period with polling mode for TM8 module.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @return period - return the pulse period at specified edge
***********************************************************************************************************************/
uint32_t TM81_Get_PulsePeriod(tm8_channel_t ch, tm8_pulse_t edge)
{
    uint32_t period;

    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM8_CHANNEL_0:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
            NVIC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to measure input pulse interval */
            TM81->TMR10 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                         (edge << TMRMN_CIS_Pos) | _0004_TM8_MODE_CAPTURE | _0000_TM8_START_INT_UNUSED;
            TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
            MISC->NFEN2 |= _01_TM8_CH0_NOISE_ON;
            TI10_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM10_IRQn);
            break;
        case TM8_CHANNEL_1:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
            NVIC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to measure input pulse interval */
            TM81->TMR11 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                         (edge << TMRMN_CIS_Pos) | _0004_TM8_MODE_CAPTURE | _0000_TM8_START_INT_UNUSED;
            TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0002_TM8_CH1_OUTPUT_ENABLE;
            MISC->NFEN2 |= _02_TM8_CH1_NOISE_ON;
            TI11_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM11_IRQn);
            break;
        case TM8_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
            NVIC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to measure input pulse interval */
            TM81->TMR12 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                         (edge << TMRMN_CIS_Pos) | _0004_TM8_MODE_CAPTURE | _0000_TM8_START_INT_UNUSED;
            TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0004_TM8_CH2_OUTPUT_ENABLE;
            MISC->NFEN2 |= _04_TM8_CH2_NOISE_ON;
            TI12_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM12_IRQn);
            break;
        case TM8_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
            NVIC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to measure input pulse interval */
            TM81->TMR13 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                         (edge << TMRMN_CIS_Pos) | _0004_TM8_MODE_CAPTURE | _0000_TM8_START_INT_UNUSED;
            TM81->TO1  &= ~_0008_TM8_CH3_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0008_TM8_CH3_OUTPUT_ENABLE;
            MISC->NFEN2 |= _08_TM8_CH3_NOISE_ON;
            TI13_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM13_IRQn);
            break;
        default: 
            break;
    }
    /* Start specify channel */
    TM81_Channel_Start(ch);
    
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM8_CHANNEL_0:
            g_inttm10Taken = 0;
            while(g_inttm10Taken < 2);
            period = g_tm81_ch0_width;
            break;
        case TM8_CHANNEL_1:
            g_inttm11Taken = 0;
            while(g_inttm11Taken < 2);
            period = g_tm81_ch1_width;
            break;
        case TM8_CHANNEL_2:
            g_inttm12Taken = 0;
            while(g_inttm12Taken < 2);
            period = g_tm81_ch2_width;
            break;
        case TM8_CHANNEL_3:
            g_inttm13Taken = 0;
            while(g_inttm13Taken < 2);
            period = g_tm81_ch3_width;
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    return (period); 
}
/***********************************************************************************************************************
* Function Name: TM81_Get_PulseWidth
* @brief  This function initializes the TM8 module.
* @param  ch - specify the channel number
* @param  level - specify the input levle of TI
* @return width - return the pulse width at specified level
***********************************************************************************************************************/
uint32_t TM81_Get_PulseWidth(tm8_channel_t ch, tm8_pulse_t level)
{
    uint32_t width;

    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM8_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
            NVIC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to measure input pulse width */
            TM81->TMR10 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0200_TM8_TRIGGER_TIMN_BOTH |
                         (level << TMRMN_CIS_Pos) | _000C_TM8_MODE_HIGHLOW_MEASURE;
            TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
            MISC->NFEN2 |= _01_TM8_CH0_NOISE_ON;
            TI10_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM10_IRQn);
            break;
        case TM8_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
            NVIC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to measure input pulse width */
            TM81->TMR11 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0200_TM8_TRIGGER_TIMN_BOTH |
                         (level << TMRMN_CIS_Pos) | _000C_TM8_MODE_HIGHLOW_MEASURE;
            TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0002_TM8_CH1_OUTPUT_ENABLE;
            MISC->NFEN2 |= _02_TM8_CH1_NOISE_ON;
            TI11_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM11_IRQn);
            break;
        case TM8_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
            NVIC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to measure input pulse width */
            TM81->TMR12 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0200_TM8_TRIGGER_TIMN_BOTH |
                         (level << TMRMN_CIS_Pos) | _000C_TM8_MODE_HIGHLOW_MEASURE;
            TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0004_TM8_CH2_OUTPUT_ENABLE;
            MISC->NFEN2 |= _04_TM8_CH2_NOISE_ON;
            TI12_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM12_IRQn);
            break;
        case TM8_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
            NVIC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to measure input pulse width */
            TM81->TMR13 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0200_TM8_TRIGGER_TIMN_BOTH |
                         (level << TMRMN_CIS_Pos) | _000C_TM8_MODE_HIGHLOW_MEASURE;
            TM81->TO1  &= ~_0008_TM8_CH3_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0008_TM8_CH3_OUTPUT_ENABLE;
            MISC->NFEN2 |= _08_TM8_CH3_NOISE_ON;
            TI13_PORT_SETTING();
        /* enable interrupt */
            INTC_EnableIRQ(TM13_IRQn);
            break;
        default: 
            break;
    }

    /* Start specify channel */
    TM81_Channel_Start(ch);
    
    /* polling INTTMmn interrupt flag */
    switch(ch) 
    {
        case TM8_CHANNEL_0:
            g_inttm10Taken = 0;
            while(g_inttm10Taken < 1);
            width = g_tm81_ch0_width;
            break;
        case TM8_CHANNEL_1:
            g_inttm11Taken = 0;
            while(g_inttm11Taken < 1);
            width = g_tm81_ch1_width;
            break;
        case TM8_CHANNEL_2:
            g_inttm12Taken = 0;
            while(g_inttm12Taken < 1);
            width = g_tm81_ch2_width;
        case TM8_CHANNEL_3:
            g_inttm13Taken = 0;
            while(g_inttm13Taken < 1);
            width = g_tm81_ch3_width;
            break;
        default: 
            break;
    }

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    return (width); 
}
/***********************************************************************************************************************
* Function Name: TM81_DelayCounter
* @brief  This function initializes the TM8 module.
* @param  ch - specify the channel number
* @param  edge - specify the input edge of TI
* @param  delay - specify the delay number of TI
* @return None
***********************************************************************************************************************/
void TM81_DelayCounter(tm8_channel_t ch, tm8_edge_t edge, uint16_t delay)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* stop specify channel */
    TM81_Channel_Stop(ch);  
    
    switch(ch) 
    {
        case TM8_CHANNEL_0: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM10_IRQn);
            NVIC_ClearPendingIRQ(TM10_IRQn);
        /* Channel 0 is used to measure input pulse width */
            TM81->TMR10 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                          (edge << TMRMN_CIS_Pos) | _0008_TM8_MODE_ONE_COUNT | _0000_TM8_START_INT_UNUSED;
            TM81->TDR10 = delay - 1;
            TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
            MISC->NFEN2 |= _01_TM8_CH0_NOISE_ON;
            TI10_PORT_SETTING();
            break;
        case TM8_CHANNEL_1: 
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM11_IRQn);
            NVIC_ClearPendingIRQ(TM11_IRQn);
        /* Channel 1 is used to measure input pulse width */
            TM81->TMR11 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                          (edge << TMRMN_CIS_Pos) | _0008_TM8_MODE_ONE_COUNT | _0000_TM8_START_INT_UNUSED;
            TM81->TDR11 = delay - 1;
            TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0002_TM8_CH1_OUTPUT_ENABLE;
            MISC->NFEN2 |= _02_TM8_CH1_NOISE_ON;
            TI11_PORT_SETTING();
            break;
        case TM8_CHANNEL_2:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM12_IRQn);
            NVIC_ClearPendingIRQ(TM12_IRQn);
        /* Channel 2 is used to measure input pulse width */
            TM81->TMR12 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                          (edge << TMRMN_CIS_Pos) | _0008_TM8_MODE_ONE_COUNT | _0000_TM8_START_INT_UNUSED;
            TM81->TDR12 = delay - 1;
            TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0004_TM8_CH2_OUTPUT_ENABLE;
            MISC->NFEN2 |= _04_TM8_CH2_NOISE_ON;
            TI12_PORT_SETTING();
            break;
        case TM8_CHANNEL_3:
        /* clear interrupt flag */
            INTC_ClearPendingIRQ(TM13_IRQn);
            NVIC_ClearPendingIRQ(TM13_IRQn);
        /* Channel 3 is used to measure input pulse width */
            TM81->TMR13 = _0000_TM8_CLOCK_SELECT_CKM0 | _0000_TM8_CLOCK_MODE_CKS | _0100_TM8_TRIGGER_TIMN_VALID |
                          (edge << TMRMN_CIS_Pos) | _0008_TM8_MODE_ONE_COUNT | _0000_TM8_START_INT_UNUSED;
            TM81->TDR13 = delay - 1;
            TM81->TO1  &= ~_0008_TM8_CH3_OUTPUT_VALUE_1;
            TM81->TOE1 &= ~_0008_TM8_CH3_OUTPUT_ENABLE;
            MISC->NFEN2 |= _08_TM8_CH3_NOISE_ON;
            TI13_PORT_SETTING();
            break;
        default: 
            break;
    }

    /* Start specify channel */
    TM81_Channel_Start(ch);   
}
/***********************************************************************************************************************
* Function Name: TM81_One_Shot_Pulse_Output
* @brief  This function initializes the TM8 module as one-shot pulse output function.
* @param  edge - the valid edge of TI 
* @param  delay - the delay time from TI valid edge 
* @param  width - the pulse width of output
* @return None
***********************************************************************************************************************/
void TM81_One_Shot_Pulse_Output(tm8_edge_t edge, uint16_t delay, uint16_t width)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* master channel */
    TM81->TMR10 = _8000_TM8_CLOCK_SELECT_CKM1 | _0100_TM8_TRIGGER_TIMN_VALID | (edge << TMRMN_CIS_Pos) | _0008_TM8_MODE_ONE_COUNT;
    TM81->TDR10 = delay - 2;
    TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
    TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
    /* slave channel */
    TM81->TMR11 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0008_TM8_MODE_ONE_COUNT;
    TM81->TDR11 = width;
    TM81->TOM1 |=  _0002_TM8_CH1_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0002_TM8_CH1_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0002_TM8_CH1_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO11_PORT_SETTING();

    /* Start specify channel */
    TM81_Channel_Start(TM8_CHANNEL_0 | TM8_CHANNEL_1);
}
/***********************************************************************************************************************
* Function Name: TM81_PWM_1Period_1Duty
* @brief  This function initializes the TM8 module as PWM function.
* @param  period - the period of PWM waveform
* @param  duty - the duty factor of PWM waveform
* @return None
***********************************************************************************************************************/
void TM81_PWM_1Period_1Duty(uint16_t period, uint16_t duty)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* master channel */
    TM81->TMR10 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_TRIGGER_SOFTWARE | _0001_TM8_MODE_PWM_MASTER;
    TM81->TDR10 = period - 1;
    TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
    TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
    /* slave channel */
    TM81->TMR11 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR11 = duty;
    TM81->TOM1 |=  _0002_TM8_CH1_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0002_TM8_CH1_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0002_TM8_CH1_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO11_PORT_SETTING();

    /* Start specify channel */
    TM81_Channel_Start(TM8_CHANNEL_0 | TM8_CHANNEL_1);
}
/***********************************************************************************************************************
* Function Name: TM81_PWM_1Period_2Duty
* @brief  This function initializes the TM8 module as PWM function.
* @param  period - the period of PWM waveform
* @param  duty1 - the duty1 factor of PWM waveform
* @param  duty2 - the duty2 factor of PWM waveform
* @return None
***********************************************************************************************************************/
void TM81_PWM_1Period_2Duty(uint16_t period, uint16_t duty1, uint16_t duty2)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* master channel */
    TM81->TMR10 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_TRIGGER_SOFTWARE | _0001_TM8_MODE_PWM_MASTER;
    TM81->TDR10 = period - 1;
    TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
    TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
    /* slave channel */
    TM81->TMR11 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR11 = duty1;
    TM81->TOM1 |=  _0002_TM8_CH1_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0002_TM8_CH1_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0002_TM8_CH1_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO11_PORT_SETTING();
    /* slave channel */
    TM81->TMR12 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR12 = duty2;
    TM81->TOM1 |=  _0004_TM8_CH2_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0004_TM8_CH2_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0004_TM8_CH2_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO12_PORT_SETTING();

    /* Start specify channel */
    TM81_Channel_Start(TM8_CHANNEL_0 | TM8_CHANNEL_1 | TM8_CHANNEL_2);
}
/***********************************************************************************************************************
* Function Name: TM81_PWM_1Period_3Duty
* @brief  This function initializes the TM8 module as PWM function.
* @param  period - the period of PWM waveform
* @param  duty1 - the duty1 factor of PWM waveform
* @param  duty2 - the duty2 factor of PWM waveform
* @param  duty3 - the duty3 factor of PWM waveform
* @return None
***********************************************************************************************************************/
void TM81_PWM_1Period_3Duty(uint16_t period, uint16_t duty1, uint16_t duty2, uint16_t duty3)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* master channel */
    TM81->TMR10 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_TRIGGER_SOFTWARE | _0001_TM8_MODE_PWM_MASTER;
    TM81->TDR10 = period - 1;
    TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
    TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
    /* slave channel */
    TM81->TMR11 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR11 = duty1;
    TM81->TOM1 |=  _0002_TM8_CH1_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0002_TM8_CH1_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0002_TM8_CH1_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO11_PORT_SETTING();
    /* slave channel */
    TM81->TMR12 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR12 = duty2;
    TM81->TOM1 |=  _0004_TM8_CH2_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0004_TM8_CH2_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0004_TM8_CH2_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO12_PORT_SETTING();
    /* slave channel */
    TM81->TMR13 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR13 = duty3;
    TM81->TOM1 |=  _0008_TM8_CH3_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0008_TM8_CH3_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0008_TM8_CH3_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0008_TM8_CH3_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO13_PORT_SETTING();

    /* Start specify channel */
    TM81_Channel_Start(TM8_CHANNEL_0 | TM8_CHANNEL_1 | TM8_CHANNEL_2 | TM8_CHANNEL_3);
}
/***********************************************************************************************************************
* Function Name: TM81_PWM_2Period_2Duty
* @brief  This function initializes the TM8 module as PWM function.
* @param  period1 - the period1 of PWM waveform
* @param  duty1 - the duty1 factor of PWM waveform
* @param  period2 - the period2 of PWM waveform
* @param  duty2 - the duty2 factor of PWM waveform
* @return None
***********************************************************************************************************************/
void TM81_PWM_2Period_2Duty(uint16_t period1, uint16_t duty1, uint16_t period2, uint16_t duty2)
{
    CGC->PER2 |= CGC_PER2_TM81EN_Msk;    /* enables input clock supply */
    TM81->TPS1 = _0000_TM8_CKM3_fCLK_8 | _0000_TM8_CKM2_fCLK_1 | _0000_TM8_CKM1_fCLK_0 | _0000_TM8_CKM0_fCLK_0; 

    /* master channel */
    TM81->TMR10 = _8000_TM8_CLOCK_SELECT_CKM1 | _0000_TM8_TRIGGER_SOFTWARE | _0001_TM8_MODE_PWM_MASTER;
    TM81->TDR10 = period1 - 1;
    TM81->TO1  &= ~_0001_TM8_CH0_OUTPUT_VALUE_1;
    TM81->TOE1 &= ~_0001_TM8_CH0_OUTPUT_ENABLE;
    /* slave channel */
    TM81->TMR11 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR11 = duty1;
    TM81->TOM1 |=  _0002_TM8_CH1_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0002_TM8_CH1_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0002_TM8_CH1_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0002_TM8_CH1_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO11_PORT_SETTING();
    /* master channel */
    TM81->TMR12 = _8000_TM8_CLOCK_SELECT_CKM1 | _0800_TM8_COMBINATION_MASTER | _0000_TM8_TRIGGER_SOFTWARE | _0001_TM8_MODE_PWM_MASTER;
    TM81->TDR12 = period2 - 1;
    TM81->TO1  &= ~_0004_TM8_CH2_OUTPUT_VALUE_1;
    TM81->TOE1 &= ~_0004_TM8_CH2_OUTPUT_ENABLE;
    TO12_PORT_SETTING();
    /* slave channel */
    TM81->TMR13 = _8000_TM8_CLOCK_SELECT_CKM1 | _0400_TM8_TRIGGER_MASTER_INT | _0009_TM8_MODE_PWM_SLAVE;
    TM81->TDR13 = duty2;
    TM81->TOM1 |=  _0008_TM8_CH3_SLAVE_OUTPUT;       /* slave channel output mode */
    TM81->TOL1 &= ~_0008_TM8_CH3_OUTPUT_LEVEL_L;     /* positive logic output (active-high) */
    TM81->TO1  &= ~_0008_TM8_CH3_OUTPUT_VALUE_1;     /* TOmp default output level */
    TM81->TOE1 |=  _0008_TM8_CH3_OUTPUT_ENABLE;      /* Enable TOmp output by counting operation */
    TO13_PORT_SETTING();

    /* Start specify channel */
    TM81_Channel_Start(TM8_CHANNEL_0 | TM8_CHANNEL_1 | TM8_CHANNEL_2 | TM8_CHANNEL_3 );
}
/***********************************************************************************************************************
* Function Name: TM81_Channel_Start
* @brief  This function starts TM81 channel counter.
* @param  ch - specify the channel number
* @return None
***********************************************************************************************************************/
void TM81_Channel_Start(uint16_t ch)
{
    TM81->TS1 |= ch;
}
/***********************************************************************************************************************
* Function Name: TM8_Channel1_Stop
* @brief  This function stops TM81 channel counter.
* @param  ch - specify the channel number
* @return None
***********************************************************************************************************************/
void TM81_Channel_Stop(uint16_t ch)
{
    TM81->TT1 |= ch;
}
/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
