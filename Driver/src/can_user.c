/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    can_user.c
* @brief   This file implements device driver for CAN module.
* @version 1.0.0
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdio.h>
#include "userdefine.h"
#include "BAT32A239.h"
#include "can.h"
#include "rtc.h"

/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//void IRQ23_Handler(void) __attribute__((alias("CAN0Rec_Interrupt")));
//void IRQ25_Handler(void) __attribute__((alias("CAN0Trx_Interrupt")));
//void IRQ24_Handler(void) __attribute__((alias("CAN0Wup_Interrupt")));
//void IRQ09_Handler(void) __attribute__((alias("CAN0Err_Interrupt")));

//void IRQ29_Handler(void) __attribute__((alias("CAN1Rec_Interrupt")));
//void IRQ31_Handler(void) __attribute__((alias("CAN1Trx_Interrupt")));
//void IRQ30_Handler(void) __attribute__((alias("CAN1Wup_Interrupt")));
//void IRQ26_Handler(void) __attribute__((alias("CAN1Err_Interrupt")));


/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
uint32_t RxCount;

void CAN0Rec_Handler(void);
void CAN0Trx_Handler(void);
void CAN0Wup_Handler(void);
void CAN0Err_Handler(void);
void CAN1Rec_Handler(void);
void CAN1Trx_Handler(void);
void CAN1Wup_Handler(void);
void CAN1Err_Handler(void);

extern rtc_counter_value_t date_time;
uint16_t gTemp;

#if defined(CAN0_USE)
uint8_t gCanerror0flag;
CANMSG_REC_Type can0MsgRec[16];
#endif
#if defined(CAN1_USE)
uint8_t gCanerror1flag;
CANMSG_REC_Type can1MsgRec[16];
#endif


