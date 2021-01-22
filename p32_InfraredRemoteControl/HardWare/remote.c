#include "remote.h"
#include "bsp_usart.h" //�������ڴ�ӡ��Ϣ



// ������Ϣ�ṹ�嶨��
KeyPressInfoTypeDef KeyPressInfo  = {0,0,0,0,0};

// �ж����ȼ�����
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ���벶��ͨ�� GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CHx_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL_TIM_CHx_PORT, &GPIO_InitStructure);	
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
    
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNEL_x;
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL_TIM, &TIM_ICInitStructure);
	
	// ������ºͲ����жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);	
  // �������ºͲ����ж�  
	TIM_ITConfig (GENERAL_TIM, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE );
	
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL_TIM, ENABLE);
}



//����ң�س�ʼ��
//����IO�Լ���ʱ��4�����벶��
void Remote_Init(void)    			  
{  
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();								 
}

//����������
//����ֵ:
//	 0,û���κΰ�������
//����,���µİ�����ֵ.
//ʹ���������ֻ�ܵ�֪�����Ǹ�����
//�����ܺܺ����ó����Ĺ���
u8 Remote_Scan(void)
{
	u8 tmp;
	u8 keyCode=0;
	if(KeyPressInfo.valid)//�յ�һ����Ч�İ�����Ϣ
	{
		keyCode=(KeyPressInfo.code>>8) & 0xff;//����
		tmp=KeyPressInfo.code & 0xff;//�����
		if(keyCode!=(u8)~tmp)
			keyCode=0;
		KeyPressInfo.valid=0;
	}
	return keyCode;	
}

//static���εı���ֻ�ܱ��ļ���ʹ��
static uint8_t  Polarity_index=0;  //����������ã��½���0��������1	
const  uint16_t PolaritySelect[2]={TIM_ICPolarity_Falling,TIM_ICPolarity_Rising};
static uint16_t HighTime[200];//���ڼ�¼����ĸߵ�ƽ��ʱ������λΪ10uS
static uint16_t LowTime[200]; //���ڼ�¼����ĵ͵�ƽ��ʱ������λΪ10uS
static uint16_t tmpHigh;//��Ÿߵ�ƽʱ�����ʱ����
static uint16_t tmpLow; //��ŵ͵�ƽʱ�����ʱ����


//��ʱ��4�жϷ������	 
void GENERAL_TIM_INT_FUN(void)
{
	
	// ���ز����ж�
	if ( TIM_GetITStatus (GENERAL_TIM, GENERAL_TIM_IT_CCx ) != RESET)
	{
		// ��������0
		TIM_SetCounter ( GENERAL_TIM, 0 );
		
		
		if(Polarity_index) //������ڲ������������
		{
			Polarity_index=0;
			tmpLow=GENERAL_TIM_GetCapturex_FUN (GENERAL_TIM);//�Ƚ��͵�ƽʱ�䱣���
			
		}
		else
		{
			Polarity_index=1;
			tmpHigh=GENERAL_TIM_GetCapturex_FUN (GENERAL_TIM);//�Ƚ��ߵ�ƽʱ�䱣���
			if(tmpLow>850 && tmpLow<950)
			if(tmpHigh>400 && tmpHigh<500) //�յ� ͬ����ͷ
			{
				KeyPressInfo.valid=0;//2021-1-22 add
				KeyPressInfo.isPressing=1;
				KeyPressInfo.nPulse=0;
				KeyPressInfo.nLongPress=0;
				KeyPressInfo.code=0;
			}
			if(KeyPressInfo.isPressing)//������ڰ��°���
			{
				if(KeyPressInfo.nPulse<sizeof(HighTime)/2)
				{
					HighTime[KeyPressInfo.nPulse]=tmpHigh;
					LowTime[KeyPressInfo.nPulse]=tmpLow;
				}
				HighTime[KeyPressInfo.nPulse]=tmpHigh;
				LowTime[KeyPressInfo.nPulse]=tmpLow;
				//ͬ����֮���32λ��ʾ ��������
				if(KeyPressInfo.nPulse>0 && KeyPressInfo.nPulse<33)
				{
					if(tmpHigh>100)
						KeyPressInfo.code|=0x1;
					if(KeyPressInfo.nPulse<32)
						KeyPressInfo.code<<=1;
				}
				if(KeyPressInfo.nPulse==32)  //�ѻ����������ı��루4�ֽ���Ϣ��
					KeyPressInfo.valid=1;
				
				if(KeyPressInfo.nPulse>34)  //������ʱ
				{
					KeyPressInfo.nLongPress=(KeyPressInfo.nPulse-33)>>1;
				}
				KeyPressInfo.nPulse++; //��������1
			}			
		}
			
		TIM_ClearITPendingBit (GENERAL_TIM,GENERAL_TIM_IT_CCx);	
		// ���Ĳ����������
		GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM,PolaritySelect[Polarity_index] );		
	}

	//�����ɿ�120mS����ʱ��TIM4�������
	if ( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
		
		if(KeyPressInfo.isPressing)
		{
			KeyPressInfo.isPressing=0;//�����ɿ���
		}
			
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 	
	}
}


//������Ϣ��ӡ������
//���Թ۲�����źŵ�ÿ���ߵ͵�ƽ��ʱ��
//���Թ۲쳤���������ص�
//��Ҫ��ǰ���úô���

void Remote_Debug(void)
{
	int t=0,nextIndex;	
	
	if(KeyPressInfo.valid)//�յ�һ����Ч�İ�����Ϣ
	{
		
		printf("num= %-3d, �͵�ƽ= %-6d uS, �ߵ�ƽ= %-6d uS\r\n\r\n",0,LowTime[0]*10,HighTime[0]*10);
		
		for(t=1;t<=32;t++)
		{
			printf("num= %-3d, �͵�ƽ= %-6d uS, �ߵ�ƽ= %-6d uS\r\n",t,LowTime[t]*10,HighTime[t]*10);
		}
	
		printf("\r\nnum= %-3d, �͵�ƽ= %-6d uS, �ߵ�ƽ= %-6d uS\r\n\r\n",33,LowTime[33]*10,HighTime[33]*10);
		

		nextIndex=0;
		while(KeyPressInfo.isPressing)
		for(t=nextIndex;t<KeyPressInfo.nLongPress;t++)
		{
			printf("������...\r\n");
			printf("num= %-3d, �͵�ƽ= %-6d uS, �ߵ�ƽ= %-6d uS\r\n",t,LowTime[t*2+34]*10,HighTime[t*2+34]*10);
			printf("num= %-3d, �͵�ƽ= %-6d uS, �ߵ�ƽ= %-6d uS\r\n",t,LowTime[t*2+35]*10,HighTime[t*2+35]*10);
			nextIndex=t+1;
		}
		KeyPressInfo.valid=0;//������Ϣ�ѱ�ʹ��
		
		printf("\r\n4�ֽڰ������룺 0X%04X\r\n",KeyPressInfo.code);	
		for(t=31;t>=0;t--)
		{
			if((KeyPressInfo.code>>t)&0x1)
				printf("1");
			else
				printf("0");
			
			if(t%8==0)
				printf("\r\n");
		}
		printf("\r\n\r\n");
					
	}	
	
}

































