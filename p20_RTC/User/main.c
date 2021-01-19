	
#include "stm32f10x.h"
#include "usart.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "bsp_rtc.h"
#include "pg12864.h"
#include "remote.h"


u8 status;  //表示处于哪个运行状态
u8 preKey;

const u8  Status_Normal  = 0;
const u8  Status_SetYear = 1;
const u8  Status_SetMonth= 2;
const u8  Status_SetDay  = 3;
const u8  Status_SetHour = 4;
const u8  Status_SetMin  = 5;
const u8  Status_SetSec  = 6;

const char StatusBuffer[7][10]={"电子日历","设置年","设置月","设置日","设置时","设置分","设置秒"};


/**
const u8 KEY_0=0xB2;
const u8 KEY_1=0x42;
const u8 KEY_2=0x82;
const u8 KEY_3=0x02;
const u8 KEY_4=0x62;
const u8 KEY_5=0xA2;
const u8 KEY_6=0x22;
const u8 KEY_7=0x52;
const u8 KEY_8=0x92;
const u8 KEY_9=0x12;
const u8 KEY_up=0x28;
const u8 KEY_down=0x68;
const u8 KEY_left=0xB8;
const u8 KEY_right=0x88;
const u8 KEY_OK=0xA8;
const u8 KEY_EXIT=0x38;
const u8 KEY_menu=0xD2;//进入时间设置状态
*/

const u8 KEY_0=0x08;
const u8 KEY_1=0x88;
const u8 KEY_2=0x48;
const u8 KEY_3=0xc8;
const u8 KEY_4=0x28;
const u8 KEY_5=0xa8;
const u8 KEY_6=0x68;
const u8 KEY_7=0xe8;
const u8 KEY_8=0x18;
const u8 KEY_9=0x98;
const u8 KEY_up=0xa0;
const u8 KEY_down=0x60;
const u8 KEY_left=0xe0;
const u8 KEY_right=0x10;
const u8 KEY_OK=0x20;
const u8 KEY_EXIT=0xc0;
const u8 KEY_menu=0x40;//进入时间设置状态







//一个月最多几天
const u8 MaxDay[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

// N = 2^32/365/24/60/60 = 136 年

/*时间结构体，默认时间2000-01-01 00:00:00*/
struct rtc_time systmtime=
{ 
0,30,8,7,10,2019,1
};

extern char const *WEEK_STR[];
extern __IO uint32_t TimeDisplay ;

/**
  * @brief  RTC中断函数.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	  {
	    /* Clear the RTC Second interrupt */
	    RTC_ClearITPendingBit(RTC_IT_SEC);
	
	    /* 表示main函数可以访问时间了*/
	    TimeDisplay = 1;
	
	    /* Wait until last write operation on RTC registers has finished */
	    RTC_WaitForLastTask();
	  }
}




void changSetStatus(u8 key)
{
	if(key==KEY_left)
	{
		if(status>1)
			status--;
		else if(status==1)
			status=6;
	}
	else if(key==KEY_right)
	{
		if(status<6)
			status++;
		else if(status==6)
			status=1;
	}
}

void DisplayTimeOn_12864(struct rtc_time *tm,u8* buffer,u8 key)
{
	//sprintf((char*)buffer, "2019年06月08日");
	//sprintf((char*)buffer, "23:12:30");
	sprintf((char*)buffer, "%02X",key);
	WriteEn(status,0,0,(u8*)buffer);
	WriteCn(status,16,0,(u8*)StatusBuffer[status]);
	if(status)
	{
		sprintf((char*)buffer, "  ");
		WriteEn(status,64,0,(u8*)buffer);
	}
		
	
	 sprintf((char*)buffer, "%4d",tm->tm_year);
	WriteEn(status,8,2,(u8*)buffer);
	WriteCn(status,8+8*4,2,(u8*)"年");
	sprintf((char*)buffer, "%2d",tm->tm_mon);
	WriteEn(status,8+8*6,2,(u8*)buffer);
	WriteCn(status,8+8*8,2,(u8*)"月");
	sprintf((char*)buffer, "%2d",tm->tm_mday);
	WriteEn(status,8+8*10,2,(u8*)buffer);
	WriteCn(status,8+8*12,2,(u8*)"日");
	
	/*
	 * Determine the day of week
	 */
	GregorianDay(tm);
	sprintf((char*)buffer, "星期%s",WEEK_STR[tm->tm_wday]);
	WriteCn(status,80,4,(u8*)buffer);
	
	sprintf((char*)buffer, "%2d:%02d:%02d",tm->tm_hour,tm->tm_min,tm->tm_sec);
	WriteEn(status,0,6,(u8*)buffer);	
}

