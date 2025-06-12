/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    iica_user.c
* @brief   This file implements device driver for IICA module.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "userdefine.h"
#include "BAT32A239.h"
#include "iica.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//void IRQ16_Handler(void) __attribute__((alias("iica0_interrupt")));

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: iica0_interrupt
* @brief  IICA interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void iica0_interrupt(void)
{
    INTC_ClearPendingIRQ(IICA0_IRQn);
    IICA0_Clear_Wakeup();
    if ((IICA0->IICS0 & _80_IICA_MASTER_DEVICE) == 0x80U)
    {
        iica0_masterhandler();
    }
    else
    {
        iica0_slavehandler();
    }
}
/***********************************************************************************************************************
* Function Name: iica0_masterhandler
* @brief  This function is IICA0 master handler.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_masterhandler(void)
{
    /* Control for communication */
    if ((0U == (IICA0->IICF0 & IICA0_IICF0_IICBSY_Msk)) && (g_iica0_tx_cnt != 0U))
    {
        iica0_callback_master_error(MD_SPT);
    }
    /* Control for sended address */
    else
    {
        if ((g_iica0_master_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (IICA0->IICS0 & IICA0_IICS0_ACKD_Msk)
            {
                g_iica0_master_status_flag |= _80_IICA_ADDRESS_COMPLETE;

                if (IICA0->IICS0 & IICA0_IICS0_TRC_Msk)
                {
                    IICA0->IICCTL00 |= IICA0_IICCTL00_WTIM_Msk;     /* interrupt request is generated at the ninth clock's falling edge */

                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0->IICA00 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        iica0_callback_master_sendend();
                    }
                }
                else
                {
                    IICA0->IICCTL00 |= IICA0_IICCTL00_ACKE_Msk;     /* enable acknowledgment */
                    IICA0->IICCTL00 &= ~IICA0_IICCTL00_WTIM_Msk;    /* interrupt request is generated at the eighth clock's falling edge */
                    IICA0->IICCTL00 |= IICA0_IICCTL00_WREL_Msk;     /* cancel wait */
                }
            }
            else
            {
                iica0_callback_master_error(MD_NACK);
            }
        }
        else
        {
            /* Master send control */
            if (IICA0->IICS0 & IICA0_IICS0_TRC_Msk)
            {
                if ((0U == (IICA0->IICS0 & IICA0_IICS0_ACKD_Msk)) && (g_iica0_tx_cnt != 0U))
                {
                    iica0_callback_master_error(MD_NACK);
                }
                else
                {
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0->IICA00 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        iica0_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0->IICA00;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;

                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        IICA0->IICCTL00 &= ~IICA0_IICCTL00_ACKE_Msk;   /* disable acknowledgment */
                        IICA0->IICCTL00 |= IICA0_IICCTL00_WREL_Msk;    /* cancel wait */
                        IICA0->IICCTL00 |= IICA0_IICCTL00_WTIM_Msk;    /* interrupt request is generated at the eighth clock's falling edge */
                    }
                    else
                    {
                        IICA0->IICCTL00 |= IICA0_IICCTL00_WREL_Msk;     /* cancel wait */
                    }
                }
                else
                {
                    iica0_callback_master_receiveend();
                }
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: iica0_callback_master_error
* @brief  This function is a callback function when IICA0 master error occurs.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica0_callback_master_receiveend
* @brief  This function is a callback function when IICA0 finishes master reception.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_callback_master_receiveend(void)
{
    IICA0->IICCTL00 |= IICA0_IICCTL00_SPT_Msk;            /* stop condition is generated */
    while(0U == (IICA0->IICS0 & IICA0_IICS0_SPD_Msk));    /* stop condition is detected */

    /* Start user code. Do not edit comment generated here */
        g_iica0_rx_end = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica0_callback_master_sendend
* @brief  This function is a callback function when IICA0 finishes master transmission.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_callback_master_sendend(void)
{
    IICA0->IICCTL00 |= IICA0_IICCTL00_SPT_Msk;            /* stop condition is generated */
    while(0U == (IICA0->IICS0 & IICA0_IICS0_SPD_Msk));    /* stop condition is detected */
    /* Start user code. Do not edit comment generated here */
    g_iica0_tx_end = 1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: iica0_slavehandler
* @brief  This function is IICA0 slave handler.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_slavehandler(void)
{
    /* Control for stop condition */
    if (IICA0->IICS0 & IICA0_IICS0_SPD_Msk)
    {
        /* Get stop condition */
        IICA0->IICCTL00 &= ~IICA0_IICCTL00_SPIE_Msk; /* SPIE0 = 0: disable */
        if(g_iica0_slave_status_flag & _80_IICA_ADDRESS_COMPLETE)
        {
            if (g_iica0_slave_status_flag & 0x04U)   /* send flag */
            {
                iica0_callback_slave_sendend();
            }
            if (g_iica0_slave_status_flag & 0x02U)   /* receive flag */
            {
                iica0_callback_slave_receiveend();
            }
        }
        g_iica0_slave_status_flag = 1U;
    }
    else
    {
        if ((g_iica0_slave_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (IICA0->IICS0 & IICA0_IICS0_COI_Msk)
            {
                IICA0->IICCTL00 |= IICA0_IICCTL00_SPIE_Msk; /* SPIE0 = 1: enable */
                //g_iica0_slave_status_flag |= _80_IICA_ADDRESS_COMPLETE;
                g_iica0_slave_status_flag = _80_IICA_ADDRESS_COMPLETE;

                if (IICA0->IICS0 & IICA0_IICS0_TRC_Msk)  /* TRC0 == 1 */
                {
                    IICA0->IICCTL00 |= IICA0_IICCTL00_WTIM_Msk;  /* WTIM0 = 1:  interrupt request is generated at the ninth clock's falling edge */

                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0->IICA00 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        iica0_callback_slave_sendend();
                        IICA0->IICCTL00 |= IICA0_IICCTL00_WREL_Msk;  /* WREL0 = 1U: cancel wait */
                    }
                }
                else
                {
                    IICA0->IICCTL00 |=  IICA0_IICCTL00_ACKE_Msk;   /* ACKE0 = 1U: enable acknowledgment */
                    IICA0->IICCTL00 &= ~IICA0_IICCTL00_WTIM_Msk;   /* WTIM0 = 0U: interrupt request is generated at the eighth clock's falling edge */
                    IICA0->IICCTL00 |=  IICA0_IICCTL00_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                }
            }
            else
            {
                iica0_callback_slave_error(MD_ERROR);
            }
        }
        else
        {
            if (IICA0->IICS0 & IICA0_IICS0_TRC_Msk)  /* TRC0 == 1 */
            {
                g_iica0_slave_status_flag |= 4U;   /* send flag */
                if ((0U == (IICA0->IICS0 & IICA0_IICS0_ACKD_Msk)) && (g_iica0_tx_cnt != 0U))
                {
                    iica0_callback_slave_error(MD_NACK);
                }
                else
                {
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0->IICA00 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        iica0_callback_slave_sendend();
                        IICA0->IICCTL00 |= IICA0_IICCTL00_WREL_Msk;  /* WREL0 = 1U: cancel wait */
                    }
                }
            }
            else
            {
                g_iica0_slave_status_flag |= 2U;  /* receive flag */
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0->IICA00;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;

                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        IICA0->IICCTL00 |= IICA0_IICCTL00_WTIM_Msk;   /* WTIM0 = 1:  interrupt request is generated at the ninth clock's falling edge */
                        IICA0->IICCTL00 |= IICA0_IICCTL00_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                        iica0_callback_slave_receiveend();
                    }
                    else
                    {
                        IICA0->IICCTL00 |=  IICA0_IICCTL00_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                    }
                }
                else
                {
                    IICA0->IICCTL00 |=  IICA0_IICCTL00_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                }
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: iica0_callback_slave_error
* @brief  This function is a callback function when IICA0 slave error occurs.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_callback_slave_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    IICA0->IICCTL00 |=  IICA0_IICCTL00_WREL_Msk;   /* WREL0 = 1U: cancel wait */   
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica0_callback_slave_receiveend
* @brief  This function is a callback function when IICA0 finishes slave reception.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_callback_slave_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    g_iica0_rx_end = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica0_callback_slave_sendend
* @brief  This function is a callback function when IICA0 finishes slave transmission.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica0_callback_slave_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    g_iica0_tx_end = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica1_interrupt
* @brief  IICA interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void iica1_interrupt(void)
{
    INTC_ClearPendingIRQ(IICA1_IRQn);
    IICA1_Clear_Wakeup();
    if ((IICA1->IICS1 & _80_IICA_MASTER_DEVICE) == 0x80U)
    {
        iica1_masterhandler();
    }
    else
    {
        iica1_slavehandler();
    }
}
/***********************************************************************************************************************
* Function Name: iica1_masterhandler
* @brief  This function is IICA1 master handler.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_masterhandler(void)
{
    /* Control for communication */
    if ((0U == (IICA1->IICF1 & IICA1_IICF1_IICBSY_Msk)) && (g_iica1_tx_cnt != 0U))
    {
        iica1_callback_master_error(MD_SPT);
    }
    /* Control for sended address */
    else
    {
        if ((g_iica1_master_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (IICA1->IICS1 & IICA1_IICS1_ACKD_Msk)
            {
                g_iica1_master_status_flag |= _80_IICA_ADDRESS_COMPLETE;

                if (IICA1->IICS1 & IICA1_IICS1_TRC_Msk)
                {
                    IICA1->IICCTL10 |= IICA1_IICCTL10_WTIM_Msk;     /* interrupt request is generated at the ninth clock's falling edge */

                    if (g_iica1_tx_cnt > 0U)
                    {
                        IICA1->IICA10 = *gp_iica1_tx_address;
                        gp_iica1_tx_address++;
                        g_iica1_tx_cnt--;
                    }
                    else
                    {
                        iica1_callback_master_sendend();
                    }
                }
                else
                {
                    IICA1->IICCTL10 |= IICA1_IICCTL10_ACKE_Msk;     /* enable acknowledgment */
                    IICA1->IICCTL10 &= ~IICA1_IICCTL10_WTIM_Msk;    /* interrupt request is generated at the eighth clock's falling edge */
                    IICA1->IICCTL10 |= IICA1_IICCTL10_WREL_Msk;     /* cancel wait */
                }
            }
            else
            {
                iica1_callback_master_error(MD_NACK);
            }
        }
        else
        {
            /* Master send control */
            if (IICA1->IICS1 & IICA1_IICS1_TRC_Msk)
            {
                if ((0U == (IICA1->IICS1 & IICA1_IICS1_ACKD_Msk)) && (g_iica1_tx_cnt != 0U))
                {
                    iica1_callback_master_error(MD_NACK);
                }
                else
                {
                    if (g_iica1_tx_cnt > 0U)
                    {
                        IICA1->IICA10 = *gp_iica1_tx_address;
                        gp_iica1_tx_address++;
                        g_iica1_tx_cnt--;
                    }
                    else
                    {
                        iica1_callback_master_sendend();
                    }
                }
            }
            /* Master receive control */
            else
            {
                if (g_iica1_rx_cnt < g_iica1_rx_len)
                {
                    *gp_iica1_rx_address = IICA1->IICA10;
                    gp_iica1_rx_address++;
                    g_iica1_rx_cnt++;

                    if (g_iica1_rx_cnt == g_iica1_rx_len)
                    {
                        IICA1->IICCTL10 &= ~IICA1_IICCTL10_ACKE_Msk;   /* disable acknowledgment */
                        IICA1->IICCTL10 |= IICA1_IICCTL10_WREL_Msk;    /* cancel wait */
                        IICA1->IICCTL10 |= IICA1_IICCTL10_WTIM_Msk;    /* interrupt request is generated at the eighth clock's falling edge */
                    }
                    else
                    {
                        IICA1->IICCTL10 |= IICA1_IICCTL10_WREL_Msk;     /* cancel wait */
                    }
                }
                else
                {
                    iica1_callback_master_receiveend();
                }
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: iica1_callback_master_error
* @brief  This function is a callback function when IICA1 master error occurs.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_callback_master_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica1_callback_master_receiveend
* @brief  This function is a callback function when IICA1 finishes master reception.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_callback_master_receiveend(void)
{
    IICA1->IICCTL10 |= IICA1_IICCTL10_SPT_Msk;            /* stop condition is generated */
    while(0U == (IICA1->IICS1 & IICA1_IICS1_SPD_Msk));    /* stop condition is detected */

    /* Start user code. Do not edit comment generated here */
        g_iica1_rx_end = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica1_callback_master_sendend
* @brief  This function is a callback function when IICA1 finishes master transmission.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_callback_master_sendend(void)
{
    IICA1->IICCTL10 |= IICA1_IICCTL10_SPT_Msk;            /* stop condition is generated */
    while(0U == (IICA1->IICS1 & IICA1_IICS1_SPD_Msk));    /* stop condition is detected */
    /* Start user code. Do not edit comment generated here */
    g_iica1_tx_end = 1;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: iica1_slavehandler
* @brief  This function is IICA1 slave handler.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_slavehandler(void)
{
    /* Control for stop condition */
    if (IICA1->IICS1 & IICA1_IICS1_SPD_Msk)
    {
        /* Get stop condition */
        IICA1->IICCTL10 &= ~IICA1_IICCTL10_SPIE_Msk; /* SPIE0 = 0: disable */
        if(g_iica1_slave_status_flag & _80_IICA_ADDRESS_COMPLETE)
        {
            if (g_iica1_slave_status_flag & 0x04U)   /* send flag */
            {
                iica1_callback_slave_sendend();
            }
            if (g_iica1_slave_status_flag & 0x02U)   /* receive flag */
            {
                iica1_callback_slave_receiveend();
            }
        }
        g_iica1_slave_status_flag = 1U;
    }
    else
    {
        if ((g_iica1_slave_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (IICA1->IICS1 & IICA1_IICS1_COI_Msk)
            {
                IICA1->IICCTL10 |= IICA1_IICCTL10_SPIE_Msk; /* SPIE0 = 1: enable */
                //g_iica1_slave_status_flag |= _80_IICA_ADDRESS_COMPLETE;
                g_iica1_slave_status_flag = _80_IICA_ADDRESS_COMPLETE;

                if (IICA1->IICS1 & IICA1_IICS1_TRC_Msk)  /* TRC0 == 1 */
                {
                    IICA1->IICCTL10 |= IICA1_IICCTL10_WTIM_Msk;  /* WTIM0 = 1:  interrupt request is generated at the ninth clock's falling edge */

                    if (g_iica1_tx_cnt > 0U)
                    {
                        IICA1->IICA10 = *gp_iica1_tx_address;
                        gp_iica1_tx_address++;
                        g_iica1_tx_cnt--;
                    }
                    else
                    {
                        iica1_callback_slave_sendend();
                        IICA1->IICCTL10 |= IICA1_IICCTL10_WREL_Msk;  /* WREL0 = 1U: cancel wait */
                    }
                }
                else
                {
                    IICA1->IICCTL10 |=  IICA1_IICCTL10_ACKE_Msk;   /* ACKE0 = 1U: enable acknowledgment */
                    IICA1->IICCTL10 &= ~IICA1_IICCTL10_WTIM_Msk;   /* WTIM0 = 0U: interrupt request is generated at the eighth clock's falling edge */
                    IICA1->IICCTL10 |=  IICA1_IICCTL10_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                }
            }
            else
            {
                iica1_callback_slave_error(MD_ERROR);
            }
        }
        else
        {
            if (IICA1->IICS1 & IICA1_IICS1_TRC_Msk)  /* TRC0 == 1 */
            {
                g_iica1_slave_status_flag |= 4U;   /* send flag */
                if ((0U == (IICA1->IICS1 & IICA1_IICS1_ACKD_Msk)) && (g_iica1_tx_cnt != 0U))
                {
                    iica1_callback_slave_error(MD_NACK);
                }
                else
                {
                    if (g_iica1_tx_cnt > 0U)
                    {
                        IICA1->IICA10 = *gp_iica1_tx_address;
                        gp_iica1_tx_address++;
                        g_iica1_tx_cnt--;
                    }
                    else
                    {
                        iica1_callback_slave_sendend();
                        IICA1->IICCTL10 |= IICA1_IICCTL10_WREL_Msk;  /* WREL0 = 1U: cancel wait */
                    }
                }
            }
            else
            {
                g_iica1_slave_status_flag |= 2U;  /* receive flag */
                if (g_iica1_rx_cnt < g_iica1_rx_len)
                {
                    *gp_iica1_rx_address = IICA1->IICA10;
                    gp_iica1_rx_address++;
                    g_iica1_rx_cnt++;

                    if (g_iica1_rx_cnt == g_iica1_rx_len)
                    {
                        IICA1->IICCTL10 |= IICA1_IICCTL10_WTIM_Msk;   /* WTIM0 = 1:  interrupt request is generated at the ninth clock's falling edge */
                        IICA1->IICCTL10 |= IICA1_IICCTL10_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                        iica1_callback_slave_receiveend();
                    }
                    else
                    {
                        IICA1->IICCTL10 |=  IICA1_IICCTL10_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                    }
                }
                else
                {
                    IICA1->IICCTL10 |=  IICA1_IICCTL10_WREL_Msk;   /* WREL0 = 1U: cancel wait */
                }
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: iica1_callback_slave_error
* @brief  This function is a callback function when IICA1 slave error occurs.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_callback_slave_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
    IICA1->IICCTL10 |=  IICA1_IICCTL10_WREL_Msk;   /* WREL0 = 1U: cancel wait */   
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica1_callback_slave_receiveend
* @brief  This function is a callback function when IICA1 finishes slave reception.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_callback_slave_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    g_iica1_rx_end = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: iica1_callback_slave_sendend
* @brief  This function is a callback function when IICA1 finishes slave transmission.
* @param  None
* @return None
***********************************************************************************************************************/
static void iica1_callback_slave_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    g_iica1_tx_end = 1;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
