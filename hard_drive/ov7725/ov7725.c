/**
******************************************************************************
* @file    bsp_ov7725.c
* @version V1.0
* @date    2013-xx-xx
* @brief   OV7725����
******************************************************************************
*/ 

#include "ov7725.h"
#include "lcd.h"
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
#include "r_cg_icu.h"
#include "r_cg_tmr.h"
#include "r_cg_sci.h"
uint8_t  imgBuf[60][10] = { 0 };
Image_Feature image_feature;

uint16_t Camera_Data = 0;
uint8_t  RED_value = 0;
uint8_t  GREEN_value = 0;
uint8_t  BLUE_value = 0;
uint16_t Camera_FPS = 0;
extern uint8_t error[2];
uint8_t error1,error2;

//����ͷ��ʼ������
//ע�⣺ʹ�����ַ�ʽ��ʼ���ṹ�壬Ҫ��c/c++ѡ����ѡ�� C99 mode
OV7725_MODE_PARAM cam_mode =
{
  /*���°�����������ͷ���ã������в��ԣ�����һ�飬����������ע�͵�����*/
  /************����1*********������ʾ*****************************/
  
  .QVGA_VGA = 0,	//QVGAģʽ
  .cam_sx = 0,
  .cam_sy = 0,	
  
  .cam_width = 320,
  .cam_height = 240,
  
  .lcd_sx = 0,
  .lcd_sy = 0,
  .lcd_scan = 3, //LCDɨ��ģʽ�����������ÿ���1��3��5��7ģʽ
  
  //���¿ɸ����Լ�����Ҫ������������Χ���ṹ�����Ͷ���	
  .light_mode = 0,//�Զ�����ģʽ
  .saturation = 0,	
  .brightness = 0,
  .contrast = 0,
  .effect = 0,		//����ģʽ
  
  
  /**********����2*********������ʾ****************************/	
  /*������ʾ��ҪVGAģʽ��ͬ�ֱ�������£���QVGA֡���Ե�*/
  /*VGAģʽ�ֱ���Ϊ640*480������ȡ��240*320��ͼ�����������ʾ*/
  /*�����̲�֧�ֳ���320*240�� 240*320�ķֱ�������*/
  
  //	.QVGA_VGA = 1,	//VGAģʽ
  //	//ȡVGAģʽ���еĴ��ڣ��ɸ���ʵ����Ҫ����
  //	.cam_sx = (640-240)/2,
  //	.cam_sy = (480-320)/2,	
  //	
  //	.cam_width = 240, 
  //	.cam_height = 320, //��VGAģʽ�£���ֵ�ſ��Դ���240
  //	
  //	.lcd_sx = 0,
  //	.lcd_sy = 0,
  //	.lcd_scan = 0, //LCDɨ��ģʽ�����������ÿ���0��2��4��6ģʽ
  //	
  //	//���¿ɸ����Լ�����Ҫ������������Χ���ṹ�����Ͷ���
  //	.light_mode = 0,//�Զ�����ģʽ
  //	.saturation = 0,	
  //	.brightness = 0,
  //	.contrast = 0,
  //	.effect = 0,		//����ģʽ
  
  
  /*******����3************С�ֱ���****************************/	
  /*С��320*240�ֱ��ʵģ���ʹ��QVGAģʽ,���õ�ʱ��ע��Һ�����߽�*/
  
  //	.QVGA_VGA = 0,	//QVGAģʽ
  //	//ȡQVGAģʽ���еĴ��ڣ��ɸ���ʵ����Ҫ����
  //	.cam_sx = (320-100)/2,
  //	.cam_sy = (240-150)/2,	
  //	
  //	.cam_width = 100, 
  //	.cam_height = 150, 
  //	
  //	/*Һ��������ʾλ��Ҳ���Ը�����Ҫ������ע�ⲻҪ�����߽缴��*/
  //	.lcd_sx = 50,
  //	.lcd_sy = 50,
  //	.lcd_scan = 3, //LCDɨ��ģʽ��0-7ģʽ��֧�֣�ע�ⲻҪ�����߽缴��
  
  //	//���¿ɸ����Լ�����Ҫ������������Χ���ṹ�����Ͷ���	
  //	.light_mode = 0,//�Զ�����ģʽ
  //	.saturation = 0,	
  //	.brightness = 0,
  //	.contrast = 0,
  //	.effect = 0,		//����ģʽ
  
};


typedef struct Reg
{
  uint8_t Address;			       /*�Ĵ�����ַ*/
  uint8_t Value;		           /*�Ĵ���ֵ*/
}Reg_Info;

