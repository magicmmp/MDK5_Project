#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

#define  _USE_SPI2



#ifdef _USE_SPI2  

	//����ԭ�ӿ������Flash����SPI2
	/*SPI�ӿڶ���-��ͷ****************************/
	#define      SPIx                        	   SPI2
	#define      SPI_APBxClock_FUN            	   RCC_APB1PeriphClockCmd
	//��Ҫʹ�ܵ�SPIx��ʱ��
	#define      RCC_APBxPeriph_SPIx               RCC_APB1Periph_SPI2
	//ʹ��GPIOB�˿ڵ�ʱ��
	#define      RCC_APB2Periph_GPIOx              RCC_APB2Periph_GPIOB   
	#define      SPI_PORT                 		   GPIOB
	
	
	//CS(NSS)���� Ƭѡѡ��ͨGPIO����
	#define      SPI_CS_PIN                        GPIO_Pin_12

	//SCK����
	#define      SPI_SCK_PIN                 		GPIO_Pin_13
	//MISO����
	#define      SPI_MISO_PIN                		GPIO_Pin_14
	//MOSI����
	#define      SPI_MOSI_PIN                		GPIO_Pin_15

	/*SPI�ӿڶ���-��β****************************/
#else 

	/*SPI�ӿڶ���-��ͷ****************************/
	#define      SPIx                        	    SPI1
	#define      SPI_APBxClock_FUN            	    RCC_APB2PeriphClockCmd
	//��Ҫʹ�ܵ�SPIx��ʱ��
	#define      RCC_APBxPeriph_SPIx                RCC_APB2Periph_SPI1
	//ʹ��GPIOB�˿ڵ�ʱ��
	#define      RCC_APB2Periph_GPIOx               RCC_APB2Periph_GPIOA  
	#define      SPI_PORT                 		    GPIOA
	
	
	//CS(NSS)���� Ƭѡѡ��ͨGPIO����
	#define      SPI_CS_PIN                         GPIO_Pin_4

	//SCK����
	#define      SPI_SCK_PIN                 		GPIO_Pin_5
	//MISO����
	#define      SPI_MISO_PIN                		GPIO_Pin_6
	//MOSI����
	#define      SPI_MOSI_PIN                		GPIO_Pin_7

	/*SPI�ӿڶ���-��β****************************/

#endif



 
 				  	    													  
void SPIx_Init(void);			 //��ʼ��SPI��
void SPIx_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPIx_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

