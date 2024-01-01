#include "remote.h"
#include "bsp_usart.h" //�������ڴ�ӡ��Ϣ



// ������Ϣ�ṹ�嶨��
Struct_IR_Info IR_info;

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

//��ң��������ת��Ϊ��е����Ч��
uint8_t IR_Remote_to_key(void) 
{
  uint8_t keyCode=0;
  if(IR_info.valid)
  {
    IR_info.valid=0;
    switch(IR_info.IR_code[2]) 
    {
      case 0x15:keyCode=2;;break; //��е������Ч�Ա�ǲ��ܷ����棬�����κμ����ᴥ����Ч
      case 0x1D:keyCode=3;break;
      case 0x11:keyCode=1;break;        
    }   
  }
  return IR_info.IR_code[2];	
}

//static���εı���ֻ�ܱ��ļ���ʹ��
static uint8_t  Polarity_index=0;  //����������ã��½���0��������1	
const  uint16_t PolaritySelect[2]={TIM_ICPolarity_Falling,TIM_ICPolarity_Rising};
static uint16_t HighTime[50];//���ڼ�¼����ĸߵ�ƽ��ʱ������λΪ10uS
static uint16_t LowTime[50]; //���ڼ�¼����ĵ͵�ƽ��ʱ������λΪ10uS
static uint16_t tmpHigh;//��Ÿߵ�ƽʱ�����ʱ����
static uint16_t tmpLow; //��ŵ͵�ƽʱ�����ʱ����
/**
typedef struct
{   
	uint8_t    valid;        //�յ���Ч������Ϣ�����Ч�����ʺ�Ӧ�ֶ���Ϊ��Ч
  uint8_t    valid_tmp;      //��¼��ǰ�Ƿ�����Ч�Ķ̰��������ڴ�����
  uint8_t    flag_print;     //���ڴ�ӡ�������ݵı��
	uint8_t    isPressing;   // �������ڱ����£��յ�ͬ��ͷ��1���ɿ�120mS��Ϊ0
	uint8_t    nPulse;       //�ܹ��յ���������
	uint8_t    nPulse_tmp;      //�������������ֻΪÿ��4�����巢��һ�γ�������
	uint8_t    IR_code[4];     //�������롣4�ֽڣ��ɸ��ֽڵ����ֽڣ�ң������ַ+��ַ����+����+�����
  uint8_t    idx;    //ָʾ��ǰ�յ��ڼ��ֽ�
}Struct_IR_Info;
**/

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
				IR_info.valid=0;//2021-1-22 add
				IR_info.isPressing=1;
				IR_info.nPulse=0; //Ϊ����ʾ���峤�����ݶ���¼ʱ���м�������
        IR_info.nPulse_tmp=0; //Ϊ�˳���ʱÿ��4�η���һ�γ�������
				IR_info.valid_tmp=0;
				IR_info.IR_code[0]=0;
        IR_info.IR_code[1]=0;
        IR_info.IR_code[2]=0;
        IR_info.IR_code[3]=0;
        IR_info.idx=0;
        IR_info.flag_print=0;
        
			}
			if(IR_info.isPressing)//������ڰ��°���
			{
				//ͬ����֮���32λ��ʾ 4�ֽڰ������룺��ַ�롢��ַ�뷴�롢�����롢�����뷴��
				if(IR_info.nPulse>0 && IR_info.nPulse<33)
				{
          IR_info.IR_code[IR_info.idx]>>=1;
          //ע�⣺tmpHigh >=128Ӧ��д�ɣ�tmpHigh & 0xff80������д��tmpHigh&0xff10��������� 2023-8-23
					if(tmpHigh & 0xff80)  //560us��Ӧ56��1600us��Ӧ160�����������жϣ�tmpHigh&0xff80���൱��tmpHigh>=128
          {
						IR_info.IR_code[IR_info.idx]|=0x80;
          }
          if(!(IR_info.nPulse&0x7))
            IR_info.idx++;    //���浽��һ���ֽ�
				}
        if(IR_info.valid_tmp)//ʵ�ֳ�������,�ɲ��ؼ�¼����������ֻ��Ҫ��һ��u8ѭ����������ʹ�Լ������Ҳ����ν
        {
          if(!(IR_info.nPulse_tmp&0x3))  //IR_info.nPulse_tmpҪ����32�ұ�4����
          {
            IR_info.valid=1; 
          } 
        }
        if(IR_info.nPulse==32)  //�ѻ����������ı��루4�ֽ���Ϣ��
        {
          if((IR_info.IR_code[2]^IR_info.IR_code[3])==0xff) //���ָ����ķ���
          {
            IR_info.valid=1;
            IR_info.valid_tmp=1;
          }
        }
        if(IR_info.nPulse<sizeof(HighTime)>>1)
				{
					HighTime[IR_info.nPulse]=tmpHigh;
					LowTime[IR_info.nPulse]=tmpLow;
          IR_info.nPulse++; //��������1��������ڶ����������
				}
			
        IR_info.nPulse_tmp++;			
			}			
		}
			
		TIM_ClearITPendingBit (GENERAL_TIM,GENERAL_TIM_IT_CCx);	
		// ���Ĳ����������
		GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM,PolaritySelect[Polarity_index] );		
	}

	//�����ɿ�120mS����ʱ��TIM4�������
	if( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
		if(IR_info.isPressing)
		{
			IR_info.isPressing=0;//�����ɿ���
      IR_info.flag_print=1;  //���Դ�ӡ����������
		}
		IR_info.valid_tmp=0; //������Ч�����Ѿ��ɿ�	
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 	
	}
}


//������Ϣ��ӡ������
//���Թ۲�����źŵ�ÿ���ߵ͵�ƽ��ʱ��
//���Թ۲쳤���������ص�
//��Ҫ��ǰ���úô���

void Remote_Debug(void)
{
	uint8_t i,tmp,t;	
	
	if(IR_info.flag_print)//�յ�һ����Ч�İ�����Ϣ
	{
    IR_info.flag_print=0;//������Ϣ�ѱ�ʹ��
		for(t=0;t<IR_info.nPulse;t++)
		{
			printf("num= %3d, �͵�ƽ= %6d uS, �ߵ�ƽ= %6d uS\r\n",t,LowTime[t]*10,HighTime[t]*10);
      if(t==0||t==32||t==33)
        printf("\r\n");
		}
    
    printf("\r\n4�ֽڰ������룺     0x%02X%02X%02X%02X\r\n",IR_info.IR_code[0],IR_info.IR_code[1],IR_info.IR_code[2],IR_info.IR_code[3]);
		printf("\r\n");
		for(i=0;i<4;i++)
    {
      tmp=IR_info.IR_code[i];
      for(t=0;t<8;t++)
      {
        if(tmp&0x80)
          printf("1");
        else
          printf("0");
        tmp<<=1;
      }
      printf("\r\n");
    }
		printf("\r\n\r\n");		
	}		
}

