/* �Ĵ����������� */
Reg_Info Sensor_Config[] =
{
//  {REG_SCAL0,           0x0A},
//  {REG_AWB_Ctrl0,       0xE0},
//  {REG_DSPAuto,         0xff},
//  {REG_DSP_Ctrl2,       0x0C},
//  {REG_DSP_Ctrl3,       0x00},
//  {REG_DSP_Ctrl4,       0x00},
//  {REG_SDE,		0x06},	//��ɫ	����SDE����Ĵ���������ʵ������Ч��
  
  {REG_COM7,          0x46},
  
  {REG_COM3,          0xD0},
  {REG_COM4,          0x41}, /* PLL x4 */
  {REG_CLKRC,         0x00}, /* Res/Bypass pre-scalar */
  
  // VGA Window Size
  {REG_HSTART,        0x3F},
  {REG_HSIZE,         0x50},
  {REG_VSTRT,         0x03},
  {REG_VSIZE,         0x78},
  {REG_HREF,          0x00},
  
  // Scale down to QVGA Resoultion
  {REG_HOutSize,      0x14},
  {REG_VOutSize,      0x1E},
  
  {REG_COM12,         0x03},
  {REG_EXHCH,         0x00},
  {REG_TGT_B,         0x7F},
  {REG_FixGain,       0x09},
  {REG_AWB_Ctrl0,     0x01},//�ص� //E0
  {REG_DSP_Ctrl1,     0xFF},
  
  {REG_DSP_Ctrl2,     0x0C},
  
  {REG_DSP_Ctrl3,     0x00},
  {REG_DSP_Ctrl4,     0x00},
  {REG_DSPAuto,       0xFF},
  
  {REG_COM8,          0x00},//F0
  {REG_COM6,          0xC5},
  {REG_COM9,          0x21},
  {REG_BDBase,        0x7F},
  {REG_BDMStep,        0x03},
  {REG_AEW,           0x96},
  {REG_AEB,           0x64},
  {REG_VPT,           0xA1},
  {REG_EXHCL,         0x00},
  {REG_AWBCtrl3,     0xAA},
  
  //Gamma
  {REG_GAM1,          0x0C},
  {REG_GAM2,          0x16},
  {REG_GAM3,          0x2A},
  {REG_GAM4,          0x4E},
  {REG_GAM5,          0x61},
  {REG_GAM6,          0x6F},
  {REG_GAM7,          0x7B},
  {REG_GAM8,          0x86},
  {REG_GAM9,          0x8E},
  {REG_GAM10,         0x97},
  {REG_GAM11,         0xA4},
  {REG_GAM12,         0xAF},
  {REG_GAM13,         0xC5},
  {REG_GAM14,         0xD7},
  {REG_GAM15,         0xE8},
  
  {REG_SLOP,          0x20},
  {REG_EDGE1,         0x05},
  {REG_EDGE2,         0x03},
  {REG_EDGE3,         0x00},
  {REG_DNSOff,        0x01},
  
  {REG_MTX1,          0xB0},
  {REG_MTX2,          0x9D},
  {REG_MTX3,          0x13},
  {REG_MTX4,          0x16},
  {REG_MTX5,          0x7B},
  {REG_MTX6,          0x91},
  {REG_MTX_Ctrl,      0x1E},
  
  {REG_BRIGHT,        0x08},
  {REG_CNST,          0x20},
  {REG_UVADJ0,        0x81},
  {REG_SDE,           0x06},
  
  // For 30 fps/60Hz
  {REG_DM_LNL,        0x00},
  {REG_DM_LNH,        0x00},
  {REG_BDBase,        0x7F},
  {REG_BDMStep,       0x03},
  
  // Lens Correction, should be tuned with real camera module
  {REG_LC_CTR,        0x01}, // Enable LC and use 1 coefficient for all 3 channels
  {REG_LC_RADI,       0x30}, // The radius of the circle where no compensation applies
  {REG_LC_COEF,       0x30}, // RGB Lens correction coefficient
  
  // Frame reduction in night mode.
//  {REG_COM5,          0xD5},
  {0x00,              0x00},
  
  ///////////////////////////////////
//  {REG_CLKRC,           0x00}, /* clock config */
//  {REG_COM7,            0x46}, //RGB�����ʽ
//  {REG_HSTART,          0x3F},
//  {REG_HSIZE,           0x50},
//  {REG_VSTRT,           0x03},
//  {REG_VSIZE,           0x78},
//  {REG_HREF,            0x00},
//  {REG_SCAL0,           0x0A},
//  {REG_AWB_Ctrl0,       0xE0},
//  {REG_DSPAuto,         0xff},
//  {REG_DSP_Ctrl2,       0x0C},
//  {REG_DSP_Ctrl3,       0x00},
//  {REG_DSP_Ctrl4,       0x00},
//  {REG_SDE,		0x06},	//��ɫ	����SDE����Ĵ���������ʵ������Ч��
//  
//  {REG_HOutSize     , 0x14},
//  
//  {REG_VOutSize     , 0x1E},
//
//  {REG_COM4,            0x40},
//  {REG_COM8,            0xCC},
//  {REG_AECH,            0x00},
//  {REG_AEC,             0x05},
//  
//  {REG_CLKRC,           0x00}, /* clock config */
//  {REG_COM7,            0x46}, /* QVGA RGB565 */
//  {REG_HSTART,          0x3f}, /* ˮƽͼ��ʼ */
//  {REG_HSIZE,           0x50}, /* ˮƽͼ���� */
//  {REG_VSTRT,           0x03}, /* ��ֱͼ��ʼ */
//  {REG_VSIZE,           0x78}, /* ��ֱͼ��߶� */
//  {REG_HREF,            0x00}, /* ���� */
//  {REG_HOutSize,        0x14}, /* ˮƽ������ 320:0x50 80:0x14*/
//  {REG_VOutSize,        0x1E}, /* ��ֱ����߶� 240:0x78 60:0x1E*/
////  {REG_EXHCH,           0x00}, /* ���� */
//  
//  /*DSP control*/
//  {REG_TGT_B,           0x7f},
//  {REG_FixGain,         0x09},
//  {REG_AWB_Ctrl0,       0xe0},
//  {REG_DSP_Ctrl1,       0xff},
//  {REG_DSP_Ctrl2,       0x20},
//  {REG_DSP_Ctrl3,	0x00},
//  {REG_DSP_Ctrl4,       0x00},
//  
//  /*AGC AEC AWB*/
//  {REG_COM8,		0xf0},
//  {REG_COM4,		0xC1}, /*Pll AEC CONFIG*/ //81
//  {REG_COM6,		0xc5},
//  {REG_COM9,		0x21},
//  {REG_BDBase,	        0xFF},
//  {REG_BDMStep,	        0x01},
//  {REG_AEW,		0x34},
//  {REG_AEB,		0x3c},
//  {REG_VPT,		0xa1},
//  {REG_EXHCL,		0x00},
//  {REG_AWBCtrl3,        0xaa},
//  {REG_COM8,		0xff},
//  {REG_AWBCtrl1,        0x5d},
//  
//  {REG_EDGE1,		0x0a},
//  {REG_DNSOff,	        0x01},
//  {REG_EDGE2,		0x01},
//  {REG_EDGE3,		0x01},
//  
//  {REG_MTX1,		0x5f},
//  {REG_MTX2,		0x53},
//  {REG_MTX3,		0x11},
//  {REG_MTX4,		0x1a},
//  {REG_MTX5,		0x3d},
//  {REG_MTX6,		0x5a},
//  {REG_MTX_Ctrl,        0x1e},
//  
//  {REG_BRIGHT,	        0x00},
//  {REG_CNST,		0x25},
//  {REG_USAT,		0x65},
//  {REG_VSAT,		0x65},
//  {REG_UVADJ0,	        0x81},
////  {REG_SDE,		  0x20},	//�ڰ�
//  {REG_SDE,		0x06},	//��ɫ	����SDE����Ĵ���������ʵ������Ч��
//  
//  /*GAMMA config*/
//  {REG_GAM1,		0x0c},
//  {REG_GAM2,		0x16},
//  {REG_GAM3,		0x2a},
//  {REG_GAM4,		0x4e},
//  {REG_GAM5,		0x61},
//  {REG_GAM6,		0x6f},
//  {REG_GAM7,		0x7b},
//  {REG_GAM8,		0x86},
//  {REG_GAM9,		0x8e},
//  {REG_GAM10,		0x97},
//  {REG_GAM11,		0xa4},
//  {REG_GAM12,		0xaf},
//  {REG_GAM13,		0xc5},
//  {REG_GAM14,		0xd7},
//  {REG_GAM15,		0xe8},
//  {REG_SLOP,		0x20},
//  
//  {REG_HUECOS,	        0x80},
//  {REG_HUESIN,	        0x80},
//  {REG_DSPAuto, 	0xff},
//  {REG_DM_LNL,	        0x00},
//  {REG_BDBase,	        0x99},
//  {REG_BDMStep,	        0x03},
//  {REG_LC_RADI,	        0x00},
//  {REG_LC_COEF,	        0x13},
//  {REG_LC_XC,		0x08},
//  {REG_LC_COEFB,        0x14},
//  {REG_LC_COEFR,        0x17},
//  {REG_LC_CTR,	        0x05},
//  
  
  /*night mode auto frame rate control*/
  //{REG_COM5,		0xf5},	 /*��ҹ�ӻ����£��Զ�����֡�ʣ���֤���նȻ�������*/
  {REG_COM5,		0x31},	/*ҹ�ӻ���֡�ʲ���*/
};

