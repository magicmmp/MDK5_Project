#ifndef __RED_H
#define __RED_H 
#include "sys.h"   

//�ʺ�ֱ������������ԭ�ӵ�stm32F103��Ӣ��
/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ����ʹ��TIM4��ͨ��4�����պ���ң�ص��źţ�����������PB9

#define            GENERAL_TIM                   TIM4
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM4
//120mS��ʱ�������
#define            GENERAL_TIM_PERIOD            12000
//��ʱ��ʱ�ӷ�Ƶ�󣬼�һ����Ҫ10uS
#define            GENERAL_TIM_PSC              (720-1)

// TIM ���벶��ͨ��GPIO��غ궨��
//�������ͷ����PB9
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CHx_PORT          GPIOB
#define            GENERAL_TIM_CHx_PIN           GPIO_Pin_9
#define            GENERAL_TIM_CHANNEL_x         TIM_Channel_4

// �ж���غ궨��
#define            GENERAL_TIM_IT_CCx            TIM_IT_CC4
#define            GENERAL_TIM_IRQ               TIM4_IRQn
#define            GENERAL_TIM_INT_FUN           TIM4_IRQHandler

// ��ȡ����Ĵ���ֵ�����궨��
#define            GENERAL_TIM_GetCapturex_FUN                 TIM_GetCapture4
// �����źż��Ժ����궨��
#define            GENERAL_TIM_OCxPolarityConfig_FUN           TIM_OC4PolarityConfig

// ������Ϣ�ṹ������
typedef struct
{   
	uint8_t    valid;        //�յ���Ч������Ϣ�����Ч�����ʺ�Ӧ�ֶ���Ϊ��Ч
  uint8_t    valid_tmp;      //��¼��ǰ�Ƿ�����Ч�Ķ̰��������ڴ�����
  uint8_t    flag_print;     //���ڴ�ӡ�������ݵı��
	uint8_t    isPressing;   // �������ڱ����£��յ�ͬ��ͷ��1���ɿ�120mS��Ϊ0
	uint8_t    nPulse;       //�ܹ��յ���������
	uint8_t    nPulse_tmp;      //�������������ֻΪÿ��4�����巢��һ�γ�������
	uint8_t    IR_code[4];     //�������롣4�ֽڣ��ɸ��ֽڵ����ֽڣ�ң������ַ+��ַ����+����+�����
 // uint32_t   IR_code_test;
  uint8_t    idx;    //ָʾ��ǰ�յ��ڼ��ֽ�
}Struct_IR_Info;



extern Struct_IR_Info IR_info ;


void      Remote_Init(void);    	//���⴫��������ͷ���ų�ʼ��
uint8_t   IR_Remote_to_key(void);       //ң��������ɨ��
void 	  Remote_Debug(void);
#endif















