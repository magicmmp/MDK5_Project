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
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //ʹ�� GPIOB �˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //BEEP-->GPIOB.8 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //�ٶ�Ϊ 50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure); //���ݲ�����ʼ�� GPIOB.8
    GPIO_ResetBits(GPIOB,GPIO_Pin_8); //��� 0���رշ��������
  
  
    while(1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_8); //PB.5 �����
      
        Delay(0x1fffff);
      
        GPIO_ResetBits(GPIOB,GPIO_Pin_8); //��� 0���رշ��������
        Delay(0x1fffff);
      
    }
  
    
  
	
}


/*********************************************END OF FILE**********************/