uint8_t OV7725_REG_NUM = sizeof(Sensor_Config)/sizeof(Sensor_Config[0]);	  /*�ṹ�������Ա��Ŀ*/

volatile uint8_t Ov7725_vsync ;	 /* ֡ͬ���źű�־�����жϺ�����main��������ʹ�� */

/************************************************
* ��������FIFO_GPIO_Config
* ����  ��FIFO GPIO����
* ����  ����
* ���  ����
* ע��  ����
************************************************/
static void FIFO_GPIO_Config(void)
{
  FIFO_OE_L();	  					/*����ʹFIFO���ʹ��*/
  FIFO_WE_H();   					/*����ʹFIFOд����*/  
}


/************************************************
* ��������VSYNC_GPIO_Config
* ����  ��OV7725 VSYNC�ж��������
* ����  ����
* ���  ����
* ע��  ����
************************************************/
static void VSYNC_GPIO_Config(void)
{
}


/************************************************
* ��������OV7725_GPIO_Config
* ����  ����ʼ����������ͷ��ӵ�IO
* ����  ����
* ���  ����
* ע��  ����
************************************************/
void OV7725_GPIO_Config(void)
{
  SCCB_GPIO_Config();
  FIFO_GPIO_Config();
  VSYNC_GPIO_Config();
}

