#include "pg12864.h"
#include "sys.h"

//һ��һ����ʾһ��
//��ʾ��״̬kn�µĸ�����ʾ���ַ�λ�úͿ��ȣ�������Ӣ����ĸ��Ϊ���㵥λ
//��{17,4}��ʾ��Ļ�ϵ�17��Ӣ����ĸ���4����ĸ���ȣ�����2�еĵ�2����ĸ��λ��
const u8 Hilight[7][2]={{0,0},{17,4},{23,2},{27,2},{48,2},{51,2},{54,2}};	

//��ģ��ÿ���ֽ����ݱ�ʾһ�����ߵ�8���㡣�ֽڸ�λ��ʾ�ĵ������档
  const  struct Enchar Zifu[]={
'=',0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,
'_',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
44, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x78,0x70,0x00,0x00,0x00,//','
'-',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
':',0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,
' ',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
'0',0x00,0x0f,0x10,0x20,0x20,0x20,0x10,0x0f,0x00,0xf0,0x08,0x04,0x04,0x04,0x08,0xf0,
'1',0x00,0x00,0x00,0x10,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,
'2',0x00,0x0C,0x10,0x20,0x20,0x20,0x11,0x0E,0x00,0x1C,0x24,0x44,0x84,0x84,0x04,0x04,
'3',0x00,0x08,0x10,0x21,0x21,0x21,0x12,0x0C,0x00,0x10,0x08,0x04,0x04,0x04,0x88,0x70,
'4',0x00,0x00,0x00,0x03,0x0C,0x10,0x3F,0x00,0x00,0x20,0xE0,0x20,0x20,0x24,0xFC,0x24,
'5',0x00,0x3E,0x22,0x22,0x22,0x21,0x20,0x00,0x00,0x08,0x04,0x04,0x04,0x08,0xF0,0x00,
'6',0x00,0x0F,0x11,0x22,0x22,0x22,0x11,0x08,0x00,0xF0,0x08,0x04,0x04,0x04,0x08,0xF0,
'7',0x00,0x20,0x20,0x20,0x21,0x26,0x38,0x00,0x00,0x00,0x1c,0x60,0x80,0x00,0x00,0x00,
'8',0x00,0x0C,0x12,0x21,0x21,0x12,0x0C,0x00,0x00,0x70,0x88,0x04,0x04,0x88,0x70,0x00,
'9',0x00,0x0f,0x10,0x20,0x20,0x10,0x0f,0x00,0x00,0x00,0x84,0x44,0x44,0x88,0xf0,0x00,
'A',0x00,0x00,0x01,0x06,0x38,0x06,0x01,0x00,0x00,0x1c,0xe0,0x20,0x20,0x20,0xe0,0x1c,
'M',0x00,0x1F,0x0C,0x03,0x00,0x03,0x0C,0x1F,0x00,0xFC,0x00,0x00,0xC0,0x00,0x00,0xFC,
'B',0x10,0x1F,0x11,0x11,0x11,0x0E,0x00,0x00,0x04,0xFC,0x04,0x04,0x04,0x88,0x70,0x00,
'C',0x00,0x07,0x08,0x10,0x10,0x10,0x10,0x08,0x00,0xf0,0x08,0x04,0x04,0x04,0x04,0x08,
'D',0x10,0x1F,0x10,0x10,0x10,0x08,0x07,0x00,0x04,0xFC,0x04,0x04,0x04,0x08,0xF0,0x00,
'E',0x10,0x1f,0x10,0x10,0x10,0x11,0x08,0x00,0x04,0xfc,0x84,0x84,0x84,0xc4,0x04,0x08,
'F',0x00,0x10,0x1f,0x11,0x11,0x13,0x10,0x08,0x00,0x04,0xfc,0x04,0x00,0x80,0x00,0x00,
'G',0x03,0x04,0x08,0x08,0x08,0x0c,0x00,0x00,0xe0,0x18,0x04,0x04,0x04,0x24,0x38,0x20,
'H',0x10,0x1F,0x10,0x00,0x00,0x10,0x1F,0x10,0x04,0xFC,0x84,0x80,0x80,0x84,0xFC,0x04,
'I',0x00,0x10,0x10,0x1F,0x10,0x10,0x00,0x00,0x00,0x04,0x04,0xFC,0x04,0x04,0x00,0x00,
'J',0x00,0x00,0x10,0x10,0x1F,0x10,0x10,0x00,0x03,0x01,0x01,0x01,0xFE,0x00,0x00,0x00,
'K',0x10,0x1F,0x11,0x03,0x14,0x18,0x10,0x00,0x04,0xFC,0x04,0x80,0x64,0x1C,0x04,0x00,
'L',0x10,0x1F,0x10,0x00,0x00,0x00,0x00,0x00,0x04,0xFC,0x04,0x04,0x04,0x04,0x0C,0x00,
'N',0x10,0x1F,0x0C,0x03,0x00,0x10,0x1F,0x10,0x04,0xFC,0x04,0x00,0xE0,0x18,0xFC,0x00,
'O',0x07,0x08,0x10,0x10,0x10,0x10,0x08,0x07,0xf0,0x08,0x04,0x04,0x04,0x04,0x08,0xf0,
'P',0x10,0x1F,0x10,0x10,0x10,0x10,0x0F,0x00,0x02,0xFe,0x82,0x80,0x80,0x80,0x00,0x00,
'Q',0x07,0x08,0x10,0x10,0x10,0x08,0x07,0x00,0xF0,0x18,0x24,0x24,0x1C,0x0A,0xF2,0x00,
'R',0x10,0x1F,0x11,0x11,0x11,0x11,0x0E,0x00,0x04,0xFC,0x04,0x00,0xC0,0x30,0x0C,0x04,
'S',0x00,0x0e,0x11,0x10,0x10,0x10,0x08,0x00,0x00,0x08,0x04,0x84,0x84,0x44,0x38,0x00,
'T',0x18,0x10,0x10,0x1F,0x10,0x10,0x18,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00,
'U',0x10,0x1F,0x10,0x00,0x00,0x10,0x1F,0x10,0x00,0xF8,0x04,0x04,0x04,0x04,0xF8,0x00,
'V',0x10,0x1E,0x11,0x00,0x00,0x13,0x1C,0x10,0x00,0x00,0xE0,0x1C,0x70,0x80,0x00,0x00,
'W',0x1f,0x10,0x00,0x03,0x00,0x10,0x1f,0x00,0xc0,0x3c,0xe0,0x00,0xe0,0x3c,0xc0,0x00,
'X',0x10,0x18,0x16,0x01,0x01,0x16,0x18,0x10,0x04,0x0C,0x34,0xC0,0xC0,0x34,0x0C,0x04,
'Y',0x10,0x1C,0x13,0x00,0x13,0x1C,0x10,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00,
'Z',0x08,0x10,0x10,0x10,0x13,0x1C,0x10,0x00,0x04,0x1C,0x64,0x84,0x04,0x04,0x18,0x00,
'a',0x00,0x02,0x04,0x04,0x04,0x04,0x03,0x00,0x00,0x70,0x48,0x88,0x88,0x90,0xf8,0x00,
'b',0x00,0x3f,0x01,0x02,0x02,0x02,0x01,0x00,0x00,0xf8,0x10,0x08,0x08,0x08,0x10,0xe0,
'c',0x00,0x03,0x02,0x04,0x04,0x04,0x02,0x00,0x00,0xf0,0x08,0x08,0x08,0x08,0x10,0x00,
'd',0x00,0x01,0x02,0x02,0x02,0x01,0x3f,0x00,0xe0,0x10,0x08,0x08,0x08,0x10,0xf8,0x00,
'e',0x00,0x03,0x04,0x08,0x08,0x08,0x04,0x03,0x00,0xe0,0x90,0x88,0x88,0x88,0x88,0x90,
'f',0x00,0x01,0x01,0x1f,0x21,0x21,0x18,0x00,0x00,0x00,0x04,0xfc,0x04,0x00,0x00,0x00,
'g',0x00,0x07,0x08,0x08,0x08,0x09,0x0f,0x00,0x00,0x08,0x84,0x84,0x84,0x04,0xf8,0x00,
'h',0x20,0x3f,0x01,0x02,0x02,0x02,0x01,0x00,0x08,0xf8,0x08,0x00,0x00,0x08,0xf8,0x08,
'i',0x00,0x00,0x00,0x04,0x17,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xf8,0x08,0x00,0x00,
'j',0x00,0x00,0x00,0x04,0x17,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0xF8,0x00,0x00,0x00,  
'k',0x20,0x3F,0x00,0x00,0x02,0x03,0x02,0x00,0x04,0xFC,0x24,0x40,0xA0,0x14,0x0C,0x04,
'l',0x00,0x00,0x20,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0xFC,0x04,0x00,0x00,0x00,
'm',0x00,0x0f,0x04,0x08,0x07,0x08,0x08,0x07,0x08,0xf8,0x08,0x00,0xf8,0x00,0x08,0xf8,
'n',0x08,0x0f,0x04,0x08,0x08,0x08,0x07,0x00,0x08,0xf8,0x08,0x00,0x00,0x08,0xf8,0x08,
'o',0x00,0x01,0x02,0x04,0x04,0x02,0x01,0x00,0x00,0xe0,0x10,0x08,0x08,0x10,0xe0,0x00,
'p',0x00,0x0f,0x04,0x08,0x08,0x08,0x04,0x03,0x02,0xfe,0x82,0x40,0x40,0x40,0x80,0x00,
'q',0x03,0x04,0x08,0x08,0x08,0x04,0x0f,0x00,0x00,0x80,0x40,0x40,0x40,0x82,0xfe,0x02,
'r',0x00,0x00,0x08,0x0f,0x04,0x08,0x08,0x00,0x00,0x00,0x08,0xf8,0x08,0x00,0x00,0x00,
's',0x00,0x06,0x09,0x08,0x08,0x08,0x04,0x00,0x00,0x10,0x08,0x88,0x88,0x48,0x30,0x00,
't',0x00,0x02,0x02,0x3f,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0xf0,0x08,0x08,0x10,0x00,
'u',0x04,0x07,0x00,0x00,0x00,0x04,0x07,0x00,0x00,0xf0,0x08,0x08,0x08,0x10,0xf8,0x08,
'v',0x04,0x07,0x04,0x00,0x04,0x07,0x04,0x00,0x00,0x00,0xe0,0x18,0xe0,0x00,0x00,0x00,
'w',0x04,0x07,0x04,0x00,0x04,0x07,0x04,0x00,0x00,0xc0,0x38,0xc0,0x38,0xc0,0x00,0x00,
'x',0x04,0x06,0x05,0x00,0x05,0x06,0x04,0x00,0x08,0x18,0x28,0xc0,0x28,0x18,0x08,0x00,
'y',0x00,0x04,0x07,0x04,0x00,0x04,0x07,0x04,0x00,0x04,0x04,0xc8,0x30,0xc0,0x00,0x00,
'z',0x00,0x02,0x02,0x02,0x02,0x02,0x03,0x00,0x00,0x18,0x28,0x48,0x48,0x88,0x08,0x00,
'.',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00,
'?',0x00,0x1C,0x20,0x20,0x21,0x22,0x1C,0x00,0x00,0x00,0x00,0xEC,0x0C,0x00,0x00,0x00};
  


  const  struct Cnchar Hanzi[]={
	 
"��", 0x00,0x04,0x08,0x33,0xE2,0x22,0x22,0x3F,
      0x22,0x22,0x22,0x26,0x62,0x20,0x00,0x00,
      0x20,0x20,0x20,0xE0,0x20,0x20,0x20,0xFF,
      0x20,0x20,0x20,0x20,0x20,0x60,0x20,0x00,

"��", 0x00,0x00,0x00,0x00,0x7F,0x44,0x44,0x44,
      0x44,0x44,0x44,0xFF,0x40,0x00,0x00,0x00,
      0x00,0x01,0x02,0x0C,0xF0,0x40,0x40,0x40,
      0x40,0x42,0x41,0xFE,0x00,0x00,0x00,0x00,
"��",0x00,0x00,0x00,0x7D,0x54,0x54,0x54,0x57,0x54,0x54,0x54,0x7C,0x00,0x00,0x00,0x00,
     0x00,0x22,0x42,0x92,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0x92,0x82,0x02,0x00,0x00,
"��",0x00,0x20,0xFF,0x24,0x24,0x24,0xFF,0x20,0x00,0x7F,0x44,0x44,0x44,0x7F,0x00,0x00,
     0x11,0x12,0xF4,0x90,0x90,0x98,0xF5,0x12,0x0C,0xF0,0x40,0x42,0x41,0xFE,0x00,0x00,
"һ",0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
"��",0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
     0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,
"��",0x00,0x20,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x20,0x00,0x00,
     0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
"��",0x00,0x3F,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x3F,0x00,0x00,
     0x00,0xFE,0x14,0x24,0xC4,0x04,0x04,0x04,0x04,0x84,0x44,0x44,0x44,0xFE,0x00,0x00,
"��",0x00,0x40,0x42,0x42,0x42,0x43,0x7E,0x42,0x42,0x42,0x42,0x43,0x40,0x40,0x00,0x00,
     0x02,0x02,0x02,0x02,0x1E,0xE2,0x02,0x02,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x00,
"��",0x04,0x04,0x04,0x04,0x04,0x04,0x84,0x44,0x34,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
     0x00,0x02,0x04,0x08,0x30,0xC0,0x00,0x00,0x00,0x80,0x40,0x20,0x18,0x06,0x00,0x00,
"��",0x02,0x02,0x42,0x33,0x00,0x02,0x05,0x79,0x41,0x41,0x41,0x79,0x05,0x04,0x04,0x00,
     0x00,0x00,0x00,0xFC,0x09,0x11,0x02,0xC2,0x34,0x08,0x14,0x62,0x82,0x01,0x01,0x00,
"��",0x00,0xE8,0xA8,0xAB,0xAA,0xEA,0xAA,0xBE,0xAA,0xEA,0xAA,0xAB,0xA8,0xE8,0x00,0x00,
     0x02,0x02,0x02,0xFE,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xFE,0x02,0x02,0x02,0x00,
"ʱ",0x00,0x3F,0x21,0x21,0x21,0x7F,0x28,0x08,0x09,0x08,0x08,0x08,0xFF,0x08,0x08,0x00,
      0x00,0xFC,0x08,0x08,0x08,0xFC,0x00,0x00,0x00,0xC4,0x02,0x01,0xFE,0x00,0x00,0x00,
"��",0x00,0x01,0x02,0x04,0x19,0x61,0x01,0x01,0xC1,0x31,0x09,0x04,0x03,0x01,0x01,0x00,
      0x80,0x00,0x01,0x02,0x04,0xF8,0x00,0x02,0x01,0x02,0xFC,0x00,0x00,0x80,0x00,0x00,
"��",0x04,0x24,0x24,0x25,0x7F,0xC5,0x44,0x01,0x0E,0x00,0xFF,0x00,0x08,0x04,0x06,0x00,
      0x08,0x10,0x60,0x80,0xFF,0x00,0x81,0x01,0x02,0x04,0xE8,0x10,0x20,0xC0,0x00,0x00,
"��",0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x02,0x02,0x02,0x02,0x02,0x02,0x00,0x00,0x00,
     0x02,0x02,0x02,0x02,0x02,0x02,0xFE,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00,
"��",0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x42,0x41,0x40,0x40,0x40,0x40,0x00,
     0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,
"��",0x10,0x10,0x10,0x10,0x11,0x1E,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
     0x04,0x08,0x12,0x62,0x82,0x82,0x82,0x82,0xFE,0x82,0x82,0x82,0x82,0x02,0x02,0x00,
"��",0x10,0x10,0x10,0x10,0x13,0x1C,0xF0,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,
     0x10,0x20,0x40,0x80,0xFF,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFF,0x00,0x00,0x00,
"��",0x00,0x1F,0x12,0x12,0x12,0x12,0xFF,0x12,0x12,0x12,0x12,0x3F,0x10,0x00,0x00,0x00,
      0x00,0xE0,0x40,0x40,0x40,0x40,0xFC,0x42,0x42,0x42,0x42,0xE2,0x02,0x0E,0x00,0x00,
"��",0x01,0x01,0x41,0x41,0x41,0x41,0x41,0x47,0x45,0x49,0x51,0x61,0x01,0x03,0x01,0x00,
      0x00,0x00,0x00,0x00,0x00,0x02,0x01,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
"��",0x00,0x00,0x00,0x7F,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x7F,0x00,0x00,0x00,0x00,
     0x00,0x00,0x00,0xFF,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0xFF,0x00,0x00,0x00,0x00, 
"��",0x00,0x00,0x7F,0x40,0x42,0x42,0x42,0x42,0x5F,0x42,0x42,0x42,0x42,0x43,0x40,0x00,
     0x01,0x06,0xF8,0x01,0x02,0x04,0x18,0x60,0x80,0x00,0x02,0x01,0x02,0xFC,0x00,0x00};






