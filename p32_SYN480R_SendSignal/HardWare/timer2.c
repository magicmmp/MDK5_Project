#include "timer2.h"
#include "sys.h"

//数组元素意义
//开始传输标志位、NEC数据起始位、高电平位、低电平位、未定义
const uint16_t NEC_ARR[5]={1000,1350,225,112,0};
const uint16_t NEC_CCR[5]={200,900,56,56,0};
const uint8_t NEC_DATA[5]={0x12,0x66,0x56,0x65,0};


const uint16_t CCRV[5]={180,150,100,80,50};
uint16_t CCRV_index=0;

void TIM2_PWM_Init()
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); //Timer2部分重映射  TIM2_CH3->PB10    
 
   //设置该引脚为复用输出功能,输出TIM2 CH3的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = 200; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =719; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = CCRV[0];
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	// 输出通道空闲电平极性配置
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	

}

uint16_t test_time2=0;
u8 bitValue;
//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		if(bitsToSend>0)
			bitsToSend--;
		if(bitsToSend>0 && bitsToSend<33)
		{
			bitValue=32-bitsToSend;
			bitValue=NEC_DATA[bitValue/8]&(0x1<<(bitValue&0x7));
			if(bitValue)//发送位1
			{
				TIM_SetCompare3(TIM2,NEC_CCR[2]);
				TIM_SetAutoreload(TIM2,NEC_ARR[2]);
			}
			else//发送位0
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
		else
		{
			TIM_Cmd(TIM2, DISABLE);
		}
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		test_time2++;
	}
}



u8 bitsToSend;
void NECSendByte()
{
	bitsToSend=34;
	TIM_SetCompare3(TIM2,NEC_CCR[0]);
	TIM_SetAutoreload(TIM2,NEC_ARR[0]);
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
	
}

