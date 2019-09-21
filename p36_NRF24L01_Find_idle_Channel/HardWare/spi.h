#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define  _USE_SPI2



#ifdef _USE_SPI2  

	//正点原子开发板的Flash连在SPI2
	/*SPI接口定义-开头****************************/
	#define      SPIx                        	   SPI2
	#define      SPI_APBxClock_FUN            	   RCC_APB1PeriphClockCmd
	//需要使能的SPIx的时钟
	#define      RCC_APBxPeriph_SPIx               RCC_APB1Periph_SPI2
	//使能GPIOB端口的时钟
	#define      RCC_APB2Periph_GPIOx              RCC_APB2Periph_GPIOB   
	#define      SPI_PORT                 		   GPIOB
	
	
	//CS(NSS)引脚 片选选普通GPIO即可
	#define      SPI_CS_PIN                        GPIO_Pin_12

	//SCK引脚
	#define      SPI_SCK_PIN                 		GPIO_Pin_13
	//MISO引脚
	#define      SPI_MISO_PIN                		GPIO_Pin_14
	//MOSI引脚
	#define      SPI_MOSI_PIN                		GPIO_Pin_15

	/*SPI接口定义-结尾****************************/
#else 

	/*SPI接口定义-开头****************************/
	#define      SPIx                        	    SPI1
	#define      SPI_APBxClock_FUN            	    RCC_APB2PeriphClockCmd
	//需要使能的SPIx的时钟
	#define      RCC_APBxPeriph_SPIx                RCC_APB2Periph_SPI1
	//使能GPIOB端口的时钟
	#define      RCC_APB2Periph_GPIOx               RCC_APB2Periph_GPIOA  
	#define      SPI_PORT                 		    GPIOA
	
	
	//CS(NSS)引脚 片选选普通GPIO即可
	#define      SPI_CS_PIN                         GPIO_Pin_4

	//SCK引脚
	#define      SPI_SCK_PIN                 		GPIO_Pin_5
	//MISO引脚
	#define      SPI_MISO_PIN                		GPIO_Pin_6
	//MOSI引脚
	#define      SPI_MOSI_PIN                		GPIO_Pin_7

	/*SPI接口定义-结尾****************************/

#endif



 
 				  	    													  
void SPIx_Init(void);			 //初始化SPI口
void SPIx_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPIx_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
		 
#endif

