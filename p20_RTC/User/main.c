/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   rtc 测试，显示时间格式为: xx:xx:xx
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
	
#include "stm32f10x.h"
#include "usart.h"
#include "key.h"
#include "delay.h"
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







//一个月最多几天
const u8 MaxDay[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

// N = 2^32/365/24/60/60 = 136 年

/*时间结构体，默认时间2000-01-01 00:00:00*/
struct rtc_time systmtime=
{ 
0,30,8,7,10,2019,1
};

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


//【*】注意事项：
//在bsp_rtc.h文件中：

//1.可设置宏USE_LCD_DISPLAY控制是否使用LCD显示
//2.可设置宏RTC_CLOCK_SOURCE_LSI和RTC_CLOCK_SOURCE_LSE控制使用LSE晶振还是LSI晶振

//3.STM32的LSE晶振要求非常严格，同样的电路、板子批量产品时总有些会出现问题。
//  本实验中默认使用LSI晶振。
//  
//4.！！！若希望RTC在主电源掉电后仍然运行，需要给开发板的电池槽安装钮扣电池，
//  ！！！且改成使用外部晶振模式RTC_CLOCK_SOURCE_LSE
//  钮扣电池型号：CR1220
/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */




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
	sprintf((char*)buffer, "kn=%d,key=0x%02X",status,key);
	WriteEn(status,0,0,(u8*)buffer);
	
	sprintf((char*)buffer, "%4d",tm->tm_year);
	WriteEn(status,8,2,(u8*)buffer);
	WriteCn(status,8+8*4,2,(u8*)"年");
	sprintf((char*)buffer, "%2d",tm->tm_mon);
	WriteEn(status,8+8*6,2,(u8*)buffer);
	WriteCn(status,8+8*8,2,(u8*)"月");
	sprintf((char*)buffer, "%2d",tm->tm_mday);
	WriteEn(status,8+8*10,2,(u8*)buffer);
	WriteCn(status,8+8*12,2,(u8*)"日");
	//16,16+5*8,16+10*8,
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



//正点原子开发板的  RTC使用外部晶振  不能工作？

int main()
{		

	u8 t;
	u8 key;
	
	uint32_t BJ_TimeVar;
	u8 buffer[17];//显示一行的内容
	
	status=Status_Normal;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	
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

