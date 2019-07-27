#ifndef __SCCB_H
#define __SCCB_H

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define SCL_H         PORT2.PODR.BIT.B2 = 1U
#define SCL_L         PORT2.PODR.BIT.B2 = 0U

#define SDA_H         PORT2.PODR.BIT.B3 = 1U
#define SDA_L         PORT2.PODR.BIT.B3 = 0U

#define SCL_read      PORT2.PIDR.BIT.B2
#define SDA_read      PORT2.PIDR.BIT.B3

#define ADDR_OV7725   0x42

void SCCB_GPIO_Config(void);
int SCCB_WriteByte(unsigned short WriteAddress , unsigned char SendByte);
int SCCB_ReadByte(unsigned char * pBuffer,   unsigned short length,   unsigned char ReadAddress);

#endif 
