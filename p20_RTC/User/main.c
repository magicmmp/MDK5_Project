/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   rtc ���ԣ���ʾʱ���ʽΪ: xx:xx:xx
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
	
#include "stm32f10x.h"
#include "usart.h"
#include "key.h"
#include "delay.h"
#include "sys.h"
#include "bsp_rtc.h"


// N = 2^32/365/24/60/60 = 136 ��

/*ʱ��ṹ�壬Ĭ��ʱ��2000-01-01 00:00:00*/
struct rtc_time systmtime=
{
0,0,0,1,1,2000,0
};

extern __IO uint32_t TimeDisplay ;

/**
  * @brief  RTC�жϺ���.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	  {
	    /* Clear the RTC Second interrupt */
	    RTC_ClearITPendingBit(RTC_IT_SEC);
	
	    /* ��ʾmain�������Է���ʱ����*/
	    TimeDisplay = 1;
	
	    /* Wait until last write operation on RTC registers has finished */
	    RTC_WaitForLastTask();
	  }
}


//��*��ע�����
//��bsp_rtc.h�ļ��У�

//1.�����ú�USE_LCD_DISPLAY�����Ƿ�ʹ��LCD��ʾ
//2.�����ú�RTC_CLOCK_SOURCE_LSI��RTC_CLOCK_SOURCE_LSE����ʹ��LSE������LSI����

//3.STM32��LSE����Ҫ��ǳ��ϸ�ͬ���ĵ�·������������Ʒʱ����Щ��������⡣
//  ��ʵ����Ĭ��ʹ��LSI����
//  
//4.��������ϣ��RTC������Դ�������Ȼ���У���Ҫ��������ĵ�ز۰�װť�۵�أ�
//  �������Ҹĳ�ʹ���ⲿ����ģʽRTC_CLOCK_SOURCE_LSE
//  ť�۵���ͺţ�CR1220
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */


//����ԭ�ӿ������  RTCʹ���ⲿ����  ���ܹ�����

int main()
{		

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	KEY_Init(); 
		
	
	 

		/* ����RTC���ж����ȼ� */
	  RTC_NVIC_Config();
	  RTC_CheckAndConfig(&systmtime);
	
	  while (1)
	  {
	    /* ÿ��1s ����һ��ʱ��*/
	    if (TimeDisplay == 1)
	    {
				/* ��ǰʱ�� */
	      Time_Display( RTC_GetCounter(),&systmtime); 		  
	      TimeDisplay = 0;
	    }
			
			//���°���KEY0��ͨ�������޸�ʱ��
			if( KEY_Scan(0) == KEY0_PRES  )
			{
				struct rtc_time set_time;

				/*ʹ�ô��ڽ������õ�ʱ�䣬��������ʱע��ĩβҪ�ӻس�*/
				Time_Regulate_Get(&set_time);
				/*�ý��յ���ʱ������RTC*/
				Time_Adjust(&set_time);
				
				//�򱸷ݼĴ���д���־
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
			} 			
	  }
}

/***********************************END OF FILE*********************************/

