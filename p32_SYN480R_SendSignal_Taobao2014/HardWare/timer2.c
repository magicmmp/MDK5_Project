#include "timer2.h"
#include "sys.h"
#include "delay.h"
//����Ԫ������
//��ʼ�����־λ��NEC������ʼλ���ߵ�ƽλ���͵�ƽλ��δ����
/**
const uint16_t NEC_ARR[5]={1350,600,112,112,0};
const uint16_t NEC_CCR[5]={450,450,56,56,0};
*/
const uint16_t NEC_ARR[5]={1000,1350,225,112,112};
const uint16_t NEC_CCR[5]={200,900,56,56,56};
const uint8_t NEC_DATA[5]={0x78,0x56,0x34,0x12,0};


const uint16_t CCRV[5]={180,150,100,80,50};
uint16_t CCRV_index=0;


void PB10_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
}

void SimpleSignal()
{
	u8 i,j;
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(5000);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(20000);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(10000);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(5000);
//�����źž���ԽԶ���ź�Խ��
//���øߵ�ƽʱ����С���͵�ƽʱ����
//�ͻ���ִ������Ըߺ͵͵�ƽ���������ʱ�䶼����̫�̡�
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			delay_us(500);
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			delay_us(500);
			if(NEC_DATA[i]&(0x1<<j))
				delay_us(500);
		}
	}
	
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(500);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(500);
}

void syn480r_SendBytes(u8 data[],u8 nBytesToSend)
{
	u8 i,j;
	u8 tmp;
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(500);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(500);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(10000);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(5000);
	
	//�ȷ��ͱ���Ҫ������ֽ���
	tmp=nBytesToSend;
	for(j=0;j<8;j++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		delay_us(500);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		delay_us(500);
		if(tmp&(0x1<<j))
			delay_us(500);
	}
	tmp=~nBytesToSend;
	for(j=0;j<8;j++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		delay_us(500);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		delay_us(500);
		if(tmp&(0x1<<j))
			delay_us(500);
	}
	
		
	//��������
	for(i=0;i<nBytesToSend;i++)
	{
		tmp=data[i];
		for(j=0;j<8;j++)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			delay_us(500);
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			delay_us(500);
			if(tmp&(0x1<<j))
				delay_us(500);
		}
		tmp=~data[i];
		for(j=0;j<8;j++)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
			delay_us(500);
			GPIO_ResetBits(GPIOB, GPIO_Pin_10);
			delay_us(500);
			if(tmp&(0x1<<j))
				delay_us(500);
		}
	}

	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	delay_us(500);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	delay_us(500);
}


void TIM2_PWM_Init()
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); //Timer2������ӳ��  TIM2_CH3->PB10    
 
   //���ø�����Ϊ�����������,���TIM2 CH3��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = NEC_ARR[0]; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =719; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = NEC_CCR[0];
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	// ���ͨ�����е�ƽ��������
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3
	
	TIM2->CCMR2 &=0xff8f;
	TIM2->CCMR2 |=0x0050;
	//TIM2->CCMR2 |=0x0008;//OC3PE������Ƚ�3Ԥװ��ʹ��

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	

}

uint16_t test_time2=0;
u8 bitValue;
//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		if(bitsToSend>0)
			bitsToSend--;
		if(bitsToSend>1 && bitsToSend<34)
		{
			bitValue=33-bitsToSend;
			bitValue=NEC_DATA[bitValue/8]&(0x1<<(bitValue&0x7));
			if(bitValue)//����λ1
			{
				TIM_SetCompare3(TIM2,NEC_CCR[2]);
				TIM_SetAutoreload(TIM2,NEC_ARR[2]);
			}
			else//����λ0
			{
				TIM_SetCompare3(TIM2,NEC_CCR[3]);
				TIM_SetAutoreload(TIM2,NEC_ARR[3]);
			}
			
		}
		else if(bitsToSend==33)
		{
			TIM_SetCompare3(TIM2,NEC_CCR[1]);
			TIM_SetAutoreload(TIM2,NEC_ARR[1]);
		}
		else if(bitsToSend==1)
		{
			TIM_SetCompare3(TIM2,NEC_CCR[4]);
			TIM_SetAutoreload(TIM2,NEC_ARR[4]);
		}
		else
		{
			TIM_Cmd(TIM2, DISABLE);
			TIM_SetCompare3(TIM2,NEC_CCR[1]);
			TIM_SetAutoreload(TIM2,NEC_ARR[1]);
			TIM2->CCMR2 &=0xff8f;
			TIM2->CCMR2 |=0x0050;//ǿ�ƶ�ʱ������ʱPWM����ߵ�ƽ
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		test_time2++;
	}
}



u8 bitsToSend;
void NECSendByte()
{
	bitsToSend=34;
	TIM2->CCMR2 &=0xff8f;
	TIM2->CCMR2 |=0x0070;
	TIM_SetCompare3(TIM2,NEC_CCR[1]);
	TIM_SetAutoreload(TIM2,NEC_ARR[1]);
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
	
}

