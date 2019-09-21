#include "smc1602A.h"


//这个LCD1602速度较慢
//执行任何指令时，一些等待时间必不可少
void Delay(int ms)
{
	int i;
	for(;ms>0;ms--)
		for(i=0;i<10000;i++);
}


//第一次设置GPIOC和GPIOG 为输出模式，用于写指令或数据		    
//LED IO初始化
void GPIO_Init_1602(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PC,PG端口时钟
		
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD; 		 //开漏输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 //IO口速度为10MHz
	 GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化GPIOG
					
	 GPIO_InitStructure.GPIO_Pin=(uint16_t)0x00ff;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	 					
}



void Set_GPIOC_Input(void)
{ 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 GPIO_InitStructure.GPIO_Pin=(uint16_t)0x00ff;				 
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //浮空输入模式
					 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 						
}

void Set_GPIOC_Output(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 GPIO_InitStructure.GPIO_Pin=(uint16_t)0x00ff;				 
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 //IO口速度为10MHz 
					 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz						
}





 //写指令
void writeCmd(uint8_t cmd)
{
	uint16_t tmp;
	RS=0;
	RW=0;
	EN=0;
	tmp=GPIO_ReadOutputData(GPIOC);
	tmp=(tmp & 0xff00) | cmd;
	Set_GPIOC_Output();
	GPIO_Write(GPIOC,tmp);
	Delay(2);
	EN=1;
	Delay(5);
	EN=0;
	Delay(5);
}

//往RAM的特定位置写一个字节数据
//这个位置由上一个指令设置
void writeData(uint8_t data)
{
	uint16_t tmp;
	RS=1;
	RW=0;
	EN=0;
	tmp=GPIO_ReadOutputData(GPIOC);
	tmp=(tmp & 0xff00) | data;
	Set_GPIOC_Output();
	GPIO_Write(GPIOC,tmp);
	Delay(2);
	EN=1;
	Delay(5);
	EN=0;
	Delay(5);
}

//从RAM的特定位置读一个字节数据
//这个位置由上一个指令设置
uint8_t readByte(void)
{
	uint16_t tmp;
	RS=1;
	RW=1;
	Delay(2);
	EN=1;
	//tmp=(tmp & 0xff00) | data;
	Set_GPIOC_Input();
	Delay(3);
	tmp=GPIO_ReadInputData(GPIOC);	
	EN=0;
	Delay(5);
	return tmp&0xff;
}


void initial_1602(void)
{ 
	writeCmd(0x01);
	writeCmd(0x38);
	writeCmd(0x06);
	writeCmd(0x0c);	 
}

void showOn_1602(RowNum p,uint8_t *s)
{ 
	uint8_t  i;
	
	//设置写RAM的起始地址
	if(p==ROW_1)
		writeCmd(0x80);
	else
		writeCmd(0xc0);//0x80 + 0xC0

	for(i=0;i<16;i++)
	{	
		writeData(s[i]);//已设置为指针自动增加
	}	
}

/**
*   在RAM中第p行第column列的位置写一个字节
*/
void writeByteToRAM_1602(RowNum p,uint8_t columNum,uint8_t data)
{ 
	columNum=columNum%40; //RAM地址：每行40个字节
	
	if(p==ROW_1)
		writeCmd(0x80+columNum);
	else
		writeCmd(0xc0+columNum);
	
	writeData(data);		
}

/**
*   从RAM中指定行、列号处读一个字节
*/
uint8_t readByteFromRAM_1602(RowNum p,uint8_t columNum)
{ 
	columNum=columNum%40; //RAM地址：每行40个字节
	
	if(p==ROW_1)
		writeCmd(0x80+columNum);
	else
		writeCmd(0xc0+columNum);
	
	return readByte();		
}

  












