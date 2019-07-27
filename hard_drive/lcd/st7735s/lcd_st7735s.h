#ifndef _LCD_ST7735S_H_
#define _LCD_ST7735S_H_

#include "r_cg_userdefine.h"

//�û�����
#define ST7735S_DIR_DEFAULT     0       //����Ĭ�Ϸ��� (0~3)

#if (USE_LCD == LCD_ST7735S)


/*********************** ��ֲ���� ******************************/
#define uint8   uint8_t
#define uint16  uint16_t
#define uint32  uint32_t

/*********************** API�ӿ� ******************************/
//�ṩAPI�ӿڸ�LCD����

#define LCD_H                   ST7735S_get_h()                 //��
#define LCD_W                   ST7735S_get_w()                 //��

#define LCD_INIT()              LCD_ST7735S_init()              //��ʼ��
#define LCD_PTLON(site,size)    LCD_ST7735S_ptlon(site,size)    //����
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //дģʽ
#define LCD_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //д����
#define LCD_WR_CMD(cmd)         LCD_ST7735S_WR_8CMD(cmd)        //����
#define LCD_SET_DIR(opt)        LCD_ST7735S_dir(opt)            //����

#define LCD_DIR                 ST7735S_get_dir()               //��ȡ����


//ͨ���������ã��ٶ�̫���ˣ��ĳɺ궨�����
//#define LCD_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       //д����
//#define LCD_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         //����

#endif  //(USE_LCD == LCD_ST7735S)

/********************* ST7735S�ڲ�ʵ�� ***********************/
//LCD �ĹܽŶ���
#define     LCD_ST7735S_WR      PTC9
#define     LCD_ST7735S_RD      PTC10
#define     LCD_ST7735S_CS      PTC4
#define     LCD_ST7735S_RS      PTC12
#define     LCD_ST7735S_RST     PTC8

//�˲�����Ҫ�޸ĳ�ʼ����Ĵ���
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
                            }while(0)   //LCD_WR=0;LCD_WR=1;����һ��������

#define LCD_ST7735S_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=1;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0=(uint8)(data);\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;������д�����ݵ�RAM


#define LCD_ST7735S_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((uint8)((data)>>8) );LCD_ST7735S_WR_8DATA( (uint8)(data) );}while(0)       //д����



#endif  //_VCAN_LCD_ST7735S_H_
