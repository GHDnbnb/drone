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
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements main function.
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
#include "r_cg_cgc.h"
#include "r_cg_icu.h"
#include "r_cg_port.h"
#include "r_cg_tmr.h"
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
#include "ov7725.h"
#include "lcd.h"
#include "delay.h"
uint8_t error[2];
uint8_t temp;
uint8_t x1=0x11;
uint8_t x2=0x22;
uint8_t x3=0x33;
uint8_t x4=0x44;
void mode1();
void mode2();
void mode3();
void mode4();
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */


/* End user code. Do not edit comment generated here */

/* Set option bytes */
#if __LITTLE_ENDIAN__
#pragma public_equ="__MDES",0xFFFFFFFFUL
#else
#pragma public_equ="__MDES",0xFFFFFFF8UL
#endif
#pragma public_equ="__OFS0",0xFFFFFFFFUL
#pragma public_equ="__OFS1",0xFFFFFFFFUL


static void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    
    R_TMR0_Start();
    temp=PORT9.PIDR.BYTE;
    switch(temp)
    {
    case 0x1c:
      R_SCI1_Serial_Send(&x1,1);  mode1();  break;

    case 0x1a:
      R_SCI1_Serial_Send(&x2,1);  mode2();  break;

    case 0x16:
      R_SCI1_Serial_Send(&x3,1);  mode3();  break;
    
    case 0x0e:
      R_SCI1_Serial_Send(&x4,1);  mode4();  break;
    default:
      mode1();
      break;
    }

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    DI();
    while(0 == OV7725_Init());//初始化OV7725
    OV7725_Light_Mode(0);
//    OV7725_Special_Effect(0);
    OV7725_Brightness(-2);//光照度
    OV7725_Color_Saturation(0);//饱和度
    OV7725_Contrast(1);//对比度
    ov7725_vsync = 0;
    R_ICU_IRQ5_Start();
    R_SCI1_Start();
    EI();
    LCD_init();
    LCD_str((Site_t){24,10}, "LCD_init", WHITE, BLACK);
    //LCD_str((Site_t){20,100}, "X:", WHITE, RED);
    //LCD_str((Site_t){60,100}, "Y:", WHITE, RED);
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
void mode1()
{
    LCD_str((Site_t){24,100}, "MODE1", WHITE, BLACK);

    //激光
    while(1U)
    {
      camera_black();                            //起飞悬停
    }  
}

void mode2()
{
    LCD_str((Site_t){24,100}, "MODE2", WHITE, BLACK);      //机车识别
}

void mode3()
{
    LCD_str((Site_t){24,100}, "MODE3", WHITE, BLACK);
    //激光
    uint16_t i=264;
    while(i--)
    {
      camera_black();                            //起飞
    }
    while(1U)
    {
      camera_red();                              //跟随小车
    }
}

void mode4()
{
    LCD_str((Site_t){24,100}, "MODE4", WHITE, BLACK);
    while(1U)
    {
       camera_black();
    }
}
/* End user code. Do not edit comment generated here */