/************************************************
* ��������Sensor_Init
* ����  ��Sensor��ʼ��
* ����  ����
* ���  ������1�ɹ�������0ʧ��
* ע��  ����
************************************************/
uint8_t Sensor_IDCode = 0;	
ErrorStatus OV7725_Init(void)
{
  uint16_t i = 0;
  Sensor_IDCode = 0;	
  
  OV7725_GPIO_Config();
  //DEBUG("ov7725 Register Config Start......");
  
  if( 0 == SCCB_WriteByte ( REG_COM7, 0x80 ) ) /* ��λsensor */
  {
    //DEBUG("sccb write data error");		
    return ERROR ;
  }	
  
  if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, REG_VER ) )	 /* ��ȡsensor ID�� */
  {
    //DEBUG("read id faild");		
    return ERROR;
  }
  //DEBUG("Sensor ID is 0x%x", Sensor_IDCode);	
  
  if(Sensor_IDCode == OV7725_ID)
  {
    for( i = 0 ; i < OV7725_REG_NUM ; i++ )
    {
      if( 0 == SCCB_WriteByte(Sensor_Config[i].Address, Sensor_Config[i].Value) )
      {                
        //DEBUG("write reg faild", Sensor_Config[i].Address);
        return ERROR;
      }
    }
  }
  else
  {
    return ERROR;
  }
  //DEBUG("ov7725 Register Config Success");
  
  return SUCCESS;
}



/**
* @brief  ���ù���ģʽ
* @param  mode :����ģʽ��������Χ[0~5]
@arg 0:�Զ�
@arg 1:����
@arg 2:����
@arg 3:�칫��
@arg 4:����
@arg 5:ҹ��
* @retval ��
*/
void OV7725_Light_Mode(uint8_t mode)
{
  switch(mode)
  {
  case 0:	//Auto���Զ�ģʽ
    SCCB_WriteByte(0x13, 0xff); //AWB on 
    SCCB_WriteByte(0x0e, 0x65);
    SCCB_WriteByte(0x2d, 0x00);
    SCCB_WriteByte(0x2e, 0x00);
    break;
  case 1://sunny������
    SCCB_WriteByte(0x13, 0xfd); //AWB off
    SCCB_WriteByte(0x01, 0x5a);
    SCCB_WriteByte(0x02, 0x5c);
    SCCB_WriteByte(0x0e, 0x65);
    SCCB_WriteByte(0x2d, 0x00);
    SCCB_WriteByte(0x2e, 0x00);
    break;	
  case 2://cloudy������
    SCCB_WriteByte(0x13, 0xfd); //AWB off
    SCCB_WriteByte(0x01, 0x58);
    SCCB_WriteByte(0x02, 0x60);
    SCCB_WriteByte(0x0e, 0x65);
    SCCB_WriteByte(0x2d, 0x00);
    SCCB_WriteByte(0x2e, 0x00);
    break;	
  case 3://office���칫��
    SCCB_WriteByte(0x13, 0xfd); //AWB off
    SCCB_WriteByte(0x01, 0x84);
    SCCB_WriteByte(0x02, 0x4c);
    SCCB_WriteByte(0x0e, 0x65);
    SCCB_WriteByte(0x2d, 0x00);
    SCCB_WriteByte(0x2e, 0x00);
    break;	
  case 4://home������
    SCCB_WriteByte(0x13, 0xfd); //AWB off
    SCCB_WriteByte(0x01, 0x96);
    SCCB_WriteByte(0x02, 0x40);
    SCCB_WriteByte(0x0e, 0x65);
    SCCB_WriteByte(0x2d, 0x00);
    SCCB_WriteByte(0x2e, 0x00);
    break;	
    
  case 5://night��ҹ��
    SCCB_WriteByte(0x13, 0xff); //AWB on
    SCCB_WriteByte(0x0e, 0xe5);
    break;	
    
  default:
    OV7725_DEBUG("Light Mode parameter error!"); 
    
    break;
  }
  
}			


/**
* @brief  ���ñ��Ͷ�
* @param  sat:���Ͷ�,������Χ[-4 ~ +4]             	
* @retval ��
*/
void OV7725_Color_Saturation(int8_t sat)
{
  
  if(sat >=-4 && sat<=4)
  {	
    SCCB_WriteByte(REG_USAT, (sat+4)<<4); 
    SCCB_WriteByte(REG_VSAT, (sat+4)<<4);
  }
  else
  {
    OV7725_DEBUG("Color Saturation parameter error!");
  }
  
}			


