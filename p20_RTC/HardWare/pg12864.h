#ifndef __PG12864_H
#define __PG12864_H 
#include "sys.h"



#define RS  PGout(14)// RRST
#define RD  PGout(13)// SDA
#define RW  PGout(15)// OE
#define RST PDout(6)// WRST
#define CS  PDout(3)// SCL

struct Enchar               
{   
	uint8_t   Index;          	
	uint8_t   Msk[16]; 
};

struct Cnchar               
{   
	uint8_t   Index[2];          	
	uint8_t   Msk[32]; 
};

void GPIO_Init_12864(void);//≥ı ºªØ
void Init_12864(void);
void WriteCmdByteTo_12864(uint8_t cmd);
void WriteDataByteTo_12864(uint8_t  data);
void ClearScreen(void);
void WriteEn(u8 kn,uint8_t x,uint8_t y,uint8_t *cn);
void WriteCn(u8 kn,uint8_t x,uint8_t y,uint8_t *cn);



		 				    
#endif