//如果按下了某个键，则调用该函数
void SetTime(u8 key,struct rtc_time *tm)
{ 
	int maxDayOfMonth;
	switch(status)
	{	
		case Status_Normal:
			if(key==KEY_menu)
				status=Status_SetYear;
			break;
		case Status_SetYear:
			if(key==KEY_up)
				tm->tm_year++;
			if(key==KEY_down)
				tm->tm_year--;
			if(tm->tm_year>2038)
				tm->tm_year=1970;
			if(tm->tm_year<1970)
				tm->tm_year=2038;
			
			if(key==KEY_left || key==KEY_right)
				changSetStatus(key);
			
		    if(key==KEY_OK)
			{
				Time_Adjust(tm);
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				status=Status_Normal;
			}
			if(key==KEY_EXIT)
			{
				status=Status_Normal;
			}
				
			break;			
		case Status_SetMonth:
			if(key==KEY_up)
				tm->tm_mon++;
			if(key==KEY_down)
				tm->tm_mon--;
			if(tm->tm_mon>12)
				tm->tm_mon=1;
			if(tm->tm_mon<1)
				tm->tm_mon=12;
			
			if(key==KEY_left || key==KEY_right)
				changSetStatus(key);

			if(key==KEY_OK)
			{
				Time_Adjust(tm);
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				status=Status_Normal;
			}
			if(key==KEY_EXIT)
			{
				status=Status_Normal;
			}
			break;
		
		case Status_SetDay:
			maxDayOfMonth=MaxDay[tm->tm_mon];
		    if((tm->tm_year%4==0) &&((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
							 (tm->tm_mon==2))
			{
				maxDayOfMonth++;
			}
			if(key==KEY_up)
					tm->tm_mday++;
			if(key==KEY_down)
				tm->tm_mday--;
			if(tm->tm_mday>maxDayOfMonth)
				tm->tm_mday=1;
			if(tm->tm_mday<1)
				tm->tm_mday=maxDayOfMonth;
			
			if(key==KEY_left || key==KEY_right)
				changSetStatus(key);

			if(key==KEY_OK)
			{
				Time_Adjust(tm);
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				status=Status_Normal;
			}
			if(key==KEY_EXIT)
			{
				status=Status_Normal;
			}
			break;
		
		case Status_SetHour:
			if(key==KEY_up)
				tm->tm_hour++;
			if(key==KEY_down)
				tm->tm_hour--;
			if(tm->tm_hour>23)
				tm->tm_hour=0;
			if(tm->tm_hour<0)
				tm->tm_hour=23;
			
			if(key==KEY_left || key==KEY_right)
				changSetStatus(key);

			if(key==KEY_OK)
			{
				Time_Adjust(tm);
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				status=Status_Normal;
			}
			if(key==KEY_EXIT)
			{
				status=Status_Normal;
			}
			break;
		
		case Status_SetMin:
			if(key==KEY_up)
				tm->tm_min++;
			if(key==KEY_down)
				tm->tm_min--;
			if(tm->tm_min>59)
				tm->tm_min=0;
			if(tm->tm_min<0)
				tm->tm_min=59;
			
			if(key==KEY_left || key==KEY_right)
				changSetStatus(key);

			if(key==KEY_OK)
			{
				Time_Adjust(tm);
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				status=Status_Normal;
			}
			if(key==KEY_EXIT)
			{
				status=Status_Normal;
			}
			break;
		
		case Status_SetSec:
			if(key==KEY_up)
				tm->tm_sec++;
			if(key==KEY_down)
				tm->tm_sec--;
			if(tm->tm_sec>59)
				tm->tm_sec=0;
			if(tm->tm_sec<0)
				tm->tm_sec=59;
			
			if(key==KEY_left || key==KEY_right)
				changSetStatus(key);

			if(key==KEY_OK)
			{
				Time_Adjust(tm);
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				status=Status_Normal;
			}
			if(key==KEY_EXIT)
			{
				status=Status_Normal;
			}
			break;
	}
	
}




int main()
{		

	u8 t;
	u8 key;
	
	uint32_t BJ_TimeVar;
	u8 buffer[17];//显示一行的内容
	
	
	//SCB->VTOR = FLASH_BASE | 0x10000; /* Vector Table Relocation in Internal FLASH. */
	
	status=Status_Normal;
	
	
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	 if(SysTick_Config(SystemCoreClock/1000))	 	/* 1ms */
	 { 
		/* Capture error */ 
		 while (1);
	 }
	 LED_Init();
	
	//delay_init();	    	 //延时函数初始化	  
	
	uart_init(115200);	 //串口初始化为115200
	KEY_Init(); 
	Remote_Init();	//
	
	 GPIO_Init_12864();
	 Init_12864();
	 ClearScreen();
	
	 
	
	
	
	
	 

		/* 配置RTC秒中断优先级 */
	  RTC_NVIC_Config();
	  RTC_CheckAndConfig(&systmtime);
	
	  while (1)
	  {
          
		if(led_time_count_tmp==0)
		{
			led_time_count_tmp=led_time_count;
			LED0=!LED0;
            LED1=!LED0;
        }
		  
			key=Remote_Scan();
			if(key) //此时有按键按下
			{
				SetTime(key,&systmtime);
				preKey=key;
			}
			else
			{
				if(status==Status_Normal)
				{
					/* 每过1s 更新一次时间*/
					if (TimeDisplay == 1)
					{
							/* 当前时间 */
						BJ_TimeVar=RTC_GetCounter()+TIME_ZOOM;
						to_tm(BJ_TimeVar, &systmtime);/*把定时器的值转换为北京时间*/			  
						TimeDisplay = 0;
					}
					
				}
	
				DisplayTimeOn_12864(&systmtime,buffer,preKey);
			}		  
	  }
}

/***********************************END OF FILE*********************************/

