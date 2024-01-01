#include "remote.h"
#include "bsp_usart.h" //串口用于打印信息



// 按键信息结构体定义
Struct_IR_Info IR_info;

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

//把遥控器按键转换为机械按键效果
uint8_t IR_Remote_to_key(void) 
{
  uint8_t keyCode=0;
  if(IR_info.valid)
  {
    IR_info.valid=0;
    switch(IR_info.IR_code[2]) 
    {
      case 0x15:keyCode=2;;break; //机械按键有效性标记不能放外面，否则任何键都会触发有效
      case 0x1D:keyCode=3;break;
      case 0x11:keyCode=1;break;        
    }   
  }
  return IR_info.IR_code[2];	
}

//static修饰的变量只能本文件内使用
static uint8_t  Polarity_index=0;  //捕获边沿设置，下降沿0或上升沿1	
const  uint16_t PolaritySelect[2]={TIM_ICPolarity_Falling,TIM_ICPolarity_Rising};
static uint16_t HighTime[50];//用于记录脉冲的高电平的时长，单位为10uS
static uint16_t LowTime[50]; //用于记录脉冲的低电平的时长，单位为10uS
static uint16_t tmpHigh;//存放高电平时间的临时变量
static uint16_t tmpLow; //存放低电平时间的临时变量
/**
typedef struct
{   
	uint8_t    valid;        //收到有效按键信息后变有效。访问后应手动设为无效
  uint8_t    valid_tmp;      //记录当前是否按下有效的短按键，用于处理长按
  uint8_t    flag_print;     //用于打印脉冲数据的标记
	uint8_t    isPressing;   // 按键正在被按下，收到同步头置1，松开120mS后为0
	uint8_t    nPulse;       //总共收到几个脉冲
	uint8_t    nPulse_tmp;      //脉冲序号自增，只为每隔4个脉冲发送一次长按按键
	uint8_t    IR_code[4];     //按键编码。4字节，由高字节到低字节：遥控器地址+地址反码+命令+命令反码
  uint8_t    idx;    //指示当前收到第几字节
}Struct_IR_Info;
**/

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
				IR_info.valid=0;//2021-1-22 add
				IR_info.isPressing=1;
				IR_info.nPulse=0; //为了显示脉冲长度数据而记录时间有几个脉冲
        IR_info.nPulse_tmp=0; //为了长按时每隔4次发送一次长按按键
				IR_info.valid_tmp=0;
				IR_info.IR_code[0]=0;
        IR_info.IR_code[1]=0;
        IR_info.IR_code[2]=0;
        IR_info.IR_code[3]=0;
        IR_info.idx=0;
        IR_info.flag_print=0;
        
			}
			if(IR_info.isPressing)//如果正在按下按键
			{
				//同步码之后的32位表示 4字节按键编码：地址码、地址码反码、命令码、命令码反码
				if(IR_info.nPulse>0 && IR_info.nPulse<33)
				{
          IR_info.IR_code[IR_info.idx]>>=1;
          //注意：tmpHigh >=128应该写成：tmpHigh & 0xff80。不能写成tmpHigh&0xff10，否则出错。 2023-8-23
					if(tmpHigh & 0xff80)  //560us对应56，1600us对应160，可以这样判断：tmpHigh&0xff80。相当于tmpHigh>=128
          {
						IR_info.IR_code[IR_info.idx]|=0x80;
          }
          if(!(IR_info.nPulse&0x7))
            IR_info.idx++;    //保存到下一个字节
				}
        if(IR_info.valid_tmp)//实现长按功能,可不必记录总脉冲数，只需要用一个u8循环递增，即使自加溢出了也无所谓
        {
          if(!(IR_info.nPulse_tmp&0x3))  //IR_info.nPulse_tmp要大于32且被4整除
          {
            IR_info.valid=1; 
          } 
        }
        if(IR_info.nPulse==32)  //已获得这个按键的编码（4字节信息）
        {
          if((IR_info.IR_code[2]^IR_info.IR_code[3])==0xff) //检测指令码的反码
          {
            IR_info.valid=1;
            IR_info.valid_tmp=1;
          }
        }
        if(IR_info.nPulse<sizeof(HighTime)>>1)
				{
					HighTime[IR_info.nPulse]=tmpHigh;
					LowTime[IR_info.nPulse]=tmpLow;
          IR_info.nPulse++; //脉冲数加1，必须放在读码等语句后面
				}
			
        IR_info.nPulse_tmp++;			
			}			
		}
			
		TIM_ClearITPendingBit (GENERAL_TIM,GENERAL_TIM_IT_CCx);	
		// 更改捕获边沿配置
		GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM,PolaritySelect[Polarity_index] );		
	}

	//按键松开120mS，定时器TIM4就溢出了
	if( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
		if(IR_info.isPressing)
		{
			IR_info.isPressing=0;//按键松开了
      IR_info.flag_print=1;  //可以打印脉冲数据了
		}
		IR_info.valid_tmp=0; //本次有效按键已经松开	
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 	
	}
}


//调试信息打印到串口
//可以观察红外信号的每个高低电平的时间
//可以观察长按的脉冲特点
//需要提前配置好串口

void Remote_Debug(void)
{
	uint8_t i,tmp,t;	
	
	if(IR_info.flag_print)//收到一个有效的按键信息
	{
    IR_info.flag_print=0;//按键信息已被使用
		for(t=0;t<IR_info.nPulse;t++)
		{
			printf("num= %3d, 低电平= %6d uS, 高电平= %6d uS\r\n",t,LowTime[t]*10,HighTime[t]*10);
      if(t==0||t==32||t==33)
        printf("\r\n");
		}
    
    printf("\r\n4字节按键编码：     0x%02X%02X%02X%02X\r\n",IR_info.IR_code[0],IR_info.IR_code[1],IR_info.IR_code[2],IR_info.IR_code[3]);
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

































