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

	 u8 ch_1,  ch_2,tmp_ch;
	 u8 is_busy_1,is_busy_2;
	 u8 rx_flag1,rx_flag2;
	 u16 t=0;			 
	 u8 tmp_buf[5];	
	 u8 rx_buff1[33];
	 u8 rx_buff2[33];
	delay_init();
	LED_Init();
	
	
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

	
	    WriteEn(0,0,(uint8_t*)"check idle CH:");	
		
		NRF24L01_RX_Mode();
		ch_1=0;
		ch_2=1;
		while(1)
		{	
			NRF24L01_CE=0;	  
			NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,ch_1);	     //����RFͨ��Ƶ��		  
			NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
			delay_ms(10);
			
			if(ch_1==2)
			{
				rx_flag1=1;
				while(rx_flag1)
					rx_flag1=NRF24L01_RxPacket(rx_buff1);
			}	
			else
				rx_flag1=NRF24L01_RxPacket(rx_buff1);
			is_busy_1=NRF24L01_Read_Reg(CD);
			
			NRF24L01_CE=0;	  
			NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,ch_2);	     //����RFͨ��Ƶ��		  
			NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
			delay_ms(10);
			rx_flag2=NRF24L01_RxPacket(rx_buff2);
			is_busy_2=NRF24L01_Read_Reg(CD);
			
			tmp_ch=ch_1;
			tmp_buf[2]=tmp_ch%10+'0';
			tmp_ch=tmp_ch/10;
			tmp_buf[1]=tmp_ch%10+'0';
			tmp_ch=tmp_ch/10;
			tmp_buf[0]=tmp_ch%10+'0';
			tmp_buf[3]=0;
			WriteEn(0,2,(uint8_t*)"ch");
			WriteEn(3*8,2,tmp_buf);
			if(is_busy_1)
				WriteEn(8*8,2,(uint8_t*)"busy");
		    else
				WriteEn(8*8,2,(uint8_t*)"idle");
			
			
			tmp_ch=ch_2;
			tmp_buf[2]=tmp_ch%10+'0';
			tmp_ch=tmp_ch/10;
			tmp_buf[1]=tmp_ch%10+'0';
			tmp_ch=tmp_ch/10;
			tmp_buf[0]=tmp_ch%10+'0';
			tmp_buf[3]=0;
			WriteEn(0,4,(uint8_t*)"ch");
			WriteEn(3*8,4,tmp_buf);
			if(is_busy_2)
				WriteEn(8*8,4,(uint8_t*)"busy");
		    else
				WriteEn(8*8,4,(uint8_t*)"idle");
			
			if(rx_flag1==0)
			{
				WriteEn(0,6,(uint8_t*)"ch1:");
				rx_buff1[0]='O';
				rx_buff1[1]='K';
				rx_buff1[3]=0;
				WriteEn(4*8,6,rx_buff1);
			}
			else
			{
				WriteEn(0,6,(uint8_t*)"ch1:");
				WriteEn(4*8,6,(uint8_t*)"xx");
			}
			
			if(rx_flag2==0)
			{
				WriteEn(8*8,6,(uint8_t*)"ch2:");
				rx_buff2[3]=0;
				WriteEn(12*8,6,rx_buff2);
			}
			else
			{
				WriteEn(8*8,6,(uint8_t*)"ch2:");
				WriteEn(12*8,6,(uint8_t*)"xx");
			}
				
			for(t=0;t<33;t++)
			{
				rx_buff1[t]=0;
				rx_buff2[t]=0;
			}
			
			ch_1=(ch_1+2)%128;
			ch_2=(ch_2+2)%128;
						
			LED0=!LED0;
			delay_ms(1000);
			delay_ms(1000);
						    
		};	
	

	
}
 
