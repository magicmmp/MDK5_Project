#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"
#include "Timer3.h"



/************************************************
 ALIENTEK精英STM32开发板实验4
 串口 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
//u16 ADC_data;//ADC值
//u8  ADC_v;   //ADC对应电压值
uint16_t LED_t;


int main(void)
{		
  u16 t;  
  u16 len;	
  u16 times=0;
  delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
  uart_init(115200);	 //串口初始化为115200
  LED_Init();			     //LED端口初始化
  KEY_Init();          //初始化与按键连接的硬件接口
  Adc_Init();
  TIM3_Int_Init();//配置T3中断周期是1ms
  while(1)
  {
    
    if(flag_T3_Update) //1ms定时中断
    {
      flag_T3_Update=0;
      LED_t++;
      times++;
      
      
    }
    
    
    if(USART_RX_STA&0x8000)
    {					   
      len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
      printf("\r\n您发送的消息为:\r\n\r\n");
      for(t=0;t<len;t++)
      {
        USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
      }
      
      printf("\r\n\r\n");//插入换行
      USART_RX_STA=0;
    }
    
      
      
    if(times%10000==0)
    {
      printf("\r\n精英STM32开发板 串口实验\r\n");
      printf("正点原子@ALIENTEK\r\n\r\n");
      printf("请输入数据,以回车键结束\n");  
    }
    
    if(LED_t>500)
    {
      LED_t=0;
      LED1=!LED1;
      LED0=!LED1;//闪烁LED,提示系统正在运行.
    }
   
    if(times%2000==0)
    {
      ADC_data=Get_Adc_Average(ADC_Channel_1,10);
      ADC_v=(float)ADC_data*(3.3/4096);
      printf("ADC值=%u, 电压=%f\r\n\r\n",ADC_data,ADC_v);
    }
  }	 
}

