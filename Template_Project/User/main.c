#include "stm32f10x.h"
#include "stm32f10x_conf.h"



void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

/**
  * @brief  ������д������ԭ�ӿ�����
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
  
    GPIO_InitTypeDef GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE); //ʹ�� PB,PE �˿�ʱ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //LED0-->PB.5 �������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  
    while(1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_5); //PB.5 �����
        GPIO_SetBits(GPIOE,GPIO_Pin_5); //PE.5 �����
      
        Delay(0xfffff);
      
        GPIO_ResetBits(GPIOB,GPIO_Pin_5); //PB.5 �����
        GPIO_ResetBits(GPIOE,GPIO_Pin_5); //PE.5 �����
        Delay(0xfffff);
      
    }
  
    
  
	
}


/*********************************************END OF FILE**********************/
