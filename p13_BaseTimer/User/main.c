#include "stm32f10x.h"
#include "led.h"
#include "bsp_TiMbase.h" 

uint32_t time=0; //��¼��ʱ���жϴ���


/**
  * @brief  ��ʱ���жϺ���.
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
ʹ�û�����ʱ��TIM6�������ж�����Ϊ1ms��
���жϴ�������¼�жϴ�������1000��ʱ��תLED�ơ�

*/

 
 int main(void)
 {  
	 LED_Init();
	 BASIC_TIM_Init();
		
              
	
	
  while(1)
  {
    if ( time == 1000 ) /*ÿ���ж�����Ϊ1ms,1000��Ϊ 1s  */
    {
       time = 0;
	   LED0=!LED0;//��תLED��
	   LED1=!LED1;
    }        
  }
                    
	 
	
 }
