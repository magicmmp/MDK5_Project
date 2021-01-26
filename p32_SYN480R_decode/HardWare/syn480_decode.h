#ifndef __SYN480_DECODE_H
#define __SYN480_DECODE_H	 
#include "sys.h"


#define PB10  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)//信号接收引脚PB10

// 按键信息结构体声明
typedef struct
{   
	uint16_t   nPulse;       //收到的第几个脉冲
	uint16_t   nPulseToWait; //总共会收到几个脉冲
	uint8_t    valid;        //收到正确的数据后变有效。访问后应手动设为无效
	uint8_t    isPressing;   //正在接收数据，收到同步头置1，信号停止120mS后为0，认为本次接收结束
	u8 dataBuffer[256];//存放收到的字节数据
	u8 nBytesReceived; //收到了多少个字节
	u8 byteTmp;  //临时存放收到的字节数据
	u8 byteTmp0; //临时存放收到的字节数据
	u8 ErrNo;    //错误码,0初始状态，1正确，2传输错误，3计数器溢出
}syn480DataInfoTypeDef;



extern syn480DataInfoTypeDef syn480DataInfo ;

extern u16 nSysTick;
extern u16 nTIM2_CCR;
void   syn480_Init(void); //初始化GPIO、定时器等外设，然后就可以接收数据了。
u8     syn480_getErrNo();//查看是否产生错误
u8 		 syn480_isValid();//查看是否收到一条数据
u8     syn480_GetData(u8 dataBuffer[]);//接收数据
//uint8_t   Remote_Scan(void);       //遥控器键盘扫描
void 	  Remote_Debug(void);//串口调试，查看每个脉冲的时间长度。要先初始化串口外设


#endif
