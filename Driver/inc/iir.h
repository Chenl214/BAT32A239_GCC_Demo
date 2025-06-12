/***********************************************************************************************************************
* Copyright (C) All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* @file    iir.h
* @brief   This file implements device driver for IIR module.
* @version 1.0.0 
* @date    2019/12/24
***********************************************************************************************************************/
#ifndef IIR_H
#define IIR_H

#include <stdint.h>
#include "userdefine.h"

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-function"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum{
    IIR0_SndOrder_Filter        = 0x00000001U,
    IIR1_SndOrder_Filter        = 0x00000010U,
    IIR1_IIR0_FourthOrderFilter = 0x00000011U,
} iir_filter_type_t;

typedef struct{
    uint8_t a1;
    uint8_t a2;
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t dummy1;
    uint8_t dummy2;
    uint8_t dummy3;
} iir_coef_type_t;

typedef struct{
    iir_filter_type_t  filterType;
    uint32_t           sampleNum;
    iir_coef_type_t *  piir0coef;
    iir_coef_type_t *  piir1coef;
    uint32_t *         piirInputData;
    uint32_t *         piirOutputData;
} iir_control_data_t;

/***********************************************************************************************************************
Global variables
***********************************************************************************************************************/
extern iir_coef_type_t st_iir0_coef;
extern iir_coef_type_t st_iir1_coef;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
MD_STATUS IIR_Filter_Init(iir_control_data_t * p_cntl_data);
MD_STATUS IIR_Filter_Function(iir_control_data_t * p_cntl_data);
void IIR_Close(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
