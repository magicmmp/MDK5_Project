#include "remote.h"
#include "bsp_usart.h" //串口用于打印信息



// 按键信息结构体定义
KeyPressInfoTypeDef KeyPressInfo  = {0,0,0,0,0};

// 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输入捕获通道 GPIO 初始化
  RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CHx_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL_TIM_CHx_PORT, &GPIO_InitStructure);	
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  // 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/	
    
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
	
	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNEL_x;
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL_TIM, &TIM_ICInitStructure);
	
	// 清除更新和捕获中断标志位
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);	
  // 开启更新和捕获中断  
	TIM_ITConfig (GENERAL_TIM, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE );
	
	// 使能计数器
	TIM_Cmd(GENERAL_TIM, ENABLE);
}



//红外遥控初始化
//设置IO以及定时器4的输入捕获
void Remote_Init(void)    			  
{  
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();								 
}

//处理红外键盘
//返回值:
//	 0,没有任何按键按下
//其他,按下的按键键值.
//使用这个函数只能得知按下那个按键
//而不能很好利用长按的功能
u8 Remote_Scan(void)
{
	u8 tmp;
	u8 keyCode=0;
	if(KeyPressInfo.valid)//收到一个有效的按键信息
	{
		keyCode=(KeyPressInfo.code>>8) & 0xff;//命令
		tmp=KeyPressInfo.code & 0xff;//命令反码
		if(keyCode!=(u8)~tmp)
			keyCode=0;
		KeyPressInfo.valid=0;
	}
	return keyCode;	
}

//static修饰的变量只能本文件内使用
static uint8_t  Polarity_index=0;  //捕获边沿设置，下降沿0或上升沿1	
const  uint16_t PolaritySelect[2]={TIM_ICPolarity_Falling,TIM_ICPolarity_Rising};
static uint16_t HighTime[200];//用于记录脉冲的高电平的时长，单位为10uS
static uint16_t LowTime[200]; //用于记录脉冲的低电平的时长，单位为10uS
static uint16_t tmpHigh;//存放高电平时间的临时变量
static uint16_t tmpLow; //存放低电平时间的临时变量


//定时器4中断服务程序	 
void GENERAL_TIM_INT_FUN(void)
{
	
	// 边沿捕获中断
	if ( TIM_GetITStatus (GENERAL_TIM, GENERAL_TIM_IT_CCx ) != RESET)
	{
		// 计数器清0
		TIM_SetCounter ( GENERAL_TIM, 0 );
		
		
		if(Polarity_index) //如果现在捕获的是上升沿
		{
			Polarity_index=0;
			tmpLow=GENERAL_TIM_GetCapturex_FUN (GENERAL_TIM);//先将低电平时间保存好
			
		}
		else
		{
			Polarity_index=1;
			tmpHigh=GENERAL_TIM_GetCapturex_FUN (GENERAL_TIM);//先将高电平时间保存好
			if(tmpLow>850 && tmpLow<950)
			if(tmpHigh>400 && tmpHigh<500) //收到 同步码头
			{
				KeyPressInfo.valid=0;//2021-1-22 add
				KeyPressInfo.isPressing=1;
				KeyPressInfo.nPulse=0;
				KeyPressInfo.nLongPress=0;
				KeyPressInfo.code=0;
			}
			if(KeyPressInfo.isPressing)//如果正在按下按键
			{
				if(KeyPressInfo.nPulse<sizeof(HighTime)/2)
				{
					HighTime[KeyPressInfo.nPulse]=tmpHigh;
					LowTime[KeyPressInfo.nPulse]=tmpLow;
				}
				HighTime[KeyPressInfo.nPulse]=tmpHigh;
				LowTime[KeyPressInfo.nPulse]=tmpLow;
				//同步码之后的32位表示 按键编码
				if(KeyPressInfo.nPulse>0 && KeyPressInfo.nPulse<33)
				{
					if(tmpHigh>100)
						KeyPressInfo.code|=0x1;
					if(KeyPressInfo.nPulse<32)
						KeyPressInfo.code<<=1;
				}
				if(KeyPressInfo.nPulse==32)  //已获得这个按键的编码（4字节信息）
					KeyPressInfo.valid=1;
				
				if(KeyPressInfo.nPulse>34)  //长按计时
				{
					KeyPressInfo.nLongPress=(KeyPressInfo.nPulse-33)>>1;
				}
				KeyPressInfo.nPulse++; //脉冲数加1
			}			
		}
			
		TIM_ClearITPendingBit (GENERAL_TIM,GENERAL_TIM_IT_CCx);	
		// 更改捕获边沿配置
		GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM,PolaritySelect[Polarity_index] );		
	}

	//按键松开120mS，定时器TIM4就溢出了
	if ( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
		
		if(KeyPressInfo.isPressing)
		{
			KeyPressInfo.isPressing=0;//按键松开了
		}
			
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 	
	}
}


//调试信息打印到串口
//可以观察红外信号的每个高低电平的时间
//可以观察长按的脉冲特点
//需要提前配置好串口

void Remote_Debug(void)
{
	int t=0,nextIndex;	
	
	if(KeyPressInfo.valid)//收到一个有效的按键信息
	{
		
		printf("num= %-3d, 低电平= %-6d uS, 高电平= %-6d uS\r\n\r\n",0,LowTime[0]*10,HighTime[0]*10);
		
		for(t=1;t<=32;t++)
		{
			printf("num= %-3d, 低电平= %-6d uS, 高电平= %-6d uS\r\n",t,LowTime[t]*10,HighTime[t]*10);
		}
	
		printf("\r\nnum= %-3d, 低电平= %-6d uS, 高电平= %-6d uS\r\n\r\n",33,LowTime[33]*10,HighTime[33]*10);
		

		nextIndex=0;
		while(KeyPressInfo.isPressing)
		for(t=nextIndex;t<KeyPressInfo.nLongPress;t++)
		{
			printf("长按中...\r\n");
			printf("num= %-3d, 低电平= %-6d uS, 高电平= %-6d uS\r\n",t,LowTime[t*2+34]*10,HighTime[t*2+34]*10);
			printf("num= %-3d, 低电平= %-6d uS, 高电平= %-6d uS\r\n",t,LowTime[t*2+35]*10,HighTime[t*2+35]*10);
			nextIndex=t+1;
		}
		KeyPressInfo.valid=0;//按键信息已被使用
		
		printf("\r\n4字节按键编码： 0X%04X\r\n",KeyPressInfo.code);	
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

































