#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


#ifdef TaoBao_STM32_Board_2014  //如果是淘宝2014年买的那块板

	#define LED0 PDout(13)// PB5
	#define LED1 PGout(14)// PE5	

#else 

	#define LED0 PAout(3)// PB5
	#define LED1 PAout(4)// PE5
  #define LED2 PAout(5)// PE5	

#endif

void LED_Init(void);//初始化

#endif