/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: MsgRec_Clr
* @brief  This function Clear the Message Data.
* @param  pMsg_Rec - the pointer of Receive Message
* @return None
***********************************************************************************************************************/
void MsgRec_Clr(CANMSG_REC_Type* pMsg_Rec)
{
	uint32_t i,j;
	uint8_t* pMsgDat;
	CANMSG_REC_Type* pTMsg;
	
	for(j=0;j<sizeof(can0MsgRec)/sizeof(CANMSG_REC_Type);j++)
	{
		pTMsg = pMsg_Rec+j;
		pMsgDat = (uint8_t*)pTMsg;
		for(i=0;i<sizeof(CANMSG_REC_Type);i++)
		{
			pMsgDat[i] = 0;
		}
	}
}
#if defined(CAN0_USE)
/***********************************************************************************************************************
* Function Name: CAN0Rec_Interrupt
* @brief  CAN0 reception interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Rec_Interrupt(void)
{
	if(INTC_GetPendingIRQ(CAN0REC_IRQn))
	{
		INTC_ClearPendingIRQ(CAN0REC_IRQn);
		if(CAN0->CINTS & INTS_RX_READ)
		{
			CAN0->CINTS = INTS_RX_CLR;
			CAN0Rec_Handler();
		}
	}
	

}

/***********************************************************************************************************************
* Function Name: CAN0Trx_Interrupt
* @brief  CAN0 transmission interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Trx_Interrupt(void)
{
    INTC_ClearPendingIRQ(CAN0TRX_IRQn);
    if(CAN0->CINTS & INTS_TX_READ)
    {
        CAN0->CINTS = INTS_TX_CLR;
        CAN0Trx_Handler();
    }
}

/***********************************************************************************************************************
* Function Name: CAN0Wup_Interrupt
* @brief  CAN0 wakeup interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Wup_Interrupt(void)
{
    INTC_ClearPendingIRQ(CAN0WUP_IRQn);
    if(CAN0->CINTS & INTS_WK_READ)
    {
	    CAN0->CINTS = INTS_WK_CLR;
        CAN0Wup_Handler();
    }
}

/***********************************************************************************************************************
* Function Name: CAN0Err_Interrupt
* @brief  CAN error interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Err_Interrupt(void)
{
	uint8_t i;
	uint16_t can0ints;
	CANMSG_Type* pMsg;

    INTC_ClearPendingIRQ(CAN0ERR_IRQn);//clear CAN0ERR pending flag
	can0ints = CAN0->CINTS&0x001C;//read error interrupt flag
	CAN0->CINTS = can0ints;	//clear error interrupt flag
	
	if((can0ints&INTS_ERR_READ)&&(can0ints&INTS_PERR_READ))
	{
		gCanerror0flag = 1;
	}
	
#if 1	
	if(can0ints&INTS_ERR_READ)//ERR interrrupt?
	{
		if(CAN0->CINFO&INFO_BOFF)//bus off?
		{
			//recovery bus
			//clear all TRQ
			pMsg = (CANMSG_Type*)CAN0MSG00_BASE;
			
			for(i=0;i<16;i++)//clear all msg buffer TRQ
			{
				while(pMsg->CMCTRL&MCTRL_RDY_READ)
				{
					pMsg->CMCTRL = MCTRL_RDY_CLR;//clear RDY
				}
				pMsg->CMCTRL = MCTRL_TRQ_CLR;//clear TRQ
				
				pMsg++;
			}
			
			CAN0->CCTRL = CTRL_CCERC_CLR| CTRL_AL_CLR| CTRL_VALID_CLR| PSMODE_IDLE| OPMODE_IDLE;//CAN initialize
			//Read can registor
			if(CAN0->CLEC)
			{
				CAN0->CLEC = 0x00;//clear CLEC
			}
			
			//set CCERC
			CAN0->CCTRL = CTRL_CCERC_SET;//clear CCERC
			
			CAN0->CCTRL = OPMODE_NORMAL;//CAN resume
		}
	}
	
	if(can0ints&INTS_PERR_READ)//PERR interrrupt?
	{
		if(CAN0->CLEC)
		{
			CAN0->CLEC = 0x00;//clear CLEC
		}
	}
#endif		
}


/***********************************************************************************************************************
* Function Name: CAN0Rec_Handler
* @brief  This function is CAN0 Receive handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Rec_Handler(void)
{
	CANMSG_Type* pCMsg;
	uint8_t* pMdb;
	uint32_t rgpt;
	uint8_t msgnum,i;
	
	rgpt = CAN0->CRGPT;
	
  if(rgpt&RGPT_ROVF_READ)
	{
		CAN0->CRGPT = RGPT_ROVF_CLR;
	}
	
	while(!(rgpt&RGPT_RHPM_READ))
	{
		msgnum = (rgpt>>8)&0x0f;
		pCMsg = (CANMSG_Type*)CAN0MSG00+msgnum;
		
		pCMsg->CMCTRL = MCTRL_DN_CLR;//clear DN
		can0MsgRec[msgnum].Id = (pCMsg->CMIDH<<16)|(pCMsg->CMIDL);
		can0MsgRec[msgnum].Dlc = pCMsg->CMDLC;
		pMdb = (uint8_t*)&(pCMsg->CMDB0);
		for(i=0;i<can0MsgRec[msgnum].Dlc;i++)
		{
			can0MsgRec[msgnum].Msgbuff[i] = pMdb[i];
		}
		
		can0MsgRec[msgnum].MsgRxStatus = 1;
		RxCount++;
#if 0		
		if((MsgRec[msgnum].Id&0x000007ff)==0x231)
		{
			date_time.year  = pMdb[0];
			date_time.month = pMdb[1];
			date_time.day   = pMdb[2];
			date_time.week  = pMdb[3];
			date_time.hour  = pMdb[4];
			date_time.min   = pMdb[5];
			date_time.sec   = pMdb[6];
			RTC_Set_CounterValue(&date_time);
		}
#endif
//		if((MsgRec[msgnum].Id&0x07ff)==0x702)
//		{
//			PORT->P7 ^= 0x06U;
//		}
#if 1		
		printf("RxCount %d\n",++RxCount);
		printf("Receive a message buffer%d from CAN0\n",msgnum);
		
		printf("ID 0x%08x\n",can0MsgRec[msgnum].Id);
		printf("DLC %d\n",can0MsgRec[msgnum].Dlc);
		for(i=0;i<can0MsgRec[msgnum].Dlc;i++)
		{
			printf("Data%d 0x%02x\n",i,can0MsgRec[msgnum].Msgbuff[i]);
		}
#endif		
		rgpt = CAN0->CRGPT;
	}
	 
}
/***********************************************************************************************************************
* Function Name: CAN0Trx_Handler
* @brief  This function is CAN0 Transmit handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Trx_Handler(void)
{
	while(CAN0MSG00->CMCTRL&MCTRL_RDY_READ)
	{
		CAN0MSG00->CMCTRL = MCTRL_RDY_CLR;
	}
	
	CAN0MSG00->CMIDL = 0;
	CAN0MSG00->CMIDH = (0x731<<2U)&0x1ffc;
	
	CAN0MSG00->CMDB0 = date_time.year;
	CAN0MSG00->CMDB1 = date_time.month;
	CAN0MSG00->CMDB2 = date_time.day;
//	CAN0MSG00->CMDB3 = date_time.week;
	CAN0MSG00->CMDB3 = date_time.hour;
	CAN0MSG00->CMDB4 = date_time.min;
	CAN0MSG00->CMDB5 = date_time.sec;
	CAN0MSG00->CMDB6 = gTemp>>8;
	CAN0MSG00->CMDB7 = gTemp;
	
	CAN0MSG00->CMCTRL = MCTRL_RDY_SET;
	
	CAN0MSG00->CMCTRL = MCTRL_TRQ_SET;
	
}
/***********************************************************************************************************************
* Function Name: CAN0Wup_Handler
* @brief  This function is CAN0 Wake up handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Wup_Handler(void)
{
   
}
/***********************************************************************************************************************
* Function Name: CAN0Err_Handler
* @brief  This function is CAN0 Error handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN0Err_Handler(void)
{
   
}
#endif

#if defined(CAN1_USE)
/***********************************************************************************************************************
* Function Name: CAN1Rec_Interrupt
* @brief  CAN1 reception interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Rec_Interrupt(void)
{
	if(INTC_GetPendingIRQ(CAN1REC_IRQn))
	{
		INTC_ClearPendingIRQ(CAN1REC_IRQn);
		if(CAN1->CINTS & INTS_RX_READ)
		{
			CAN1->CINTS = INTS_RX_CLR;
			CAN1Rec_Handler();
		}
	}
	
}

/***********************************************************************************************************************
* Function Name: CAN1Trx_Interrupt
* @brief  CAN1 transmission interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Trx_Interrupt(void)
{
    INTC_ClearPendingIRQ(CAN1TRX_IRQn);
    if(CAN1->CINTS & INTS_TX_READ)
    {
        CAN1->CINTS = INTS_TX_CLR;
        CAN1Trx_Handler();
    }
}

/***********************************************************************************************************************
* Function Name: CAN1Wup_Interrupt
* @brief  CAN1 wakeup interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Wup_Interrupt(void)
{
    INTC_ClearPendingIRQ(CAN1WUP_IRQn);
    if(CAN1->CINTS & INTS_WK_READ)
    {
	    CAN1->CINTS = INTS_WK_CLR;
        CAN1Wup_Handler();
    }
}

/***********************************************************************************************************************
* Function Name: CAN1Err_Interrupt
* @brief  CAN1 error interrupt service routine
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Err_Interrupt(void)
{
	uint8_t i;
	uint16_t can1ints;
	CANMSG_Type* pMsg;

  INTC_ClearPendingIRQ(CAN1ERR_IRQn);//clear CAN1ERR pending flag
	can1ints = CAN1->CINTS&0x001C;//read error interrupt flag
	CAN1->CINTS = can1ints;	//clear error interrupt flag
	
	if((can1ints&INTS_ERR_READ)&&(can1ints&INTS_PERR_READ))
	{
		gCanerror1flag = 1;
	}
	
#if 1	
	if(can1ints&INTS_ERR_READ)//ERR interrrupt?
	{
		if(CAN1->CINFO&INFO_BOFF)//bus off?
		{
			//recovery bus
			//clear all TRQ
			pMsg = (CANMSG_Type*)CAN1MSG00_BASE;
			
			for(i=0;i<16;i++)//clear all msg buffer TRQ
			{
				while(pMsg->CMCTRL&MCTRL_RDY_READ)
				{
					pMsg->CMCTRL = MCTRL_RDY_CLR;//clear RDY
				}
				pMsg->CMCTRL = MCTRL_TRQ_CLR;//clear TRQ
				
				pMsg++;
			}
			
			CAN1->CCTRL = CTRL_CCERC_CLR| CTRL_AL_CLR| CTRL_VALID_CLR| PSMODE_IDLE| OPMODE_IDLE;//CAN initialize
			//Read can registor
			if(CAN1->CLEC)
			{
				CAN1->CLEC = 0x00;//clear CLEC
			}
			
			//set CCERC
			CAN1->CCTRL = CTRL_CCERC_SET;//clear CCERC
			
			CAN1->CCTRL = OPMODE_NORMAL;//CAN resume
		}
	}
	
	if(can1ints&INTS_PERR_READ)//PERR interrrupt?
	{
		if(CAN1->CLEC)
		{
			CAN1->CLEC = 0x00;//clear CLEC
		}
	}
#endif		
}


/***********************************************************************************************************************
* Function Name: CAN1Rec_Handler
* @brief  This function is CAN1 Receive handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Rec_Handler(void)
{
	CANMSG_Type* pCMsg;
	uint8_t* pMdb;
	uint32_t rgpt;
	uint8_t msgnum,i;
	
	rgpt = CAN1->CRGPT;
	
  if(rgpt&RGPT_ROVF_READ)
	{
		CAN1->CRGPT = RGPT_ROVF_CLR;
	}
	
	while(!(rgpt&RGPT_RHPM_READ))
	{
		msgnum = (rgpt>>8)&0x0f;
		pCMsg = (CANMSG_Type*)CAN1MSG00+msgnum;
		
		pCMsg->CMCTRL = MCTRL_DN_CLR;//clear DN
		can1MsgRec[msgnum].Id = (pCMsg->CMIDH<<16)|(pCMsg->CMIDL);
		can1MsgRec[msgnum].Dlc = pCMsg->CMDLC;
		pMdb = (uint8_t*)&(pCMsg->CMDB0);
		for(i=0;i<can1MsgRec[msgnum].Dlc;i++)
		{
			can1MsgRec[msgnum].Msgbuff[i] = pMdb[i];
		}
		
		can1MsgRec[msgnum].MsgRxStatus = 1;
		RxCount++;
#if 0		
		if((MsgRec[msgnum].Id&0x000007ff)==0x231)
		{
			date_time.year  = pMdb[0];
			date_time.month = pMdb[1];
			date_time.day   = pMdb[2];
			date_time.week  = pMdb[3];
			date_time.hour  = pMdb[4];
			date_time.min   = pMdb[5];
			date_time.sec   = pMdb[6];
			RTC_Set_CounterValue(&date_time);
		}
#endif
//		if((MsgRec[msgnum].Id&0x07ff)==0x702)
//		{
//			PORT->P7 ^= 0x06U;
//		}
#if 1		
		printf("RxCount %d\n",++RxCount);
		printf("Receive a message buffer%d from CAN1\n",msgnum);
		
		printf("ID 0x%08x\n",can1MsgRec[msgnum].Id);
		printf("DLC %d\n",can1MsgRec[msgnum].Dlc);
		for(i=0;i<can1MsgRec[msgnum].Dlc;i++)
		{
			printf("Data%d 0x%02x\n",i,can1MsgRec[msgnum].Msgbuff[i]);
		}
#endif		
		rgpt = CAN1->CRGPT;
	}
	 
}
/***********************************************************************************************************************
* Function Name: CAN1Trx_Handler
* @brief  This function is CAN1 Transmit handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Trx_Handler(void)
{
	while(CAN1MSG00->CMCTRL&MCTRL_RDY_READ)
	{
		CAN1MSG00->CMCTRL = MCTRL_RDY_CLR;
	}
	
	CAN1MSG00->CMIDL = 0;
	CAN1MSG00->CMIDH = (0x731<<2U)&0x1ffc;
	
	CAN1MSG00->CMDB0 = date_time.year;
	CAN1MSG00->CMDB1 = date_time.month;
	CAN1MSG00->CMDB2 = date_time.day;
//	CAN1MSG00->CMDB3 = date_time.week;
	CAN1MSG00->CMDB3 = date_time.hour;
	CAN1MSG00->CMDB4 = date_time.min;
	CAN1MSG00->CMDB5 = date_time.sec;
	CAN1MSG00->CMDB6 = gTemp>>8;
	CAN1MSG00->CMDB7 = gTemp;
	
	CAN1MSG00->CMCTRL = MCTRL_RDY_SET;
	
	CAN1MSG00->CMCTRL = MCTRL_TRQ_SET;
	
}
/***********************************************************************************************************************
* Function Name: CAN1Wup_Handler
* @brief  This function is CAN1 Wake up handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Wup_Handler(void)
{
   
}
/***********************************************************************************************************************
* Function Name: CAN1Err_Handler
* @brief  This function is CAN1 Error handler.
* @param  None
* @return None
***********************************************************************************************************************/
void CAN1Err_Handler(void)
{
   
}
#endif

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
