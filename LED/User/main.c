#include "stm32f10x.h"


 void Delay(u32 count)
 {
	   u32 i=0;                         
	   for(;i<count;i++);
     
    
 }
 

 //运行在淘宝买的那块stm32最小核心板
 int main(void)
 {  	
									
	  GPIO_InitTypeDef  GPIO_InitStructure;
	 
                    
	 
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);//使能GPIOB和GPIOE时钟
		 //引脚输出模式设置
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			 //对引脚5设置
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //引脚速度50M
	  GPIO_Init(GPIOC, &GPIO_InitStructure);			 //将设置应用到GPIOB
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);	  	     //将设置应用到GPIOE 

	while(1)
	{
		
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
		Delay(3000000);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		Delay(3000000);
	}
 }
