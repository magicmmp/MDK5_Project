#include "stm32f10x.h"
#include "stm32f10x_conf.h"



void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  可以烧写到正点原子开发板
  * @param  无  
  * @retval 无
  */
int main(void)
{	
  
    GPIO_InitTypeDef GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE); //使能 PB,PE 端口时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //LED0-->PB.5 推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  
    while(1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_5); //PB.5 输出高
        GPIO_SetBits(GPIOE,GPIO_Pin_5); //PE.5 输出高
      
        Delay(0xfffff);
      
        GPIO_ResetBits(GPIOB,GPIO_Pin_5); //PB.5 输出高
        GPIO_ResetBits(GPIOE,GPIO_Pin_5); //PE.5 输出高
        Delay(0xfffff);
      
    }
  
    
  
	
}


/*********************************************END OF FILE**********************/
