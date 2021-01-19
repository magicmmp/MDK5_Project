	
#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "stdlib.h"
#include "timer2.h"
#include "delay.h"

int main()
{		

	u8 key; //����ֵ
	u8 x=0;
	
	u16 index;
	u8 dataToSend[256];
	u8 n;
	
	
	
	//SCB->VTOR = FLASH_BASE | 0x10000;
	
	 delay_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 KEY_Init();
	 LED_Init();
	 
	 PB10_Init();
	
	//TIM2_PWM_Init();
	
	
	

	 
	 
	
	
	
	
	 
	
	  while (1)
	  {
			//LED_RUN();
			
			key=KEY_Scan(0);	//�õ���ֵ
			
			delay_ms(1000);			
			key=KEY1_PRES;
			if(n==0)
				n=1;
			else if(n==128)
				n=255;
			else if(n==255)
				n=1;
			else
				n=n<<1;
		//  n=32;
	   	if(key)
			{						   
				switch(key)
				{				 
					
					case KEY1_PRES:	//����LED1��ת	 
								//SimpleSignal();
							 //NECSendByte();
								for(index=0;index<256;index++)
								{
									dataToSend[index]=index+x;
								}
								
								x++;
							 syn480r_SendBytes(dataToSend,n);
							 
						break;
					case KEY0_PRES:	//ͬʱ����LED0,LED1��ת
						  if(n==0)
								n=1;
							else if(n==128)
								n=255;
							else if(n==255)
								n=0;
							else
								n=n<<1;
							LED0=!LED0;
							LED1=!LED0;
							
						break;
				}
      }
			//TestSignal();
			//NECSendByte();
	
				
				LED0=!LED0;
				LED1=!LED0;
				
	  
	  }
}

/***********************************END OF FILE*********************************/

