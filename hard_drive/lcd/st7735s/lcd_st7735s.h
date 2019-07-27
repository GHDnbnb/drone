#ifndef _LCD_ST7735S_H_
#define _LCD_ST7735S_H_

#include "r_cg_userdefine.h"

//用户配置
#define ST7735S_DIR_DEFAULT     0       //定义默认方向 (0~3)

#if (USE_LCD == LCD_ST7735S)


/*********************** 移植配置 ******************************/
#define uint8   uint8_t
#define uint16  uint16_t
#define uint32  uint32_t

/*********************** API接口 ******************************/
//提供API接口给LCD调用

#define LCD_H                   ST7735S_get_h()                 //高
#define LCD_W                   ST7735S_get_w()                 //宽

#define LCD_INIT()              LCD_ST7735S_init()              //初始化
#define LCD_PTLON(site,size)    LCD_ST7735S_ptlon(site,size)    //开窗
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //写模式
#define LCD_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //写数据
#define LCD_WR_CMD(cmd)         LCD_ST7735S_WR_8CMD(cmd)        //命令
#define LCD_SET_DIR(opt)        LCD_ST7735S_dir(opt)            //方向

#define LCD_DIR                 ST7735S_get_dir()               //获取方向


//通过函数调用，速度太慢了，改成宏定义调用
//#define LCD_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //写数据
//#define LCD_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //命令

#endif  //(USE_LCD == LCD_ST7735S)

/********************* ST7735S内部实现 ***********************/
//LCD 的管脚定义
#define     LCD_ST7735S_WR      PTC9
#define     LCD_ST7735S_RD      PTC10
#define     LCD_ST7735S_CS      PTC4
#define     LCD_ST7735S_RS      PTC12
#define     LCD_ST7735S_RST     PTC8

//此部分需要修改初始化里的代码
#define     LCD_ST7735S_P0              PORTB.PODR.BYTE
#define     LCD_ST7735S_PIN             PORTB.PIDR.BYTE
#define     LCD_ST7735S_PDDR_OUT()      (DDRC_B0 = 0xff)
#define     LCD_ST7735S_PDDR_IN()       (DDRC_B0 = 0)


#define ST7735S_DELAY()                         //do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)
#define ST7735S_DELAY_MS(ms)    delay_ms(ms)


extern void     LCD_ST7735S_init();
extern void     LCD_ST7735S_dir(unsigned char option);
extern void     LCD_ST7735S_ptlon(Site_t site, Size_t size);
extern unsigned short   ST7735S_get_h();
extern unsigned short   ST7735S_get_w();
extern unsigned char    ST7735S_get_dir();

extern void LCD_ST7735S_wr_data(unsigned char data);
extern void LCD_ST7735S_wr_cmd(unsigned char cmd);

#define     LCD_ST7735S_WR_OUT      PORT0.PODR.BIT.B2
#define     LCD_ST7735S_RD_OUT      PORTD.PODR.BIT.B7
#define     LCD_ST7735S_CS_OUT      PORT0.PODR.BIT.B0
#define     LCD_ST7735S_RS_OUT      PORTD.PODR.BIT.B6
#define     LCD_ST7735S_RST_OUT     PORT0.PODR.BIT.B1

#define LCD_ST7735S_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=0;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(cmd);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;产生一个上升沿

#define LCD_ST7735S_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=1;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(data);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;在这里写入数据到RAM


#define LCD_ST7735S_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //写数据



#endif  //_VCAN_LCD_ST7735S_H_