/**
* @brief  ���ù��ն�
* @param  bri:���նȣ�������Χ[-4~+4]
* @retval ��
*/
void OV7725_Brightness(int8_t bri)
{
  uint8_t BRIGHT_Value,SIGN_Value;	
  
  switch(bri)
  {
  case 4:
    BRIGHT_Value = 0x48;
    SIGN_Value = 0x06;
    break;
    
  case 3:
    BRIGHT_Value = 0x38;
    SIGN_Value = 0x06;		
    break;	
    
  case 2:
    BRIGHT_Value = 0x28;
    SIGN_Value = 0x06;			
    break;	
    
  case 1:
    BRIGHT_Value = 0x18;
    SIGN_Value = 0x06;			
    break;	
    
  case 0:
    BRIGHT_Value = 0x08;
    SIGN_Value = 0x06;			
    break;	
    
  case -1:
    BRIGHT_Value = 0x08;
    SIGN_Value = 0x0e;		
    break;	
    
  case -2:
    BRIGHT_Value = 0x18;
    SIGN_Value = 0x0e;		
    break;	
    
  case -3:
    BRIGHT_Value = 0x28;
    SIGN_Value = 0x0e;		
    break;	
    
  case -4:
    BRIGHT_Value = 0x38;
    SIGN_Value = 0x0e;		
    break;	
    
  default:
    OV7725_DEBUG("Brightness parameter error!");
    break;
  }
  
  SCCB_WriteByte(REG_BRIGHT, BRIGHT_Value); //AWB on
  SCCB_WriteByte(REG_SIGN, SIGN_Value);
}		

/**
* @brief  ���öԱȶ�
* @param  cnst:�Աȶȣ�������Χ[-4~+4]
* @retval ��
*/
void OV7725_Contrast(int8_t cnst)
{
  if(cnst >= -4 && cnst <=4)
  {
    SCCB_WriteByte(REG_CNST, (0x30-(4-cnst)*4));
  }
  else
  {
    OV7725_DEBUG("Contrast parameter error!");
  }
}		


/**
* @brief  ��������Ч��
* @param  eff:����Ч����������Χ[0~6]:
@arg 0:����
@arg 1:�ڰ�
@arg 2:ƫ��
@arg 3:����
@arg 4:ƫ��
@arg 5:ƫ��
@arg 6:����
* @retval ��
*/
void OV7725_Special_Effect(uint8_t eff)
{
  switch(eff)
  {
  case 0://����
    SCCB_WriteByte(0xa6, 0x06);
    SCCB_WriteByte(0x60, 0x80);
    SCCB_WriteByte(0x61, 0x80);
    break;
    
  case 1://�ڰ�
    SCCB_WriteByte(0xa6, 0x26);
    SCCB_WriteByte(0x60, 0x80);
    SCCB_WriteByte(0x61, 0x80);
    break;	
    
  case 2://ƫ��
    SCCB_WriteByte(0xa6, 0x1e);
    SCCB_WriteByte(0x60, 0xa0);
    SCCB_WriteByte(0x61, 0x40);	
    break;	
    
  case 3://����
    SCCB_WriteByte(0xa6, 0x1e);
    SCCB_WriteByte(0x60, 0x40);
    SCCB_WriteByte(0x61, 0xa0);	
    break;	
    
  case 4://ƫ��
    SCCB_WriteByte(0xa6, 0x1e);
    SCCB_WriteByte(0x60, 0x80);
    SCCB_WriteByte(0x61, 0xc0);		
    break;	
    
  case 5://ƫ��
    SCCB_WriteByte(0xa6, 0x1e);
    SCCB_WriteByte(0x60, 0x60);
    SCCB_WriteByte(0x61, 0x60);		
    break;	
    
  case 6://����
    SCCB_WriteByte(0xa6, 0x46);
    break;	
    
  default:
    OV7725_DEBUG("Special Effect error!");
    break;
  }
}		



