#include "stm32f10x.h"
#include "bsp_usart.h"
#include "spi.h"
#include "key.h"
#include "24l01.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"



 
 
 
 /**
 *  1、
 *  2、同时串口可接收电脑串口助手发来的一串英文字符，显示在mini12864上。
 */
 

int main(void)
{  	

	 u8 key,mode;
	 u16 t=0;			 
	 u8 tmp_buf[33];	
	delay_init();
	LED_Init();

#ifdef TaoBao_STM32_Board_2014  //如果是淘宝2014年买的那块板
	for(t=0;t<10;t++)
	{
		LED0=!LED0;
		LED1=!LED1;
 		delay_ms(1000);
	}
#endif
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	 
	 KEY_Init();					//初始化按键
	 NRF24L01_Init();    		//初始化NRF24L01 
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	/* 初始化USART */
	 USART_Config(); 
	
	

	 
	while(NRF24L01_Check())
	{
		WriteEn(0,0,(uint8_t*)"NRF24L01 Error");
		LED0=!LED0;
		LED1=!LED1;
 		delay_ms(500);
		
	}
	
	while(1)
	{	
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			mode=0;   
			break;
		}
		else if(key==KEY1_PRES)
		{
			mode=1;
			break;
		}
		t++;
		if(t==100)
		{
			WriteEn(0,0,(uint8_t*)"KEY0: RX_Mode"); //闪烁显示提示信息
		    WriteEn(0,2,(uint8_t*)"KEY1: TX_Mode"); //闪烁显示提示信息	
			LED0=!LED0;
			LED1=!LED1;
		}
 		if(t==200)
		{	
			t=0; 
		}
		delay_ms(5);	  
	}   
	
#ifdef TaoBao_STM32_Board_2014  //如果是淘宝2014年买的那块板
	mode=1;
#endif
	
	if(mode==0)//RX模式
	{
		WriteEn(0,0,(uint8_t*)"NRF24L01 RX_Mode");	
		WriteEn(0,2,(uint8_t*)"Received DATA:");	
		NRF24L01_RX_Mode();		  
		while(1)
		{	  		    		    				 
			if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
			{
				tmp_buf[32]=0;//加入字符串结束符
				WriteEn(0,4,tmp_buf);    
			}
			else 
				delay_us(100);	   
			t++;
			if(t==10000)//大约1s钟改变一次状态
			{
				t=0;
				LED0=!LED0;
			} 				    
		};	
	}
	else//TX模式
	{							    
		WriteEn(0,0,(uint8_t*)"NRF24L01 TX_Mode");	
		NRF24L01_TX_Mode();
		mode=0;//mode从0变化到51，表示大小写共52个英文字母 
		while(1)
		{	  		   				 
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				WriteEn(0,2,(uint8_t*)"Sended DATA:");	
				WriteEn(0,4,tmp_buf); 
				for(t=0;t<32;t++)
				{
				    key=(mode+t)%52;
					if(key>25)//小写字母
				    {
						key=key-26+'a';
					}
					else //大写字母
					{
						key=key+'A';
					}
					tmp_buf[t]=key;	
				}
				mode++; 
				if(mode>51)
					mode=0;  	  
				tmp_buf[32]=0;//加入结束符		   
			}
			else
			{										   			   
				WriteEn(0,2,(uint8_t*)"Send Failed"); 
			};
			LED0=!LED0;
			delay_ms(1000);				    
		};
	} 

	
}
 
