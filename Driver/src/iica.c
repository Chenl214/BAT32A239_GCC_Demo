/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    iica.c
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
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t   g_iica0_master_status_flag; /* iica0 master flag */
volatile uint8_t   g_iica0_slave_status_flag;  /* iica0 slave flag */
volatile uint8_t * gp_iica0_rx_address;        /* iica0 receive buffer address */
volatile uint16_t  g_iica0_rx_len;             /* iica0 receive data length */
volatile uint16_t  g_iica0_rx_cnt;             /* iica0 receive data count */
volatile uint8_t * gp_iica0_tx_address;        /* iica0 send buffer address */
volatile uint16_t  g_iica0_tx_cnt;             /* iica0 send data count */
volatile uint8_t   g_iica0_tx_end;             /* iica0 send data end */
volatile uint8_t   g_iica0_rx_end;             /* iica0 receive data end */
volatile uint8_t   g_iica1_master_status_flag; /* iica1 master flag */ 
volatile uint8_t   g_iica1_slave_status_flag;  /* iica1 slave flag */
volatile uint8_t * gp_iica1_rx_address;        /* iica1 receive buffer address */
volatile uint16_t  g_iica1_rx_cnt;             /* iica1 receive data length */
volatile uint16_t  g_iica1_rx_len;             /* iica1 receive data count */
volatile uint8_t * gp_iica1_tx_address;        /* iica1 send buffer address */
volatile uint16_t  g_iica1_tx_cnt;             /* iica1 send data count */
volatile uint8_t   g_iica1_tx_end;             /* iica1 send data end */
volatile uint8_t   g_iica1_rx_end;             /* iica1 receive data end */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: IICA0_Init
* @brief  This function initializes the IICA0 module.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA0_Init(void)
{
    CGC->PER0 |= CGC_PER0_IICA0EN_Msk;              /* enables input clock supply */
    IICA0->IICCTL00 = _00_IICA_OPERATION_STOP;       /* stop operation */
    INTC_DisableIRQ(IICA0_IRQn);                         /* disable INTIICA interrupt flag */           
    INTC_ClearPendingIRQ(IICA0_IRQn);                    /* clear INTIICA interrupt flag */         

#ifdef IICA_STANDARD_MODE /* Max rate: 100Kbps */
    IICA0->IICCTL01  = _00_IICA_OPERATE_STANDARD;  /* operates in standard mode */
    IICA0->IICWL0    = _4B_IICA_LOW_WIDTH;
    IICA0->IICWH0    = _53_IICA_HIGH_WIDTH;
#endif
#ifdef IICA_FAST_MODE /* Max rate: 400Kbps */
    IICA0->IICCTL01  = _08_IICA_OPERATE_FAST;  /* operates in fast mode */
    IICA0->IICCTL01 |= _04_IICA_FILTER_ON;     /* filter on */
    IICA0->IICWL0    = _14_IICA_LOW_WIDTH;
    IICA0->IICWH0    = _12_IICA_HIGH_WIDTH;
#endif
    IICA0->IICCTL01 |= _01_IICA_FCLK_2_SELECTED;
    IICA0->SVA0      = _98_IICA0_SLAVEADDRESS;
    IICA0->IICF0    |= _02_IICA_WITHOUT_DETECTION; /* enable generation of a start condition without detecting a stop
condition */
    IICA0->IICF0    |= _01_IICA_RESERVATION_DISABLE;       /* disable communication reservation */
    IICA0->IICCTL00 |= _08_IICA_INTERRUPT_REQUEST_NINTH; /* interrupt request is generated at the ninth clock's falling edge */
    IICA0->IICCTL00 |= _04_IICA_ACKOWNLEDGMENT_ENABLE;   /* enable acknowledgment */
    IICA0->IICCTL00 |= _80_IICA_OPERATION_ENABLE;                /* enable operation */
    IICA0->IICCTL00 |= _40_IICA_OPERATION_STANDBY;       /* this exits from the current communications and sets standby mode */
    INTC_EnableIRQ(IICA0_IRQn);                          /* enable INTIICA interrupt flag */     
		
    /* Set SCLA0, SDAA0 pin */
    SCLA0_PORT_SETTING();
    SDAA0_PORT_SETTING();
}
/***********************************************************************************************************************
* Function Name: IICA0_Stop
* @brief  This function stops IICA0 module operation.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA0_Stop(void)
{
    IICA0->IICCTL00 &= ~IICA0_IICCTL00_IICE_Msk;     /* stop operation */
}
/***********************************************************************************************************************
* Function Name: IICA0_StopCondition
* @brief  This function sets IICA0 stop condition flag.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA0_StopCondition(void)
{
    IICA0->IICCTL00 |= IICA0_IICCTL00_SPT_Msk;      /* stop condition is generated */
}
/***********************************************************************************************************************
* Function Name: IICA0_MasterSend
* @brief  This function starts to send data as master mode.
* @param  adr - send address
* @param  tx_buf - transfer buffer pointer
* @param  tx_num - buffer size
* @param  wait
*             - wait for start condition
* @return status
*             - MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS IICA0_MasterSend(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    INTC_DisableIRQ(IICA0_IRQn);   /* disable INTIICA0 interrupt */

    if (IICA0->IICF0 & IICA0_IICF0_IICBSY_Msk)
    {
        /* Check bus busy */
        INTC_EnableIRQ(IICA0_IRQn);   /* enable INTIICA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        IICA0->IICCTL00 |= IICA0_IICCTL00_STT_Msk;      /* generate a start condition */
        INTC_EnableIRQ(IICA0_IRQn);   /* enable INTIICA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == (IICA0->IICS0 & IICA0_IICS0_STD_Msk))
        {
            status = MD_ERROR2;
        }
        
        /* Set parameter */
        g_iica0_tx_cnt = tx_num;
        gp_iica0_tx_address = tx_buf;
        g_iica0_tx_end = 0;	
        g_iica0_master_status_flag = _00_IICA_MASTER_FLAG_CLEAR;
        adr &= (uint8_t)~0x01U; /* set send mode */
        IICA0->IICA00 = adr; /* send address */
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: IICA0_MasterReceive
* @brief  This function starts to receive IICA0 data as master mode.
* @param  adr - receive address
* @param  rx_buf - receive buffer pointer
* @param  rx_num - buffer size
* @param  wait
*             - wait for start condition
* @return status
*             - MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS IICA0_MasterReceive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    INTC_DisableIRQ(IICA0_IRQn);   /* disable INTIICA0 interrupt */

    if (IICA0->IICF0 & IICA0_IICF0_IICBSY_Msk)
    {
        /* Check bus busy */
        INTC_EnableIRQ(IICA0_IRQn);   /* enable INTIICA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        IICA0->IICCTL00 |= IICA0_IICCTL00_STT_Msk;      /* generate a start condition */
        INTC_EnableIRQ(IICA0_IRQn);   /* enable INTIICA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == (IICA0->IICS0 & IICA0_IICS0_STD_Msk))
        {
            status = MD_ERROR2;
        }
        
        /* Set parameter */
        g_iica0_rx_len = rx_num;
        g_iica0_rx_cnt = 0U;
        gp_iica0_rx_address = rx_buf;
        g_iica0_rx_end = 0;	
        g_iica0_master_status_flag  = _00_IICA_MASTER_FLAG_CLEAR;
        adr |= 0x01U; /* set receive mode */
        IICA0->IICA00 = adr; /* receive address */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: IICA0_SlaveSend
* @brief  This function sends data as slave mode.
* @param  adr - send address
* @param  tx_buf - transfer buffer pointer
* @param  tx_num - buffer size
* @return None
***********************************************************************************************************************/
void IICA0_SlaveSend(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iica0_tx_cnt = tx_num;
    gp_iica0_tx_address = tx_buf;
    g_iica0_tx_end = 0;	
    g_iica0_slave_status_flag = 0U;
    IICA0->SVA0 = adr; /* slave address */
}
/***********************************************************************************************************************
* Function Name: IICA0_SlaveReceive
* @brief  This function receives data as slave mode.
* @param  adr - send address
* @param  tx_buf - receive buffer pointer
* @param  rx_num - buffer size
* @return None
***********************************************************************************************************************/
void IICA0_SlaveReceive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iica0_rx_len = rx_num;
    g_iica0_rx_cnt = 0U;
    gp_iica0_rx_address = rx_buf;
    g_iica0_rx_end = 0;	
    g_iica0_slave_status_flag  = 0U;
    IICA0->SVA0 = adr; /* slave address */
}
/***********************************************************************************************************************
* Function Name: IICA0_Set_Wakeup
* @brief  This function sets the WUPn bit of IICCTL01 register.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA0_Set_Wakeup(void)
{
    IICA0->IICCTL01 |= 0x80; /* WUP = 1 */
  	__NOP();
  	__NOP();
  	__NOP();	
	
    __STOP();
}

