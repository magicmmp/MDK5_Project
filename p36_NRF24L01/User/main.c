#include "stm32f10x.h"
#include "bsp_usart.h"
#include "spi.h"
#include "key.h"
#include "24l01.h"
#include "pg12864.h"
#include "led.h"
#include "delay.h"



 
 
 
 /**
 *  1��
 *  2��ͬʱ���ڿɽ��յ��Դ������ַ�����һ��Ӣ���ַ�����ʾ��mini12864�ϡ�
 */
 

int main(void)
{  	

	 u8 key,mode;
	 u16 t=0;			 
	 u8 tmp_buf[33];	
	delay_init();
	LED_Init();

#ifdef TaoBao_STM32_Board_2014  //������Ա�2014������ǿ��
	for(t=0;t<10;t++)
	{
		LED0=!LED0;
		LED1=!LED1;
 		delay_ms(1000);
	}
#endif
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	 
	 KEY_Init();					//��ʼ������
	 NRF24L01_Init();    		//��ʼ��NRF24L01 
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	/* ��ʼ��USART */
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
			WriteEn(0,0,(uint8_t*)"KEY0: RX_Mode"); //��˸��ʾ��ʾ��Ϣ
		    WriteEn(0,2,(uint8_t*)"KEY1: TX_Mode"); //��˸��ʾ��ʾ��Ϣ	
			LED0=!LED0;
			LED1=!LED1;
		}
 		if(t==200)
		{	
			t=0; 
		}
		delay_ms(5);	  
	}   
	
#ifdef TaoBao_STM32_Board_2014  //������Ա�2014������ǿ��
	mode=1;
#endif
	
	if(mode==0)//RXģʽ
	{
		WriteEn(0,0,(uint8_t*)"NRF24L01 RX_Mode");	
		WriteEn(0,2,(uint8_t*)"Received DATA:");	
		NRF24L01_RX_Mode();		  
		while(1)
		{	  		    		    				 
			if(NRF24L01_RxPacket(tmp_buf)==0)//һ�����յ���Ϣ,����ʾ����.
			{
				tmp_buf[32]=0;//�����ַ���������
				WriteEn(0,4,tmp_buf);    
			}
			else 
				delay_us(100);	   
			t++;
			if(t==10000)//��Լ1s�Ӹı�һ��״̬
			{
				t=0;
				LED0=!LED0;
			} 				    
		};	
	}
	else//TXģʽ
	{							    
		WriteEn(0,0,(uint8_t*)"NRF24L01 TX_Mode");	
		NRF24L01_TX_Mode();
		mode=0;//mode��0�仯��51����ʾ��Сд��52��Ӣ����ĸ 
		while(1)
		{	  		   				 
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				WriteEn(0,2,(uint8_t*)"Sended DATA:");	
				WriteEn(0,4,tmp_buf); 
				for(t=0;t<32;t++)
				{
				    key=(mode+t)%52;
					if(key>25)//Сд��ĸ
				    {
						key=key-26+'a';
					}
					else //��д��ĸ
					{
						key=key+'A';
					}
					tmp_buf[t]=key;	
				}
				mode++; 
				if(mode>51)
					mode=0;  	  
				tmp_buf[32]=0;//���������		   
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
 
