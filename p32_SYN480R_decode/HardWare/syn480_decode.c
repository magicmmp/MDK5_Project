#include "syn480_decode.h"
#include "stm32f10x.h"



u16 nSysTick=0;//记录进入SysTick中断的次数
u16 nTIM2_CCR=0;//记录进入TIM2中断的次数


// 按键信息结构体定义
syn480DataInfoTypeDef syn480DataInfo  = {0,0,0,0,0};

void syn480_Init()
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); //Timer2部分重映射  TIM2_CH3->PB10
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器2时钟
 	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	    
 
   //设置该引脚为复用输出功能,输出TIM2 CH3的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TIM_CH3
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //TIM_CH3
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;  //复用推挽输出GPIO_Mode_IPD
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = 12000; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =719; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	//TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	//初始化TIM2 Channel3 输入捕获模式	 
	/*--------------------输入捕获结构体初始化-------------------*/	
	
	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	//TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	//TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC3);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM2, TIM_IT_Update | TIM_IT_CC3, ENABLE );
	
	/**
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(TIM4, TIM_FLAG_Update|TIM_IT_CC4);	
  // 开启更新和捕获中断  
	TIM_ITConfig (TIM4, TIM_IT_Update | TIM_IT_CC4, ENABLE );
*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
	//NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	
	// 使能计数器
	TIM_Cmd(TIM2, ENABLE);
	//TIM_Cmd(TIM4, ENABLE);
	syn480DataInfo.nBytesReceived=0;
}


//static修饰的变量只能本文件内使用
static uint8_t  Polarity_index=0;  //捕获边沿设置，上升沿0或下降沿1	
const  uint16_t PolaritySelect[2]={TIM_ICPolarity_Rising,TIM_ICPolarity_Falling};
static uint16_t HighTime[36];//用于记录脉冲的高电平的时长，单位为10uS
static uint16_t LowTime[36]; //用于记录脉冲的低电平的时长，单位为10uS
static uint16_t tmpHigh;//存放高电平时间的临时变量
static uint16_t tmpLow; //存放低电平时间的临时变量


//无线信号距离越远，信号越弱
//则测得高电平时间会变小，低电平时间变大
//就会出现错误。所以高和低电平的脉冲持续时间都不能太短。
//定时器2中断服务程序	 
void TIM2_IRQHandler(void)
//void TIM4_IRQHandler(void)
{
	// 边沿捕获中断
	//if ( TIM_GetITStatus (TIM4, TIM_IT_CC4 ) != RESET)
	if ( TIM_GetITStatus (TIM2, TIM_IT_CC3 ) != RESET)
	{
		
		nTIM2_CCR++;//记录进入中断的次数
		// 计数器清0
		TIM_SetCounter ( TIM2, 0 );
		//TIM_SetCounter ( TIM4, 0 );
		
		
		if(Polarity_index==0) //如果现在捕获的是上升沿
		{
			Polarity_index=1;
			//tmpLow=TIM_GetCapture4 (TIM4);//先将低电平时间保存好
			tmpLow=TIM_GetCapture3 (TIM2);
			
			if(tmpLow>400 && tmpLow<600)
			if(tmpHigh>900 && tmpHigh<1100) //收到 同步码头
			{
				syn480DataInfo.isPressing=1;
				syn480DataInfo.nBytesReceived=0;
				syn480DataInfo.byteTmp=0;
				syn480DataInfo.nPulse=0;
				syn480DataInfo.nPulseToWait=16;
				syn480DataInfo.ErrNo=0;
			}
			if(syn480DataInfo.isPressing)//如果正在按下按键
			{
				if(syn480DataInfo.nPulse<sizeof(HighTime)/2)
				{
					HighTime[syn480DataInfo.nPulse]=tmpHigh;
					LowTime[syn480DataInfo.nPulse]=tmpLow;
				}
				
			
				//同步码之后的32位表示 按键编码
				if(syn480DataInfo.nPulse>0)
				{
					if(tmpLow>80)
						syn480DataInfo.byteTmp|=0x80;
						
					if(syn480DataInfo.nPulse&0x7)
					{
						syn480DataInfo.byteTmp>>=1;
					}
					else//每收到一个字节
					{
						if(syn480DataInfo.nPulse&0xf)//第一个字节
						{
							syn480DataInfo.byteTmp0=syn480DataInfo.byteTmp;
							//syn480DataInfo.dataBuffer[(syn480DataInfo.nPulse>>3)-2]=syn480DataInfo.byteTmp;
						}
						else//收到字节+字节反码
						{
								
							if(syn480DataInfo.byteTmp0+syn480DataInfo.byteTmp==0xff)
							{
								
								if(syn480DataInfo.nPulse==16)
								{
									if(syn480DataInfo.byteTmp0)//后面有一些字节要接收
									{
										syn480DataInfo.nBytesReceived=syn480DataInfo.byteTmp0;
										syn480DataInfo.nPulseToWait=syn480DataInfo.byteTmp0;
										syn480DataInfo.nPulseToWait<<=4;
										syn480DataInfo.nPulseToWait+=16;
									}
									else
									{
										syn480DataInfo.valid=1;
										syn480DataInfo.isPressing=0;//强制停止解码
										syn480DataInfo.ErrNo=1;//数据正确
									}
								}
								else
								{
									syn480DataInfo.dataBuffer[(syn480DataInfo.nPulse>>4)-2]=syn480DataInfo.byteTmp0;
									if(syn480DataInfo.nPulse==syn480DataInfo.nPulseToWait)
									{
										syn480DataInfo.valid=1;
										syn480DataInfo.ErrNo=1;//数据正确
										syn480DataInfo.isPressing=0;//强制停止解码
									}	
								}		
								
							}
							else
							{
									syn480DataInfo.ErrNo=2;//数据出错，可能是有干扰
									syn480DataInfo.isPressing=0;//强制停止解码
							}
							
						}
						syn480DataInfo.byteTmp=0;
					}
					
						
				}
				
				
					
				syn480DataInfo.nPulse++; //脉冲数加1
			}

		}
		else
		{
			Polarity_index=0;
			tmpHigh=TIM_GetCapture3 (TIM2);
			//tmpHigh=TIM_GetCapture4 (TIM4);//先将高电平时间保存好
						
		}
			
		TIM_ClearITPendingBit (TIM2,TIM_IT_CC3);	
		// 更改捕获边沿配置
		TIM_OC3PolarityConfig(TIM2,PolaritySelect[Polarity_index] );	
		
		/**
		TIM_ClearITPendingBit (TIM4,TIM_IT_CC4);	
		// 更改捕获边沿配置
		TIM_OC4PolarityConfig(TIM4,PolaritySelect[Polarity_index] );		
		*/
	}
