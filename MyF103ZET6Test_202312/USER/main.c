#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "Timer3.h"



/************************************************
 ALIENTEK��ӢSTM32������ʵ��4
 ���� ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
//u16 ADC_data;//ADCֵ
//u8  ADC_v;   //ADC��Ӧ��ѹֵ
uint16_t LED_t;


int main(void)
{		
  u16 t;  
  u16 len;	
  u16 times=0;
  delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
  LED_Init();			     //LED�˿ڳ�ʼ��
  KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
  Adc_Init();
  TIM3_Int_Init();//����T3�ж�������1ms
  while(1)
  {
    
    if(flag_T3_Update) //1ms��ʱ�ж�
    {
      flag_T3_Update=0;
      LED_t++;
      times++;
      
      
    }
    
    
    if(USART_RX_STA&0x8000)
    {					   
      len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
      printf("\r\n�����͵���ϢΪ:\r\n\r\n");
      for(t=0;t<len;t++)
      {
        USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
      }
      
      printf("\r\n\r\n");//���뻻��
      USART_RX_STA=0;
    }
    
      
      
    if(times%10000==0)
    {
      printf("\r\n��ӢSTM32������ ����ʵ��\r\n");
      printf("����ԭ��@ALIENTEK\r\n\r\n");
      printf("����������,�Իس�������\n");  
    }
    
    if(LED_t>500)
    {
      LED_t=0;
      LED1=!LED1;
      LED0=!LED1;//��˸LED,��ʾϵͳ��������.
    }
   
    if(times%2000==0)
    {
      ADC_data=Get_Adc_Average(ADC_Channel_1,10);
      ADC_v=(float)ADC_data*(3.3/4096);
      printf("ADCֵ=%u, ��ѹ=%f\r\n\r\n",ADC_data,ADC_v);
    }
  }	 
}