/**
* @brief  ����ͼ��������ڣ��ֱ��ʣ�QVGA
* @param  sx:����x��ʼλ��
* @param  sy:����y��ʼλ��
* @param  width:���ڿ��
* @param  height:���ڸ߶�
* @param QVGA_VGA:0,QVGAģʽ��1��VGAģʽ
*
* @note 	QVGAģʽ ����Ҫ��sx + width <= 320 ,sy+height <= 240
* 			 	VGAģʽ����Ҫ��sx + width <= 640 ,sy+height <= 480
*					������ Һ�����ֱ��� �� FIFO�ռ� �����ƣ������̲������ڳ���320*240������
*         ʹ��VGAģʽ��Ҫ����ΪOV7725�޷�ֱ�ӽ���XY����QVGA������ʹ����ƽ��ʾ��
*					���ó�VGAģʽ������ʹ��������ʾ��
*					���QVGAģʽ��ͬ���ֱ����� VGAģʽ ͼ�����֡������
* @retval ��
*/
void OV7725_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height,uint8_t QVGA_VGA)
{
  uint8_t reg_raw,cal_temp;
  
  /***********QVGA or VGA *************/
  if(QVGA_VGA == 0)
  {
    /*QVGA RGB565 */
    SCCB_WriteByte(REG_COM7,0x46); 
  }
  else
  {
    /*VGA RGB565 */
    SCCB_WriteByte(REG_COM7,0x06); 
  }
  
  /***************HSTART*********************/
  //��ȡ�Ĵ�����ԭ���ݣ�HStart����ƫ��ֵ����ԭʼƫ��ֲ�Ļ����ϼ��ϴ���ƫ��	
  SCCB_ReadByte(&reg_raw,1,REG_HSTART);
  
  //sxΪ����ƫ�ƣ���8λ�洢��HSTART����2λ��HREF
  cal_temp = (reg_raw + (sx>>2));	
  SCCB_WriteByte(REG_HSTART,cal_temp ); 
  
  /***************HSIZE*********************/
  //ˮƽ��ȣ���8λ�洢��HSIZE����2λ�洢��HREF
  SCCB_WriteByte(REG_HSIZE,width>>2);//HSIZE������λ 
  
  
  /***************VSTART*********************/
  //��ȡ�Ĵ�����ԭ���ݣ�VStart����ƫ��ֵ����ԭʼƫ��ֲ�Ļ����ϼ��ϴ���ƫ��	
  SCCB_ReadByte(&reg_raw,1,REG_VSTRT);	
  //syΪ����ƫ�ƣ���8λ�洢��HSTART����1λ��HREF
  cal_temp = (reg_raw + (sy>>1));	
  
  SCCB_WriteByte(REG_VSTRT,cal_temp);
  
  /***************VSIZE*********************/
  //��ֱ�߶ȣ���8λ�洢��VSIZE����1λ�洢��HREF
  SCCB_WriteByte(REG_VSIZE,height>>1);//VSIZE����һλ
  
  /***************VSTART*********************/
  //��ȡ�Ĵ�����ԭ����	
  SCCB_ReadByte(&reg_raw,1,REG_HREF);	
  //��ˮƽ��ȵĵ�2λ����ֱ�߶ȵĵ�1λ��ˮƽƫ�Ƶĵ�2λ����ֱƫ�Ƶĵ�1λ��������ӵ�HREF
  cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2)|((sx&0x03)<<4)|((sy&0x01)<<6));	
  
  SCCB_WriteByte(REG_HREF,cal_temp);
  
  /***************HOUTSIZIE /VOUTSIZE*********************/
  SCCB_WriteByte(REG_HOutSize,width>>2);
  SCCB_WriteByte(REG_VOutSize,height>>1);
  
  //��ȡ�Ĵ�����ԭ����	
  SCCB_ReadByte(&reg_raw,1,REG_EXHCH);	
  cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2));	
  
  SCCB_WriteByte(REG_EXHCH,cal_temp);	
}


