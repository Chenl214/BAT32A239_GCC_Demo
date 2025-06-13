/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    intp.c
* @brief   This file implements device driver for INTP module.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "userdefine.h"
#include "BAT32A239.h"
#include "intp.h"
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
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: INTP_Init
* @brief  This function initializes the external interrupt module.
* @param  pinMsk - select the active external interrupt pins
* @param  edge - specify the active edge of external interrupt
* @return None
***********************************************************************************************************************/
void INTP_Init(uint16_t pinMsk, intp_edge_t edge)
{
    volatile uint8_t w_count;
    /* Set INTPn pin */
    if(pinMsk & (1 << 0))
    {
        INTP0_PORT_SETTING();
        /* disable INTP0 interrupt */
        INTC_DisableIRQ(INTP0_IRQn);        
        /* clear INTP0 interrupt flag */
        INTC_ClearPendingIRQ(INTP0_IRQn); 
        INTM->EGN0 &= ~(1 << 0);
        INTM->EGP0 &= ~(1 << 0);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 0;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 0;
    }
    if(pinMsk & (1 << 1))
    {
        INTP1_PORT_SETTING();
        /* disable INTP1 interrupt */
        INTC_DisableIRQ(INTP1_IRQn);        
        /* clear INTP1 interrupt flag */
        INTC_ClearPendingIRQ(INTP1_IRQn); 
        INTM->EGN0 &= ~(1 << 1);
        INTM->EGP0 &= ~(1 << 1);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 1;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 1;
    }
    if(pinMsk & (1 << 2))
    {
        INTP2_PORT_SETTING();
        /* disable INTP2 interrupt */
        INTC_DisableIRQ(INTP2_IRQn);        
        /* clear INTP2 interrupt flag */
        INTC_ClearPendingIRQ(INTP2_IRQn); 
        INTM->EGN0 &= ~(1 << 2);
        INTM->EGP0 &= ~(1 << 2);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 2;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 2;
    }
    if(pinMsk & (1 << 3))
    {
        INTP3_PORT_SETTING();
        /* disable INTP3 interrupt */
        INTC_DisableIRQ(INTP3_IRQn);        
        /* clear INTP3 interrupt flag */
        INTC_ClearPendingIRQ(INTP3_IRQn); 
        INTM->EGN0 &= ~(1 << 3);
        INTM->EGP0 &= ~(1 << 3);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 3;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 3;
    }
    if(pinMsk & (1 << 4))
    {
        INTP4_PORT_SETTING();
        /* disable INTP4 interrupt */
        INTC_DisableIRQ(INTP4_IRQn);        
        /* clear INTP4 interrupt flag */
        INTC_ClearPendingIRQ(INTP4_IRQn); 
        INTM->EGN0 &= ~(1 << 4);
        INTM->EGP0 &= ~(1 << 4);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 4;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 4;
    }
    if(pinMsk & (1 << 5))
    {
        INTP5_PORT_SETTING();
        /* disable INTP5 interrupt */
        INTC_DisableIRQ(INTP5_IRQn);        
        /* clear INTP5 interrupt flag */
        INTC_ClearPendingIRQ(INTP5_IRQn); 
        INTM->EGN0 &= ~(1 << 5);
        INTM->EGP0 &= ~(1 << 5);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 5;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 5;
    }
    if(pinMsk & (1 << 6))
    {
        INTP6_PORT_SETTING();
        /* disable INTP6 interrupt */
        INTC_DisableIRQ(INTP6_IRQn);        
        /* clear INTP6 interrupt flag */
        INTC_ClearPendingIRQ(INTP6_IRQn); 
        INTM->EGN0 &= ~(1 << 6);
        INTM->EGP0 &= ~(1 << 6);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 6;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 6;
    }
    if(pinMsk & (1 << 7))
    {
        INTP7_PORT_SETTING();
        /* disable INTP7 interrupt */
        INTC_DisableIRQ(INTP7_IRQn);        
        /* clear INTP7 interrupt flag */
        INTC_ClearPendingIRQ(INTP7_IRQn); 
        INTM->EGN0 &= ~(1 << 7);
        INTM->EGP0 &= ~(1 << 7);
        INTM->EGN0 |= ((edge & 0x01U) >> 0) << 7;
        INTM->EGP0 |= ((edge & 0x02U) >> 1) << 7;
    }
    if(pinMsk & (1 << 8))
    {
        INTP8_PORT_SETTING();
        /* disable INTP8 interrupt */
        INTC_DisableIRQ(INTP8_IRQn);        
        /* clear INTP8 interrupt flag */
        INTC_ClearPendingIRQ(INTP8_IRQn); 
        INTM->EGN1 &= ~(1 << 0);
        INTM->EGP1 &= ~(1 << 0);
        INTM->EGN1 |= ((edge & 0x01U) >> 0) << 0;
        INTM->EGP1 |= ((edge & 0x02U) >> 1) << 0;
    }
    if(pinMsk & (1 << 9))
    {
        INTP9_PORT_SETTING();
        /* disable INTP9 interrupt */
        INTC_DisableIRQ(INTP9_IRQn);        
        /* clear INTP9 interrupt flag */
        INTC_ClearPendingIRQ(INTP9_IRQn); 
        INTM->EGN1 &= ~(1 << 1);
        INTM->EGP1 &= ~(1 << 1);
        INTM->EGN1 |= ((edge & 0x01U) >> 0) << 1;
        INTM->EGP1 |= ((edge & 0x02U) >> 1) << 1;
    }
    if(pinMsk & (1 << 10))
    {
        INTP10_PORT_SETTING();
        /* disable INTP10 interrupt */
        INTC_DisableIRQ(INTP10_IRQn);       
        /* clear INTP10 interrupt flag */
        INTC_ClearPendingIRQ(INTP10_IRQn); 
        INTM->EGN1 &= ~(1 << 2);
        INTM->EGP1 &= ~(1 << 2);
        INTM->EGN1 |= ((edge & 0x01U) >> 0) << 2;
        INTM->EGP1 |= ((edge & 0x02U) >> 1) << 2;
    }
    if(pinMsk & (1 << 11))
    {
        INTP11_PORT_SETTING();
        /* disable INTP11 interrupt */
        INTC_DisableIRQ(INTP11_IRQn);       
        /* clear INTP11 interrupt flag */
        INTC_ClearPendingIRQ(INTP11_IRQn); 
        INTM->EGN1 &= ~(1 << 3);
        INTM->EGP1 &= ~(1 << 3);
        INTM->EGN1 |= ((edge & 0x01U) >> 0) << 3;
        INTM->EGP1 |= ((edge & 0x02U) >> 1) << 3;
    }
    
    /* Change the waiting time according to the system  */
    for (w_count = 0U; w_count <= INTP_WAITTIME; w_count++)
    {   
        __NOP();
    }
}
/***********************************************************************************************************************
* Function Name: INTP_Start
* @brief  This function clears INTP interrupt flag and enables interrupt.
* @param  pinMsk - select the active external interrupt pins
* @return None
***********************************************************************************************************************/
void INTP_Start(uint16_t pinMsk)
{
    if(pinMsk & (1 << 0))
    {
        INTC_ClearPendingIRQ(INTP0_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP0_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 1))
    {
        INTC_ClearPendingIRQ(INTP1_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP1_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 2))
    {
        INTC_ClearPendingIRQ(INTP2_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP2_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 3))
    {
        INTC_ClearPendingIRQ(INTP3_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP3_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 4))
    {
        INTC_ClearPendingIRQ(INTP4_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP4_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 5))
    {
        INTC_ClearPendingIRQ(INTP5_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP5_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 6))
    {
        INTC_ClearPendingIRQ(INTP6_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP6_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 7))
    {
        INTC_ClearPendingIRQ(INTP7_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP7_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 8))
    {
        INTC_ClearPendingIRQ(INTP8_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP8_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 9))
    {
        INTC_ClearPendingIRQ(INTP9_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP9_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 10))
    {
        INTC_ClearPendingIRQ(INTP10_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP10_IRQn);       /* enable INTP interrupt */
    }
    if(pinMsk & (1 << 11))
    {
        INTC_ClearPendingIRQ(INTP11_IRQn); /* clear INTP interrupt flag */
        INTC_EnableIRQ(INTP11_IRQn);       /* enable INTP interrupt */
    }
}
/***********************************************************************************************************************
* Function Name: INTP_Stop
* @brief  This function disables INTP interrupt and clears interrupt flag.
* @param  pinMsk - select the active external interrupt pins
* @return None
***********************************************************************************************************************/
void INTP_Stop(uint16_t pinMsk)
{
    if(pinMsk & (1 << 0))
    {
        INTC_DisableIRQ(INTP0_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP0_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 1))
    {
        INTC_DisableIRQ(INTP1_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP1_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 2))
    {
        INTC_DisableIRQ(INTP2_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP2_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 3))
    {
        INTC_DisableIRQ(INTP3_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP3_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 4))
    {
        INTC_DisableIRQ(INTP4_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP4_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 5))
    {
        INTC_DisableIRQ(INTP5_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP5_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 6))
    {
        INTC_DisableIRQ(INTP6_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP6_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 7))
    {
        INTC_DisableIRQ(INTP7_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP7_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 8))
    {
        INTC_DisableIRQ(INTP8_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP8_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 9))
    {
        INTC_DisableIRQ(INTP9_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP9_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 10))
    {
        INTC_DisableIRQ(INTP10_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP10_IRQn);  /* clear INTP interrupt flag */
    }
    if(pinMsk & (1 << 11))
    {
        INTC_DisableIRQ(INTP11_IRQn);       /* disable INTP interrupt */
        INTC_ClearPendingIRQ(INTP11_IRQn);  /* clear INTP interrupt flag */
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
