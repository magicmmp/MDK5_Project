#ifndef __FONT_H
#define __FONT_H 	 
#include "stm32f10x.h"
//����ASCII��
//ƫ����32 
//ASCII�ַ���: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
//PC2LCD2002ȡģ��ʽ���ã�����+����ʽ+˳��+C51��ʽ
//�ܹ���3���ַ�����12*12��16*16��24*24�����û������������������ֱ��ʵ��ַ�����
//ÿ���ַ���ռ�õ��ֽ���Ϊ:(size/8+((size%8)?1:0))*(size/2),����size:���ֿ�����ʱ�ĵ����С(12/16/24...)

//8*16 ASCII�ַ�������
extern const unsigned char asc2_1608[95][16];  


//������ģ�ṹ�嶨��
struct CnCharTypeDef               
{   
	u8   Index[2];          	
	u8   Msk[32]; 
};
extern const u16 CnCharSize;//������ģ����
extern const  struct CnCharTypeDef CnChar_1616[];

#endif
