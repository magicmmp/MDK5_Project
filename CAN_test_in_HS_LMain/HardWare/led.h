#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


#ifdef TaoBao_STM32_Board_2014  //������Ա�2014������ǿ��

	#define LED0 PDout(13)// PB5
	#define LED1 PGout(14)// PE5	

#else 

	#define LED0 PAout(3)// PB5
	#define LED1 PAout(4)// PE5
  #define LED2 PAout(5)// PE5	

#endif

void LED_Init(void);//��ʼ��

#endif