void delay_12864(int us)
{
	int i;
	for(;us>0;us--)
		for(i=0;i<100;i++);
}


//��һ������GPIOC��GPIOG Ϊ���ģʽ������дָ�������		    
//LED IO��ʼ��
void GPIO_Init_12864(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);//ʹ��PC,PG�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ10MHz
	 GPIO_Init(GPIOG, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOG
					
	 GPIO_InitStructure.GPIO_Pin=(uint16_t)0x00ff;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	 

	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_6;
	 GPIO_Init(GPIOD, &GPIO_InitStructure);
}




void WriteCmdByteTo_12864(uint8_t cmd)
{
	uint16_t tmp;
	CS = 0;	   //chip select,��Ƭѡ
	RD = 1;	   //disable read����ʧ��
	RS = 0;       //select command��ѡ������
	RW = 0;       //select write��ѡ��дģʽ
	delay_12864(1);
	tmp=GPIO_ReadOutputData(GPIOC);
	tmp=(tmp & 0xff00) | cmd;
	GPIO_Write(GPIOC,tmp);

	delay_12864(1);

	RW = 1;
}

void WriteDataByteTo_12864(uint8_t  data)
{
	uint16_t tmp;
	CS = 0;	
	RD = 1;	   //disable read����ʧ��	
	RS = 1;       //select data��ѡ������
	RW = 0;       //select write��ѡ��дģʽ
	delay_12864(1);

	tmp=GPIO_ReadOutputData(GPIOC);
	tmp=(tmp & 0xff00) | data;
	GPIO_Write(GPIOC,tmp);
	  
	delay_12864(10);

	RW = 1;  
}
  
