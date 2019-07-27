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
* File Name    : r_cg_userdefine.h
* Version      : Applilet4 for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file includes user definition.
* Creation Date: 2017/8/2
***********************************************************************************************************************/
#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/
#define FAST_INTERRUPT_VECTOR 0

/* Start user code for function. Do not edit comment generated here */
#include "stdio.h"
#include "r_cg_port.h"
#include "r_cg_macrodriver.h"

/*
 * 定义坐标结构体
 */
typedef struct
{
    uint16_t x;
    uint16_t y;
}Site_t;

/*
 * 定义矩形大小结构体
 */
typedef struct
{
    uint16_t W;       //宽
    uint16_t H;       //高
}Size_t;

typedef struct
{
  uint8_t start_point;//起始行
  uint8_t end_point;//结束行
  uint8_t white_points;
}Row_Feature;

/*
 * 定义图像特征结构体
 */
typedef struct
{
  Row_Feature row[60];//每行白点个数
  uint8_t start_row;//白点出现行
  uint8_t end_row;//白点消失行
}Image_Feature;

extern uint8_t ov7725_vsync;

void delay_us(uint16_t utime);
void delay_ms(uint16_t utime);
/* End user code. Do not edit comment generated here */
#endif