#include "syn480_decode.h"
#include "stm32f10x.h"



u16 nSysTick=0;//��¼����SysTick�жϵĴ���
u16 nTIM2_CCR=0;//��¼����TIM2�жϵĴ���


// ������Ϣ�ṹ�嶨��
syn480DataInfoTypeDef syn480DataInfo  = {0,0,0,0,0};

void syn480_Init()
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); //Timer2������ӳ��  TIM2_CH3->PB10
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	    
 
   //���ø�����Ϊ�����������,���TIM2 CH3��PWM���岨��	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM_CH3
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;  //�����������GPIO_Mode_IPD
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = 12000; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =719; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	//TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	//��ʼ��TIM2 Channel3 ���벶��ģʽ	 
	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	//TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	//TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	
	// ������ºͲ����жϱ�־λ
  TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC3);	
  // �������ºͲ����ж�  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC3, ENABLE );
	
	/**
	// ������ºͲ����жϱ�־λ
  TIM_ClearFlag(TIM4, TIM_FLAG_Update|TIM_IT_CC4);	
  // �������ºͲ����ж�  
	TIM_ITConfig (TIM4, TIM_IT_Update | TIM_IT_CC4, ENABLE );
*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	//NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	
	// ʹ�ܼ�����
	TIM_Cmd(TIM2, ENABLE);
	//TIM_Cmd(TIM4, ENABLE);
	syn480DataInfo.nBytesReceived=0;
}


//static���εı���ֻ�ܱ��ļ���ʹ��
static uint8_t  Polarity_index=0;  //����������ã�������0���½���1	
const  uint16_t PolaritySelect[2]={TIM_ICPolarity_Rising,TIM_ICPolarity_Falling};
static uint16_t HighTime[36];//���ڼ�¼����ĸߵ�ƽ��ʱ������λΪ10uS
static uint16_t LowTime[36]; //���ڼ�¼����ĵ͵�ƽ��ʱ������λΪ10uS
static uint16_t tmpHigh;//��Ÿߵ�ƽʱ�����ʱ����
static uint16_t tmpLow; //��ŵ͵�ƽʱ�����ʱ����


//�����źž���ԽԶ���ź�Խ��
//���øߵ�ƽʱ����С���͵�ƽʱ����
//�ͻ���ִ������Ըߺ͵͵�ƽ���������ʱ�䶼����̫�̡�
//��ʱ��2�жϷ������	 
void TIM2_IRQHandler(void)
//void TIM4_IRQHandler(void)
{
	// ���ز����ж�
	//if ( TIM_GetITStatus (TIM4, TIM_IT_CC4 ) != RESET)
	if ( TIM_GetITStatus (TIM2, TIM_IT_CC3 ) != RESET)
	{
		
		nTIM2_CCR++;//��¼�����жϵĴ���
		// ��������0
		TIM_SetCounter ( TIM2, 0 );
		//TIM_SetCounter ( TIM4, 0 );
		
		
		if(Polarity_index==0) //������ڲ������������
		{
			Polarity_index=1;
			//tmpLow=TIM_GetCapture4 (TIM4);//�Ƚ��͵�ƽʱ�䱣���
			tmpLow=TIM_GetCapture3 (TIM2);
			
			if(tmpLow>400 && tmpLow<600)
			if(tmpHigh>900 && tmpHigh<1100) //�յ� ͬ����ͷ
			{
				syn480DataInfo.isPressing=1;
				syn480DataInfo.nBytesReceived=0;
				syn480DataInfo.byteTmp=0;
				syn480DataInfo.nPulse=0;
				syn480DataInfo.nPulseToWait=16;
				syn480DataInfo.ErrNo=0;
			}
			if(syn480DataInfo.isPressing)//������ڰ��°���
			{
				if(syn480DataInfo.nPulse<sizeof(HighTime)/2)
				{
					HighTime[syn480DataInfo.nPulse]=tmpHigh;
					LowTime[syn480DataInfo.nPulse]=tmpLow;
				}
				
			
				//ͬ����֮���32λ��ʾ ��������
				if(syn480DataInfo.nPulse>0)
				{
					if(tmpLow>80)
						syn480DataInfo.byteTmp|=0x80;
						
					if(syn480DataInfo.nPulse&0x7)
					{
						syn480DataInfo.byteTmp>>=1;
					}
					else//ÿ�յ�һ���ֽ�
					{
						if(syn480DataInfo.nPulse&0xf)//��һ���ֽ�
						{
							syn480DataInfo.byteTmp0=syn480DataInfo.byteTmp;
							//syn480DataInfo.dataBuffer[(syn480DataInfo.nPulse>>3)-2]=syn480DataInfo.byteTmp;
						}
						else//�յ��ֽ�+�ֽڷ���
						{
								
							if(syn480DataInfo.byteTmp0+syn480DataInfo.byteTmp==0xff)
							{
								
								if(syn480DataInfo.nPulse==16)
								{
									if(syn480DataInfo.byteTmp0)//������һЩ�ֽ�Ҫ����
									{
										syn480DataInfo.nBytesReceived=syn480DataInfo.byteTmp0;
										syn480DataInfo.nPulseToWait=syn480DataInfo.byteTmp0;
										syn480DataInfo.nPulseToWait<<=4;
										syn480DataInfo.nPulseToWait+=16;
									}
									else
									{
										syn480DataInfo.valid=1;
										syn480DataInfo.isPressing=0;//ǿ��ֹͣ����
										syn480DataInfo.ErrNo=1;//������ȷ
									}
								}
								else
								{
									syn480DataInfo.dataBuffer[(syn480DataInfo.nPulse>>4)-2]=syn480DataInfo.byteTmp0;
									if(syn480DataInfo.nPulse==syn480DataInfo.nPulseToWait)
									{
										syn480DataInfo.valid=1;
										syn480DataInfo.ErrNo=1;//������ȷ
										syn480DataInfo.isPressing=0;//ǿ��ֹͣ����
									}	
								}		
								
							}
							else
							{
									syn480DataInfo.ErrNo=2;//���ݳ����������и���
									syn480DataInfo.isPressing=0;//ǿ��ֹͣ����
							}
							
						}
						syn480DataInfo.byteTmp=0;
					}
					
						
				}
				
				
					
				syn480DataInfo.nPulse++; //��������1
			}

		}
		else
		{
			Polarity_index=0;
			tmpHigh=TIM_GetCapture3 (TIM2);
			//tmpHigh=TIM_GetCapture4 (TIM4);//�Ƚ��ߵ�ƽʱ�䱣���
						
		}
			
		TIM_ClearITPendingBit (TIM2,TIM_IT_CC3);	
		// ���Ĳ����������
		TIM_OC3PolarityConfig(TIM2,PolaritySelect[Polarity_index] );	
		
		/**
		TIM_ClearITPendingBit (TIM4,TIM_IT_CC4);	
		// ���Ĳ����������
		TIM_OC4PolarityConfig(TIM4,PolaritySelect[Polarity_index] );		
		*/
	}