void Init_12864()
{
	RST = 0;
	delay_12864(1);
	CS = 0;
	RST = 1;
	

	WriteCmdByteTo_12864(0xE2);  //reset
	delay_12864(1);	   //��ʱһ��
	WriteCmdByteTo_12864(0xA0);  //ADC select segment direction 
	WriteCmdByteTo_12864(0xC0);  //Common direction 
	WriteCmdByteTo_12864(0xA6);  //reverse display
	WriteCmdByteTo_12864(0xA4);  //normal display
	WriteCmdByteTo_12864(0xA2);  //bias set 1/9
	WriteCmdByteTo_12864(0xF8);  //Boost ratio set
	WriteCmdByteTo_12864(0x01);  //x4
	WriteCmdByteTo_12864(0x81);  //V0 a set
	WriteCmdByteTo_12864(0x23);
	WriteCmdByteTo_12864(0x25);  //Ra/Rb set
	WriteCmdByteTo_12864(0x2F);
	delay_12864(1);
	WriteCmdByteTo_12864(0x40);  //start line
	WriteCmdByteTo_12864(0xAf);  // display on
	delay_12864(1);
}

void ClearScreen(void)
{  
	uint8_t i, j;
	for(i=0; i<8; i++)
	{	
		WriteCmdByteTo_12864(0xB0+i); 
		WriteCmdByteTo_12864(0x10); 
		WriteCmdByteTo_12864(0x00);							   
		for(j=0; j<128; j++)
		   WriteDataByteTo_12864(0x00);  
	}  
}


