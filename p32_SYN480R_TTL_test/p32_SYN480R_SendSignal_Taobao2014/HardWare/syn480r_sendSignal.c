#include "syn480r_sendSignal.h"
#include "sys.h"
#include "delay.h"//使用正点原子的微秒延时函数

//先初始化发送引脚
void syn480r_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

//输入参数data[]：要发送的字节数组
//输入参数nBytesToSend：本次要发送多少个字节
//发送格式：起始信号+要发送字节数N+发送的N字节
//每个字节都发送 字节+字节反码
void syn480r_SendBytes(u8 data[],u8 nBytesToSend)
{
	u8 i,j;
	u8 tmp;
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(50);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(50);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(1000);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(500);
	
	//先发送本次要传输的字节数
	tmp=nBytesToSend;
	for(j=0;j<8;j++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		delay_us(50);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		delay_us(50);
		if(tmp&(0x1<<j))
			delay_us(50);
	}
	tmp=~nBytesToSend;
	for(j=0;j<8;j++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		delay_us(50);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		delay_us(50);
		if(tmp&(0x1<<j))
			delay_us(50);
	}
	
		
	//发送数据
	for(i=0;i<nBytesToSend;i++)
	{
		tmp=data[i];
		for(j=0;j<8;j++)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			delay_us(50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			delay_us(50);
			if(tmp&(0x1<<j))
				delay_us(50);
		}
		tmp=~data[i];
		for(j=0;j<8;j++)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			delay_us(50);
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			delay_us(50);
			if(tmp&(0x1<<j))
				delay_us(50);
		}
	}

	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(50);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(50);
}

