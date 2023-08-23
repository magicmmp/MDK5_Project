#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"




/**
  * @brief  可以烧写到正点原子开发板
  * @param  无  
  * @retval 无
  */
int main(void)
{
    
	
	GPIO_InitTypeDef GPIO_InitStructure;
	delay_init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //使能 GPIOB 端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //BEEP-->GPIOB.8 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //速度为 50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); //根据参数初始化 GPIOB.8
    GPIO_ResetBits(GPIOA,GPIO_Pin_5); //输出 0，关闭蜂鸣器输出
  
  
    while(1)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_5); //PB.5 输出高
      
        delay_us(1000);
      
        GPIO_ResetBits(GPIOA,GPIO_Pin_5); //输出 0，关闭蜂鸣器输出
        delay_us(1000);;
      
    }
  
    
  
	
}


/*********************************************END OF FILE**********************/
