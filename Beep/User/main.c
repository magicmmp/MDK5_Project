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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //使能 GPIOB 端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //BEEP-->GPIOB.8 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //速度为 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure); //根据参数初始化 GPIOB.8
    GPIO_ResetBits(GPIOB,GPIO_Pin_8); //输出 0，关闭蜂鸣器输出
  
  
    while(1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_8); //PB.5 输出高
      
        Delay(0x1fffff);
      
        GPIO_ResetBits(GPIOB,GPIO_Pin_8); //输出 0，关闭蜂鸣器输出
        Delay(0x1fffff);
      
    }
  
    
  
	
}


/*********************************************END OF FILE**********************/
