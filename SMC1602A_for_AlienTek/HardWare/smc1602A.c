#include "smc1602A.h"


//���LCD1602�ٶȽ���
//ִ���κ�ָ��ʱ��һЩ�ȴ�ʱ��ز�����
void Delay(int ms)
{
	int i;
	for(;ms>0;ms--)
		for(i=0;i<10000;i++);
}


//��һ������GPIOC��GPIOG Ϊ���ģʽ������дָ�������		    
//LED IO��ʼ��
void GPIO_Init_1602(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PC,PG�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD; 		 //��©���
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 //IO���ٶ�Ϊ10MHz
	 GPIO_Init(GPIOG, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOG
					
	 GPIO_InitStructure.GPIO_Pin=(uint16_t)0x00ff;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	 					
}



void Set_GPIOC_Input(void)
{ 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 GPIO_InitStructure.GPIO_Pin=(uint16_t)0x00ff;				 
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; //��������ģʽ
					 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 						
}

void Set_GPIOC_Output(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 GPIO_InitStructure.GPIO_Pin=(uint16_t)0x00ff;				 
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		 //IO���ٶ�Ϊ10MHz 
					 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz						
}





 //дָ��
void writeCmd(uint8_t cmd)
{
	uint16_t tmp;
	RS=0;
	RW=0;
	EN=0;
	tmp=GPIO_ReadOutputData(GPIOC);
	tmp=(tmp & 0xff00) | cmd;
	Set_GPIOC_Output();
	GPIO_Write(GPIOC,tmp);
	Delay(2);
	EN=1;
	Delay(5);
	EN=0;
	Delay(5);
}

//��RAM���ض�λ��дһ���ֽ�����
//���λ������һ��ָ������
void writeData(uint8_t data)
{
	uint16_t tmp;
	RS=1;
	RW=0;
	EN=0;
	tmp=GPIO_ReadOutputData(GPIOC);
	tmp=(tmp & 0xff00) | data;
	Set_GPIOC_Output();
	GPIO_Write(GPIOC,tmp);
	Delay(2);
	EN=1;
	Delay(5);
	EN=0;
	Delay(5);
}

//��RAM���ض�λ�ö�һ���ֽ�����
//���λ������һ��ָ������
uint8_t readByte(void)
{
	uint16_t tmp;
	RS=1;
	RW=1;
	Delay(2);
	EN=1;
	//tmp=(tmp & 0xff00) | data;
	Set_GPIOC_Input();
	Delay(3);
	tmp=GPIO_ReadInputData(GPIOC);	
	EN=0;
	Delay(5);
	return tmp&0xff;
}


void initial_1602(void)
{ 
	writeCmd(0x01);
	writeCmd(0x38);
	writeCmd(0x06);
	writeCmd(0x0c);	 
}

void showOn_1602(RowNum p,uint8_t *s)
{ 
	uint8_t  i;
	
	//����дRAM����ʼ��ַ
	if(p==ROW_1)
		writeCmd(0x80);
	else
		writeCmd(0xc0);//0x80 + 0xC0

	for(i=0;i<16;i++)
	{	
		writeData(s[i]);//������Ϊָ���Զ�����
	}	
}

/**
*   ��RAM�е�p�е�column�е�λ��дһ���ֽ�
*/
void writeByteToRAM_1602(RowNum p,uint8_t columNum,uint8_t data)
{ 
	columNum=columNum%40; //RAM��ַ��ÿ��40���ֽ�
	
	if(p==ROW_1)
		writeCmd(0x80+columNum);
	else
		writeCmd(0xc0+columNum);
	
	writeData(data);		
}

/**
*   ��RAM��ָ���С��кŴ���һ���ֽ�
*/
uint8_t readByteFromRAM_1602(RowNum p,uint8_t columNum)
{ 
	columNum=columNum%40; //RAM��ַ��ÿ��40���ֽ�
	
	if(p==ROW_1)
		writeCmd(0x80+columNum);
	else
		writeCmd(0xc0+columNum);
	
	return readByte();		
}

  