/**
* @brief  ����ͼ��������ڣ��ֱ��ʣ�VGA
* @param  sx:����x��ʼλ��
* @param  sy:����y��ʼλ��
* @param  width:���ڿ��
* @param  height:���ڸ߶�
* @note 	����������Ҫ��sx + width <= 640 ,sy+height <= 480
*					������ Һ�����ֱ��� �� FIFO�ռ� �����ƣ������̲������ڳ���320*240������
*         ʹ�ñ�������Ҫ����ΪOV7725�޷�ֱ�ӽ���XY����QVGA������ʹ����ƽ��ʾ��
*					ʹ�ñ��������ó�VGAģʽ������ʹ��������ʾ��
*					���QVGAģʽ��ͬ���ֱ����� VGAģʽ ͼ�����֡������
* @retval ��
*/
void OV7725_Window_VGA_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
  
  uint8_t reg_raw,cal_temp;
  
  /***********QVGA or VGA *************/
  /*VGA RGB565 */
  SCCB_WriteByte(REG_COM7,0x06); 
  
  /***************HSTART*********************/
  //��ȡ�Ĵ�����ԭ���ݣ�HStart����ƫ��ֵ����ԭʼƫ��ֲ�Ļ����ϼ��ϴ���ƫ��	
  SCCB_ReadByte(&reg_raw,1,REG_HSTART);
  
  //sxΪ����ƫ�ƣ���8λ�洢��HSTART����2λ��HREF
  cal_temp = (reg_raw + (sx>>2));	
  SCCB_WriteByte(REG_HSTART,cal_temp ); 
  
  /***************HSIZE*********************/
  //ˮƽ��ȣ���8λ�洢��HSIZE����2λ�洢��HREF
  SCCB_WriteByte(REG_HSIZE,width>>2);//HSIZE������λ 320 
  
  
  /***************VSTART*********************/
  //��ȡ�Ĵ�����ԭ���ݣ�VStart����ƫ��ֵ����ԭʼƫ��ֲ�Ļ����ϼ��ϴ���ƫ��	
  SCCB_ReadByte(&reg_raw,1,REG_VSTRT);	
  //syΪ����ƫ�ƣ���8λ�洢��HSTART����1λ��HREF
  cal_temp = (reg_raw + (sy>>1));	
  
  SCCB_WriteByte(REG_VSTRT,cal_temp);
  
  /***************VSIZE*********************/
  //��ֱ�߶ȣ���8λ�洢��VSIZE����1λ�洢��HREF
  SCCB_WriteByte(REG_VSIZE,height>>1);//VSIZE����һλ 240
  
  /***************VSTART*********************/
  //��ȡ�Ĵ�����ԭ����	
  SCCB_ReadByte(&reg_raw,1,REG_HREF);	
  //��ˮƽ��ȵĵ�2λ����ֱ�߶ȵĵ�1λ��ˮƽƫ�Ƶĵ�2λ����ֱƫ�Ƶĵ�1λ��������ӵ�HREF
  cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2)|((sx&0x03)<<4)|((sy&0x01)<<6));	
  
  SCCB_WriteByte(REG_VSTRT,cal_temp);
  
  /***************HOUTSIZIE /VOUTSIZE*********************/
  SCCB_WriteByte(REG_HOutSize,width>>2);
  SCCB_WriteByte(REG_VOutSize,height>>1);
  
  //��ȡ�Ĵ�����ԭ����	
  SCCB_ReadByte(&reg_raw,1,REG_EXHCH);	
  
  cal_temp = (reg_raw |(width&0x03)|((height&0x01)<<2));	
  
  SCCB_WriteByte(REG_EXHCH,cal_temp);	
}

/**
* @brief  ������ʾλ��
* @param  sx:x��ʼ��ʾλ��
* @param  sy:y��ʼ��ʾλ��
* @param  width:��ʾ���ڿ��,Ҫ���OV7725_Window_Set�����е�widthһ��
* @param  height:��ʾ���ڸ߶ȣ�Ҫ���OV7725_Window_Set�����е�heightһ��
* @retval ��
*/
void ImagDisp(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
  //  uint16_t i, j; 
  //  uint16_t Camera_Data;
  //  
  //  ILI9341_OpenWindow(sx,sy,width,height);
  //  ILI9341_Write_Cmd ( CMD_SetPixel );	
  //  
  //  for(i = 0; i < width; i++)
  //  {
  //    for(j = 0; j < height; j++)
  //    {
  //      READ_FIFO_PIXEL(Camera_Data);		/* ��FIFO����һ��rgb565���ص�Camera_Data���� */
  //      ILI9341_Write_Data(Camera_Data);
  //    }
  //  }
}

/*
ov7725 refresh black
*/
void camera_black(void)
{
  Site_t site = {24,34};   //x = 10 ,y = 20
    Size_t size = {80,60};  // W = 80 ,H = 60
    uint16_t i , j;
        if(ov7725_vsync == 2)
        {
          FIFO_PREPARE;
//          LCD_clear(WHITE);     //����
          LCD_PTLON(site, size);                      //����
          LCD_RAMWR();                                //д�ڴ�

          image_feature.start_row = 60;
          image_feature.end_row = 0;
          for(i = 0; i < 60; i++)
          {
            image_feature.row[i].start_point = 80;
            image_feature.row[i].end_point = 0;
            image_feature.row[i].white_points = 0;//���㵱ǰ�а׵���
            for(j = 0; j < 80; j++)
            {
              //�ȸߺ��
              READ_FIFO_PIXEL(Camera_Data);		/* ��FIFO����һ��rgb565���ص�Camera_Data���� */
              LCD_WR_DATA(Camera_Data);
              RED_value = (Camera_Data & 0xF800) >> 8;//Rȡ��5λ
              GREEN_value = (Camera_Data & 0x07E0) >> 3;//Gȡ��6λ
              BLUE_value = (Camera_Data & 0x001F) << 3;//Bȡ��5λ
              if(RED_value <50 && GREEN_value <60 && BLUE_value <50)    //ʶ���ɫ
              {
                imgBuf[i][j/8] |= (0x01<<(7-j%8));//����Ŀ����ɫ����
                if(image_feature.row[i].start_point == 80) image_feature.row[i].start_point = j; 
                image_feature.row[i].end_point = j;
                ++image_feature.row[i].white_points;
              }
              else
              {
                imgBuf[i][j/8] &= (0xFE<<(7-j%8));
              }
            }
            if(image_feature.row[i].white_points != 0)
            {
              if(image_feature.start_row == 60) image_feature.start_row = i;        
              image_feature.end_row = i;
            }
          }
          color_tracking();

          uint16_t time =  TMR0.TCNT;
          time <<= 8;
          time |= TMR1.TCNT;
          Camera_FPS = 39063/time;
          TMR0.TCNT = (TMR1.TCNT = 0);//TMR1�Ǹ߰�λ
//          R_ICU_IRQ5_Start();
          ov7725_vsync = 0;
        };
}

