#include "stm32f10x.h"


 void Delay(u32 count)
 {
	   u32 i=0;                         
	   for(;i<count;i++);
     
    
 }
 

 //�������Ա�2014����ǿ�stm32��
 int main(void)
 {  	
									
	  GPIO_InitTypeDef  GPIO_InitStructure;
	 
                    
	 
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);//ʹ��GPIOB��GPIOEʱ��
		 //�������ģʽ����
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			 //������5����
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�����ٶ�50M
	  GPIO_Init(GPIOD, &GPIO_InitStructure);			 //������Ӧ�õ�GPIOB
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	  GPIO_Init(GPIOG, &GPIO_InitStructure);	  	     //������Ӧ�õ�GPIOE 

	while(1)
	{
		
		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		GPIO_SetBits(GPIOG,GPIO_Pin_14);
		Delay(3000000);
		GPIO_SetBits(GPIOD,GPIO_Pin_13);
		GPIO_ResetBits(GPIOG,GPIO_Pin_14);
		Delay(3000000);
	}
 }
