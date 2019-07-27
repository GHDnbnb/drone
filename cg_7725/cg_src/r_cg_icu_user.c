/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_icu_user.c
* Version      : Applilet4 for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for ICU module.
* Creation Date: 2017/8/2
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_icu.h"
/* Start user code for include. Do not edit comment generated here */
#include "ov7725.h"
#include "r_cg_port.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
uint8_t ov7725_vsync = 0;//记录场中断标志
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_icu_irq5_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_ICU_IRQ5
#if FAST_INTERRUPT_VECTOR == VECT_ICU_IRQ5
__fast_interrupt static void r_icu_irq5_interrupt(void)
#else
__interrupt static void r_icu_irq5_interrupt(void)
#endif
{
    /* Start user code. Do not edit comment generated here */
    if(ov7725_vsync == 0)
    {
      FIFO_WRST_L(); 	                      //拉低使FIFO写(数据from摄像头)指针复位
      FIFO_WE_H();	                        //拉高使FIFO写允许
      ov7725_vsync = 1;	   	
//      FIFO_WE_H();                          //使FIFO写允许
      FIFO_WRST_H();                        //允许使FIFO写(数据from摄像头)指针运动
    }
    else if(ov7725_vsync == 1)
    {
//      R_ICU_IRQ5_Stop();
      FIFO_WE_L();                          //拉低使FIFO写暂停
      ov7725_vsync = 2;
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
