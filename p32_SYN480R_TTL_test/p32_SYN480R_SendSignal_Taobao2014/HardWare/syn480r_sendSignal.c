#include "syn480r_sendSignal.h"
#include "sys.h"
#include "delay.h"//ʹ������ԭ�ӵ�΢����ʱ����

//�ȳ�ʼ����������
void syn480r_GPIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

//�������data[]��Ҫ���͵��ֽ�����
//�������nBytesToSend������Ҫ���Ͷ��ٸ��ֽ�
//���͸�ʽ����ʼ�ź�+Ҫ�����ֽ���N+���͵�N�ֽ�
//ÿ���ֽڶ����� �ֽ�+�ֽڷ���
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
	
	//�ȷ��ͱ���Ҫ������ֽ���
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
	
		
	//��������
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