/***********************************************************************************************************************
* Function Name: IICA0_Clear_Wakeup
* @brief  This function clears the WUPn bit of IICCTL01 register.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA0_Clear_Wakeup(void)
{
    IICA0->IICCTL01 &= ~0x80; /* WUP = 0 */
  	__NOP();
  	__NOP();
  	__NOP();	
  	__NOP();
  	__NOP();	
}
/***********************************************************************************************************************
* Function Name: IICA1_Init
* @brief  This function initializes the IICA1 module.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA1_Init(void)
{
    CGC->PER2 |= CGC_PER2_IICA1EN_Msk;              /* enables input clock supply */
    IICA1->IICCTL10 = _00_IICA_OPERATION_STOP;       /* stop operation */
    INTC_DisableIRQ(IICA1_IRQn);                         /* disable INTIICA interrupt flag */           
    INTC_ClearPendingIRQ(IICA1_IRQn);                    /* clear INTIICA interrupt flag */         

#ifdef IICA_STANDARD_MODE /* Max rate: 100Kbps */
    IICA1->IICCTL11  = _00_IICA_OPERATE_STANDARD;  /* operates in standard mode */
    IICA1->IICWL1    = _4B_IICA_LOW_WIDTH;
    IICA1->IICWH1    = _53_IICA_HIGH_WIDTH;
