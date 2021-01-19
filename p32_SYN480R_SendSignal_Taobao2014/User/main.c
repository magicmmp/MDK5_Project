	
#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "stdlib.h"
#include "timer2.h"
#include "delay.h"

int main()
{		

	u8 key; //按键值
	u8 x=0;
	
	u16 index;
	u8 dataToSend[256];
	u8 n;
	
	
	
	//SCB->VTOR = FLASH_BASE | 0x10000;
	
	 delay_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	 KEY_Init();
	 LED_Init();
	 
	 PB10_Init();
	
	//TIM2_PWM_Init();
	
	
	

	 
	 
	
	
	
	
	 
	
	  while (1)
	  {
			//LED_RUN();
			
			key=KEY_Scan(0);	//得到键值
			
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
					
					case KEY1_PRES:	//控制LED1翻转	 
								//SimpleSignal();
							 //NECSendByte();
								for(index=0;index<256;index++)
								{
									dataToSend[index]=index+x;
								}
								
								x++;
							 syn480r_SendBytes(dataToSend,n);
							 
						break;
					case KEY0_PRES:	//同时控制LED0,LED1翻转
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