/**
	//�����ɿ�120mS����ʱ��TIM4�������
	if ( TIM_GetITStatus ( TIM4, TIM_IT_Update) != RESET )               
	{	
		if(syn480DataInfo.isPressing)
		{
			syn480DataInfo.isPressing=0;//�����ɿ���
		}
		TIM_ClearITPendingBit ( TIM4, TIM_FLAG_Update ); 	
	}
	*/
	if( TIM_GetITStatus ( TIM2, TIM_IT_Update) != RESET )               
	{	
		if(syn480DataInfo.isPressing)
		{
			syn480DataInfo.ErrNo=3;//���ݳ����������ź��ж�
			syn480DataInfo.isPressing=0;//ǿ��ֹͣ����
		}
		TIM_ClearITPendingBit ( TIM2, TIM_FLAG_Update ); 	
	}
	
	
	
	
}

//��ѯ�Ƿ������
//�����ܲ�����Ƹ��ŵĴ���
u8 syn480_getErrNo()
{
	u8 tmp=syn480DataInfo.ErrNo;
	syn480DataInfo.ErrNo=0;
	return tmp;
}

u8 syn480_isValid()
{
	return syn480DataInfo.valid;
}
//������� dataBuffer[] �����ڽ�������
//����ֵ���յ��˶��ٸ��ֽڣ�0��ʾ��û�յ��κ��ֽ�
u8 syn480_GetData(u8 dataBuffer[])
{
	u8 i;
	if(syn480DataInfo.valid)
	{
		syn480DataInfo.valid=0;
		for(i=0;i<syn480DataInfo.nBytesReceived;i++)
		{
			dataBuffer[i]=syn480DataInfo.dataBuffer[i];
		}
		return syn480DataInfo.nBytesReceived;
	}
	else
	{
		return 0;
	}
	
}


//������Ϣ��ӡ������
//���Թ۲�����źŵ�ÿ���ߵ͵�ƽ��ʱ��
//���Թ۲쳤���������ص�
//��Ҫ��ǰ���úô���

void Remote_Debug(void)
{
	int t;	
	
	if(syn480DataInfo.valid )//�յ�һ����Ч�İ�����Ϣ
	{
		syn480DataInfo.valid=0;//������Ϣ�ѱ�ʹ��
		
		for(t=0;t<sizeof(HighTime)/2;t++)
		{
			printf("num= %-3d, �ߵ�ƽ= %-6d uS ,�͵�ƽ= %-6d uS\r\n",t,HighTime[t]*10,LowTime[t]*10);
		}
		printf("\r\n\r\n");
		
		for(t=0;t<sizeof(HighTime)/2;t++)
		{
			HighTime[t]=0;
			LowTime[t]=0;
		}
		
					
	}	
	
}


