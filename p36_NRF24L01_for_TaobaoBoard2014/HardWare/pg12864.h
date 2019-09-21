#ifndef __PG12864_H
#define __PG12864_H 
#include "sys.h"



#define RS  PDout(8)// RRST
#define RD  PDout(9)// SDA
#define RW  PDout(10)// OE
#define RST PDout(12)// WRST
#define CS  PDout(11)// SCL

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
void WriteEn(uint8_t x,uint8_t y,uint8_t *cn);
void WriteCn(uint8_t x,uint8_t y,uint8_t *cn);



		 				    
#endif
