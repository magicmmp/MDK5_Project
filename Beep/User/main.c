#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "delay.h"




/**
  * @brief  ������д������ԭ�ӿ�����
  * @param  ��  
  * @retval ��
  */
int main(void)
{
    
	
	GPIO_InitTypeDef GPIO_InitStructure;
	delay_init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //ʹ�� GPIOB �˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //BEEP-->GPIOB.8 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; //�ٶ�Ϊ 50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); //���ݲ�����ʼ�� GPIOB.8
    GPIO_ResetBits(GPIOA,GPIO_Pin_5); //��� 0���رշ��������
  
  
    while(1)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_5); //PB.5 �����
      
        delay_us(1000);
      
        GPIO_ResetBits(GPIOA,GPIO_Pin_5); //��� 0���رշ��������
        delay_us(1000);;
      
    }
  
    
  
	
}


/*********************************************END OF FILE**********************/