/**
	//按键松开120mS，定时器TIM4就溢出了
	if ( TIM_GetITStatus ( TIM4, TIM_IT_Update) != RESET )               
	{	
		if(syn480DataInfo.isPressing)
		{
			syn480DataInfo.isPressing=0;//按键松开了
		}
		TIM_ClearITPendingBit ( TIM4, TIM_FLAG_Update ); 	
	}
	*/
	if( TIM_GetITStatus ( TIM2, TIM_IT_Update) != RESET )               
	{	
		if(syn480DataInfo.isPressing)
		{
			syn480DataInfo.ErrNo=3;//数据出错，可能是信号中断
			syn480DataInfo.isPressing=0;//强制停止解码
		}
		TIM_ClearITPendingBit ( TIM2, TIM_FLAG_Update ); 	
	}
	
	
	
	
}

//查询是否出错码
//好像不能查出类似干扰的错误
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
//输入参数 dataBuffer[] ：用于接收数据
//返回值：收到了多少个字节，0表示还没收到任何字节
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


//调试信息打印到串口
//可以观察红外信号的每个高低电平的时间
//可以观察长按的脉冲特点
//需要提前配置好串口

void Remote_Debug(void)
{
	int t;	
	
	if(syn480DataInfo.valid )//收到一个有效的按键信息
	{
		syn480DataInfo.valid=0;//按键信息已被使用
		
		for(t=0;t<sizeof(HighTime)/2;t++)
		{
			printf("num= %-3d, 高电平= %-6d uS ,低电平= %-6d uS\r\n",t,HighTime[t]*10,LowTime[t]*10);
		}
		printf("\r\n\r\n");
		
		for(t=0;t<sizeof(HighTime)/2;t++)
		{
			HighTime[t]=0;
			LowTime[t]=0;
		}
		
					
	}	
	
}


