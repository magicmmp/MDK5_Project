#include "stm32f10x.h"
#include "led.h"
#include "bsp_TiMbase.h" 

uint32_t time=0; //记录定时器中断次数


/**
  * @brief  定时器中断函数.
  * @param  None
  * @retval None
  */
void  TIM6_IRQHandler(void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		time++;
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);  		 
	}		 	
}


/**
使用基本定时器TIM6，设置中断周期为1ms。
在中断处理函数记录中断次数，当1000次时翻转LED灯。

*/

 
 int main(void)
 {  
	 LED_Init();
	 BASIC_TIM_Init();
		
              
	
	
  while(1)
  {
    if ( time == 1000 ) /*每次中断周期为1ms,1000次为 1s  */
    {
       time = 0;
	   LED0=!LED0;//翻转LED灯
	   LED1=!LED1;
    }        
  }
                    
	 
	
 }
