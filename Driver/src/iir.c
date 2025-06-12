/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    iir.c
* @brief   This file implements device driver for IIR module.
* @version 1.0.0  
* @date    2019/12/24
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "userdefine.h"
#include "BAT32G439.h"
#include "iir.h"
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
// !!!! Please specific these structure depend on your application !!!!
#warning "Please specific these parameters depend on your application! Here is just a usage demo."
iir_coef_type_t st_iir0_coef = {1,1,1,1,1,0xFF,0xFF,0xFF,};    // only low 4 bit valid
iir_coef_type_t st_iir1_coef = {1,1,1,1,1,0xFF,0xFF,0xFF,};    // only low 4 bit valid

/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: IIR_Filter_Init
* @brief  This function is second-order filter demo by IIR0/IIR1 module.
* @param  None
* @return None
***********************************************************************************************************************/
MD_STATUS IIR_Filter_Init(iir_control_data_t * p_cntl_data)
{
    MD_STATUS status = MD_OK;
    CGC->PERA |= CGC_PERA_IIREN_Msk;
    switch(p_cntl_data->filterType){
        case IIR0_SndOrder_Filter:
            IIR->I0IIRCTL |= IIR_I0IIRCTL_IIREN_Msk;

            IIR->I0A1COEF = p_cntl_data->piir0coef->a1;
            IIR->I0A2COEF = p_cntl_data->piir0coef->a2;
            IIR->I0B0COEF = p_cntl_data->piir0coef->b0;
            IIR->I0B1COEF = p_cntl_data->piir0coef->b1;
            IIR->I0B2COEF = p_cntl_data->piir0coef->b2;
            break;

        case IIR1_SndOrder_Filter:
            IIR->I1IIRCTL |= IIR_I1IIRCTL_IIREN_Msk;

            IIR->I1A1COEF = p_cntl_data->piir1coef->a1;
            IIR->I1A2COEF = p_cntl_data->piir1coef->a2;
            IIR->I1B0COEF = p_cntl_data->piir1coef->b0;
            IIR->I1B1COEF = p_cntl_data->piir1coef->b1;
            IIR->I1B2COEF = p_cntl_data->piir1coef->b2;
            break;

        case IIR1_IIR0_FourthOrderFilter:
            IIR->I0IIRCTL |= IIR_I0IIRCTL_IIREN_Msk;
            IIR->I1IIRCTL |= IIR_I1IIRCTL_IIREN_Msk;
            IIR->IIRMDCTL |= IIR_IIRMDCTL_IIRMD_Msk;

            IIR->I0A1COEF = p_cntl_data->piir0coef->a1;
            IIR->I0A2COEF = p_cntl_data->piir0coef->a2;
            IIR->I0B0COEF = p_cntl_data->piir0coef->b0;
            IIR->I0B1COEF = p_cntl_data->piir0coef->b1;
            IIR->I0B2COEF = p_cntl_data->piir0coef->b2;

            IIR->I1A1COEF = p_cntl_data->piir1coef->a1;
            IIR->I1A2COEF = p_cntl_data->piir1coef->a2;
            IIR->I1B0COEF = p_cntl_data->piir1coef->b0;
            IIR->I1B1COEF = p_cntl_data->piir1coef->b1;
            IIR->I1B2COEF = p_cntl_data->piir1coef->b2;
            break;

        default:
            status = MD_ERROR;
            break;
    }
    return status;
}
/***********************************************************************************************************************
* Function Name: IIR_Close
* @brief  This function Close the IIR0/IIR1 module.
* @param  None
* @return None
***********************************************************************************************************************/
void IIR_Close(void)
{
    CGC->PERA &= ~CGC_PERA_IIREN_Msk;
}
/***********************************************************************************************************************
* Function Name: IIR_FourthOrder_Filter_Function
* @brief  This function is fourth-order filter demo by IIR0/IIR1 module.
* @param  None
* @return MD_OK or MD_ERROR
***********************************************************************************************************************/
MD_STATUS IIR_Filter_Function(iir_control_data_t * p_cntl_data)
{
    MD_STATUS status = MD_OK;
    uint32_t i, Number;

    if (p_cntl_data->sampleNum > 0){
        Number = p_cntl_data->sampleNum;
    }
    else{
        return MD_ERROR;
    }

    switch(p_cntl_data->filterType) {
        case IIR0_SndOrder_Filter:      // IIR0 second-order filter
            for (i = 0; i < Number; i++){
                IIR->I0DATAIN = p_cntl_data->piirInputData[i];          // input sample signal
                //while(IIR->I0IIRCTL&0x800){}
                __NOP();
                __NOP();
                __NOP();
                __NOP();
                *(p_cntl_data->piirOutputData + i) = IIR->I0DATAOUT;    // filter output
            }
            break;

        case IIR1_SndOrder_Filter:      // IIR1 second-order filter
            for (i = 0; i < Number; i++){
                IIR->I1DATAIN = p_cntl_data->piirInputData[i];          // input sample signal
                //while(IIR->I1IIRCTL&0x800){}
                __NOP();
                __NOP();
                __NOP();
                __NOP();
                *(p_cntl_data->piirOutputData + i) = IIR->I1DATAOUT;    // filter output
                //p_cntl_data->piirOutputData[i] = IIR->I1DATAOUT;      // filter output
            }
            break;

        case IIR1_IIR0_FourthOrderFilter: // IIR0->IIR1 fourth-order filter
            for (i = 0; i < Number; i++){
                IIR->I0DATAIN = p_cntl_data->piirInputData[i];          // input sample signal
                //while(IIR->I1IIRCTL&0x800){}
                __NOP();
                __NOP();
                __NOP();
                __NOP();
                *(p_cntl_data->piirOutputData + i) = IIR->I1DATAOUT;    // filter output
                //p_cntl_data->piirOutputData[i] = IIR->I1DATAOUT;      // filter output
            }
            break;

        default:
            status = MD_ERROR;
            break;
    }
    return status;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