#endif
#ifdef IICA_FAST_MODE /* Max rate: 400Kbps */
    IICA1->IICCTL11  = _08_IICA_OPERATE_FAST;  /* operates in fast mode */
    IICA1->IICCTL11 |= _04_IICA_FILTER_ON;     /* filter on */
    IICA1->IICWL1    = _14_IICA_LOW_WIDTH;
    IICA1->IICWH1    = _12_IICA_HIGH_WIDTH;
#endif
    IICA1->IICCTL11 |= _01_IICA_FCLK_2_SELECTED;
    IICA1->SVA1      = _98_IICA1_SLAVEADDRESS;
    IICA1->IICF1    |= _02_IICA_WITHOUT_DETECTION; /* enable generation of a start condition without detecting a stop
condition */
    IICA1->IICF1    |= _01_IICA_RESERVATION_DISABLE;       /* disable communication reservation */
    IICA1->IICCTL10 |= _08_IICA_INTERRUPT_REQUEST_NINTH; /* interrupt request is generated at the ninth clock's falling edge */
    IICA1->IICCTL10 |= _04_IICA_ACKOWNLEDGMENT_ENABLE;   /* enable acknowledgment */
    IICA1->IICCTL10 |= _80_IICA_OPERATION_ENABLE;                /* enable operation */
    IICA1->IICCTL10 |= _40_IICA_OPERATION_STANDBY;       /* this exits from the current communications and sets standby mode */
    INTC_EnableIRQ(IICA1_IRQn);                          /* enable INTIICA interrupt flag */     
		
    /* Set SCLA0, SDAA0 pin */
    SCLA1_PORT_SETTING();
    SDAA1_PORT_SETTING();
}
/***********************************************************************************************************************
* Function Name: IICA1_Stop
* @brief  This function stops IICA1 module operation.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA1_Stop(void)
{
    IICA1->IICCTL10 &= ~IICA1_IICCTL10_IICE_Msk;     /* stop operation */
}
/***********************************************************************************************************************
* Function Name: IICA1_StopCondition
* @brief  This function sets IICA1 stop condition flag.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA1_StopCondition(void)
{
    IICA1->IICCTL10 |= IICA1_IICCTL10_SPT_Msk;      /* stop condition is generated */
}
/***********************************************************************************************************************
* Function Name: IICA1_MasterSend
* @brief  This function starts to send data as master mode.
* @param  adr - send address
* @param  tx_buf - transfer buffer pointer
* @param  tx_num - buffer size
* @param  wait
*             - wait for start condition
* @return status
*             - MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS IICA1_MasterSend(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    INTC_DisableIRQ(IICA1_IRQn);   /* disable INTIICA1 interrupt */

    if (IICA1->IICF1 & IICA1_IICF1_IICBSY_Msk)
    {
        /* Check bus busy */
        INTC_EnableIRQ(IICA1_IRQn);   /* enable INTIICA1 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        IICA1->IICCTL10 |= IICA1_IICCTL10_STT_Msk;      /* generate a start condition */
        INTC_EnableIRQ(IICA1_IRQn);   /* enable INTIICA1 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == (IICA1->IICS1 & IICA1_IICS1_STD_Msk))
        {
            status = MD_ERROR2;
        }
        
        /* Set parameter */
        g_iica1_tx_cnt = tx_num;
        gp_iica1_tx_address = tx_buf;
        g_iica1_tx_end = 0;	
        g_iica1_master_status_flag = _00_IICA_MASTER_FLAG_CLEAR;
        adr &= (uint8_t)~0x01U; /* set send mode */
        IICA1->IICA10 = adr; /* send address */
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: IICA1_MasterReceive
* @brief  This function starts to receive IICA1 data as master mode.
* @param  adr - receive address
* @param  rx_buf - receive buffer pointer
* @param  rx_num - buffer size
* @param  wait
*             - wait for start condition
* @return status
*             - MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS IICA1_MasterReceive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

    INTC_DisableIRQ(IICA1_IRQn);   /* disable INTIICA1 interrupt */

    if (IICA1->IICF1 & IICA1_IICF1_IICBSY_Msk)
    {
        /* Check bus busy */
        INTC_EnableIRQ(IICA1_IRQn);   /* enable INTIICA1 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        IICA1->IICCTL10 |= IICA1_IICCTL10_STT_Msk;      /* generate a start condition */
        INTC_EnableIRQ(IICA1_IRQn);   /* enable INTIICA1 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == (IICA1->IICS1 & IICA1_IICS1_STD_Msk))
        {
            status = MD_ERROR2;
        }
        
        /* Set parameter */
        g_iica1_rx_len = rx_num;
        g_iica1_rx_cnt = 0U;
        gp_iica1_rx_address = rx_buf;
        g_iica1_rx_end = 0;	
        g_iica1_master_status_flag  = _00_IICA_MASTER_FLAG_CLEAR;
        adr |= 0x01U; /* set receive mode */
        IICA1->IICA10 = adr; /* receive address */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: IICA1_SlaveSend
* @brief  This function sends data as slave mode.
* @param  adr - send address
* @param  tx_buf - transfer buffer pointer
* @param  tx_num - buffer size
* @return None
***********************************************************************************************************************/
void IICA1_SlaveSend(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
    g_iica1_tx_cnt = tx_num;
    gp_iica1_tx_address = tx_buf;
    g_iica1_tx_end = 0;	
    g_iica1_slave_status_flag = 0U;
    IICA1->SVA1 = adr; /* slave address */
}
/***********************************************************************************************************************
* Function Name: IICA1_SlaveReceive
* @brief  This function receives data as slave mode.
* @param  adr - send address
* @param  tx_buf - receive buffer pointer
* @param  rx_num - buffer size
* @return None
***********************************************************************************************************************/
void IICA1_SlaveReceive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
    g_iica1_rx_len = rx_num;
    g_iica1_rx_cnt = 0U;
    gp_iica1_rx_address = rx_buf;
    g_iica1_rx_end = 0;	
    g_iica1_slave_status_flag  = 0U;
    IICA1->SVA1 = adr; /* slave address */
}
/***********************************************************************************************************************
* Function Name: IICA1_Set_Wakeup
* @brief  This function sets the WUPn bit of IICCTL01 register.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA1_Set_Wakeup(void)
{
    IICA1->IICCTL11 |= 0x80; /* WUP = 1 */
  	__NOP();
  	__NOP();
  	__NOP();	
	
    __STOP();
}

/***********************************************************************************************************************
* Function Name: IICA1_Clear_Wakeup
* @brief  This function clears the WUPn bit of IICCTL01 register.
* @param  None
* @return None
***********************************************************************************************************************/
void IICA1_Clear_Wakeup(void)
{
    IICA1->IICCTL11 &= ~0x80; /* WUP = 0 */
  	__NOP();
  	__NOP();
  	__NOP();	
  	__NOP();
  	__NOP();	
}
/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