/*
ov7725 refresh red
*/
void camera_red(void)
{
  Site_t site = {24,34};   //x = 10 ,y = 20
    Size_t size = {80,60};  // W = 80 ,H = 60
    uint16_t i , j;
        if(ov7725_vsync == 2)
        {
          FIFO_PREPARE;
//          LCD_clear(WHITE);     //����
          LCD_PTLON(site, size);                      //����
          LCD_RAMWR();                                //д�ڴ�

          image_feature.start_row = 60;
          image_feature.end_row = 0;
          for(i = 0; i < 60; i++)
          {
            image_feature.row[i].start_point = 80;
            image_feature.row[i].end_point = 0;
            image_feature.row[i].white_points = 0;//���㵱ǰ�а׵���
            for(j = 0; j < 80; j++)
            {
              //�ȸߺ��
              READ_FIFO_PIXEL(Camera_Data);		/* ��FIFO����һ��rgb565���ص�Camera_Data���� */
              LCD_WR_DATA(Camera_Data);
              RED_value = (Camera_Data & 0xF800) >> 8;//Rȡ��5λ
              GREEN_value = (Camera_Data & 0x07E0) >> 3;//Gȡ��6λ
              BLUE_value = (Camera_Data & 0x001F) << 3;//Bȡ��5λ
              if(RED_value >180 && GREEN_value <70 && BLUE_value <70)     //ʶ���ɫ
              {
                imgBuf[i][j/8] |= (0x01<<(7-j%8));//����Ŀ����ɫ����
                if(image_feature.row[i].start_point == 80) image_feature.row[i].start_point = j; 
                image_feature.row[i].end_point = j;
                ++image_feature.row[i].white_points;
              }
              else
              {
                imgBuf[i][j/8] &= (0xFE<<(7-j%8));
              }
            }
            if(image_feature.row[i].white_points != 0)
            {
              if(image_feature.start_row == 60) image_feature.start_row = i;        
              image_feature.end_row = i;
            }
          }
          color_tracking();
          uint16_t time =  TMR0.TCNT;
          time <<= 8;
          time |= TMR1.TCNT;
          Camera_FPS = 39063/time;
          TMR0.TCNT = (TMR1.TCNT = 0);//TMR1�Ǹ߰�λ
//          R_ICU_IRQ5_Start();
          ov7725_vsync = 0;
        };
}

void color_tracking()
{
  uint16_t i = 0, j = 0;
  uint8_t point_min = 80;
  uint8_t point_max = 0;
  char str[2];
  Site_t cross_site = {0,0};

  if(image_feature.start_row != 60)//����Ŀ����ɫ���ص�
  {
    cross_site.y = (image_feature.start_row+image_feature.end_row)/2;
    for(i = image_feature.start_row; i < image_feature.end_row; i++)
    {
      if(image_feature.row[i].start_point < point_min)
        point_min = image_feature.row[i].start_point;
      if(image_feature.row[i].end_point > point_max)
        point_max = image_feature.row[i].end_point;
    }
    cross_site.x = (point_min+point_max)/2;
  }
  sprintf(str, "%2d", Camera_FPS);
  //LCD_str((Site_t){60,10}, (uint8_t *)str, WHITE, RED);
  sprintf(str, "%2d", (uint8_t)cross_site.x);
  //LCD_str((Site_t){35,100}, (uint8_t *)str, WHITE, RED);
  sprintf(str, "%2d", (uint8_t)cross_site.y);

  //LCD_str((Site_t){75,100}, (uint8_t *)str, WHITE, RED);
  LCD_rectangle((Site_t){24,34+cross_site.y}, (Size_t){80,1}, BLUE);
  LCD_rectangle((Site_t){24+cross_site.x,34}, (Size_t){1,60}, BLUE);
  error[0]=64-24-cross_site.x;
  error[1]=64-34-cross_site.y;
  error1=cross_site.x+100;
  error2=cross_site.y;
  R_SCI1_Serial_Send(&error1,1);
  R_SCI1_Serial_Send(&error2,1);
  //LCD_point((Site_t){24+cross_site.x,34+cross_site.y},BLACK);
  //LCD_point((Site_t){64,64},WHITE);
//LCD_rectangle((Site_t){20+cross_site.x,30}, (Size_t){1,60}, GREEN);
//  for(i = 0; i < 60; i++)
//  {
//    for(j = 0; j < 80; j++)
//    {
//      if(imgBuf[i][j/8] & (0x01<<(7-j%8)))
//        LCD_WR_DATA(WHITE);
//      else
//        LCD_WR_DATA(BLACK);
//    }
//  }

//  LCD_cross(cross_site, 40, GREEN);
}

/****************************End OF File*************************************/
