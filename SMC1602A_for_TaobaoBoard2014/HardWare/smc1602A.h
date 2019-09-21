 #ifndef __SMC1602A_H
#define __SMC1602A_H	 
#include "sys.h"


//不能把1602插到12864运行的程序上
//1602背光的2个引脚会与12864的IO口冲突

#define RS PDout(8)// RRST
#define RW PDout(9)// SDA
#define EN PDout(10)// OE

typedef enum
{ 
  ROW_1= 1,
  ROW_2
}RowNum;

void GPIO_Init_1602(void);//初始化
void Set_GPIOD_Input(void);
void Set_GPIOD_Output(void);
void writeCmd(uint8_t cmd);
void writeData(uint8_t data);
uint8_t readByte(void);
void initial_1602(void);
void showOn_1602(RowNum rowNum,uint8_t *s);
void writeByteToRAM_1602(RowNum p,uint8_t columNum,uint8_t data);
uint8_t readByteFromRAM_1602(RowNum p,uint8_t columNum);
		 				    
#endif
