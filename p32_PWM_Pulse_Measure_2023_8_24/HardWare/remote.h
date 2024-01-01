#ifndef __RED_H
#define __RED_H 
#include "sys.h"   

//适合直接运行在正点原子的stm32F103精英板
/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里使用TIM4的通道4来接收红外遥控的信号，即利用引脚PB9

#define            GENERAL_TIM                   TIM4
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM4
//120mS后定时器会溢出
#define            GENERAL_TIM_PERIOD            12000
//定时器时钟分频后，加一个数要10uS
#define            GENERAL_TIM_PSC              (720-1)

// TIM 输入捕获通道GPIO相关宏定义
//红外接收头连在PB9
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CHx_PORT          GPIOB
#define            GENERAL_TIM_CHx_PIN           GPIO_Pin_9
#define            GENERAL_TIM_CHANNEL_x         TIM_Channel_4

// 中断相关宏定义
#define            GENERAL_TIM_IT_CCx            TIM_IT_CC4
#define            GENERAL_TIM_IRQ               TIM4_IRQn
#define            GENERAL_TIM_INT_FUN           TIM4_IRQHandler

// 获取捕获寄存器值函数宏定义
#define            GENERAL_TIM_GetCapturex_FUN                 TIM_GetCapture4
// 捕获信号极性函数宏定义
#define            GENERAL_TIM_OCxPolarityConfig_FUN           TIM_OC4PolarityConfig

// 按键信息结构体声明
typedef struct
{   
	uint8_t    valid;        //收到有效按键信息后变有效。访问后应手动设为无效
  uint8_t    valid_tmp;      //记录当前是否按下有效的短按键，用于处理长按
  uint8_t    flag_print;     //用于打印脉冲数据的标记
	uint8_t    isPressing;   // 按键正在被按下，收到同步头置1，松开120mS后为0
	uint8_t    nPulse;       //总共收到几个脉冲
	uint8_t    nPulse_tmp;      //脉冲序号自增，只为每隔4个脉冲发送一次长按按键
	uint8_t    IR_code[4];     //按键编码。4字节，由高字节到低字节：遥控器地址+地址反码+命令+命令反码
 // uint32_t   IR_code_test;
  uint8_t    idx;    //指示当前收到第几字节
}Struct_IR_Info;



extern Struct_IR_Info IR_info ;


void      Remote_Init(void);    	//红外传感器接收头引脚初始化
uint8_t   IR_Remote_to_key(void);       //遥控器键盘扫描
void 	  Remote_Debug(void);
#endif