/**
*12864�Ķ�д����Ӧ���ǣ���д��ָ��λ�ö�дһ���ֽڵĺ�����
*�ٷ�װ��2��������ʵ�ֶ�д�ַ�����
*���˽����12864�ļĴ������ú��ٸ�д��
*/
//�˺������stdio.h��sprintf������������󲿷�Ҫ��
//kn����״̬,Ϊ0�򲻸�����ʾ

void WriteEn(u8 kn,u8 x,u8 y,u8 *cn)
{ 
	u8 N,xn,n,x1,fn,fg,fg1,ib,cn1,Lx; 
	fg=0;
	N=0; 
	Lx=0;//������ʾ��x���򳤶ȣ���λΪһ��Ӣ���ַ�����
	if(y<7)
	{
		n=(8-y)/2;
		xn=(128-x)/8;
		if(n>1)   
			n=(n-1)*16;
		else      
			n=0;
		n=n+xn;
		fg1=0;
		while(n&&(*cn != '\0'))
		{
			cn1=*cn;
			if(Zifu[N].Index!= cn1)
			{ 
				if(!fg)
				{
					fn=N;
					fg=1;
				} 
				N=(N+1)%68;
				if(fn==N) 
					n=0; 
			}
			if(Zifu[N].Index==cn1)
			{ 
				if(x>121&&!fg1)
				{ 
					x=0;
					y=y+1;
				}
				if(x>121&&fg1)
				{ 
					x=0;
					y=y+2;
				}	 //??????????
				WriteCmdByteTo_12864(7-y+0xb0);
				x1=(x>>4)&0x0f;
				WriteCmdByteTo_12864(0x10 + x1);
				x1=x&0x0f;     
				WriteCmdByteTo_12864(0x00 + x1);
				if(Hilight[kn][0]==y/2*16+x/8)
					Lx=Hilight[kn][1];	
				for(ib=0;ib<16;ib++)
				{ 
					if(ib==8)
					{
						WriteCmdByteTo_12864(6-y+0xb0);
						x1=(x>>4)&0x0f;
						WriteCmdByteTo_12864(0x10+x1);
						x1=x&0x0f;     
						WriteCmdByteTo_12864(0x00+x1);
					}
					if(Lx)
						WriteDataByteTo_12864(~Zifu[N].Msk[ib]);
					else 
						WriteDataByteTo_12864(Zifu[N].Msk[ib]);
				}
				n--;
				if(Lx)
					Lx--; 
				if(n) 
					x=x+8;
				else  
				{
					WriteCmdByteTo_12864(0xb0);
					WriteCmdByteTo_12864(0x10);
					WriteCmdByteTo_12864(0x00);
				}
				cn++;
				N++;
				fg=0;
				fg1=1;
			}
		}
	}
}

  void WriteCn(u8 kn,u8 x,u8 y,u8 *cn)
   { u8 N,xn,n,x1,fn,fg,fg1,i,Lx;
     fg=0;N=0;Lx=0;
   if(y<7)
    {n=(8-y)/2;xn=(128-x)/16;
	    if(n>1)   n=(n-1)*8;
		else      n=0;
		n=n+xn;fg1=0;
	  while(n&&(*cn != '\0'))
    	{ if((Hanzi[N].Index[0]!= *cn)||(Hanzi[N].Index[1] != *(cn+1)))
	       { if(!fg)
		     {fn=N;fg=1;} 
			  N=(N+1)%197;
		     if(fn==N) n=0; }
	      if((Hanzi[N].Index[0]== *cn)&&(Hanzi[N].Index[1] == *(cn+1)))
	       { if(x>113&&!fg1)
		      { x=0;y=y+1;}
			 if(x>113&&fg1)
		      { x=0;y=y+2;}	 //??????????
		     WriteCmdByteTo_12864(7-y+0xb0);
	         x1=(x>>4)&0x0f;WriteCmdByteTo_12864(0x10 + x1);
		     x1=x&0x0f;     WriteCmdByteTo_12864(0x00 + x1);
			 if(Hilight[kn][0]==y/2*16+x/8)
			    Lx=Hilight[kn][1];	
	        for(i=0;i<32;i++)
             { if(i==16)
	           {WriteCmdByteTo_12864(6-y+0xb0);
	            x1=x>>4&0x0f;WriteCmdByteTo_12864(0x10 + x1);
		        x1=x&0x0f;     WriteCmdByteTo_12864(0x00 + x1);}
                if(Lx)
					WriteDataByteTo_12864(~Hanzi[N].Msk[i]);
				else
					WriteDataByteTo_12864(Hanzi[N].Msk[i]);}
		        n--;cn=cn+2;N++;
				if(Lx>1)Lx=Lx-2;
				if(n) x=x+16;
				else  
				{WriteCmdByteTo_12864(0xb0);
	             WriteCmdByteTo_12864(0x10);
		         WriteCmdByteTo_12864(0x00);}
				fg=0;fg1=1;}}}}










