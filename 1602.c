/***����1602��غ���ʵ��*******/

#include "1602.h"


/***Һ����ʼ��*****/
void lcd_wait_for_enable()
{
	DATAPORT = 0xff;

	RS=0; RW=1; _nop_();	E=0; _nop_(); _nop_();
	while(DATAPORT & BUSY);
	E=1;
}

/*****д�����ַ�****attrib�����Ƿ���æ�ź�***/
void lcd_write_command(uchar cmd, uchar attrib)
{
	if(attrib)	lcd_wait_for_enable();	//����Ƿ�æ

	RS=0; RW=0; _nop_(); E=1;
	DATAPORT = cmd;  _nop_();

	E=0; _nop_(); _nop(); E=1;
}

/*****��ǰλ��д���ַ�**********/
void lcd_write_data(char wdata)
{
	lcd_wait_for_enable();

	RS=1; RW=0; _nop_(); E= 1;
	DATAPORT = wdata;  _nop_();

	E=0; _nop_(); _nop_(); E=1;
}

/****Һ����ʼ������*******/
void lcd_reset()
{		  //�����ֲ���ز����ʼ��

unsigned int i;
	lcd_write_command(0x38,0);
//	delay_Ms(10);  
	for (i=0;i<1000;i++);
	lcd_write_command(0x38,0);
//	delay_Ms(10);
	for (i=0;i<1000;i++);
	lcd_write_command(0x38,0);
//	delay_Ms(10);
	for (i=0;i<1000;i++);

	lcd_write_command(0x38,1);
	lcd_write_command(0x08,1);
	lcd_write_command(0x01,1);
	lcd_write_command(0x06,1);
	lcd_write_command(0x0c,1);
}

/****��궨λ******/
void disp_xy(uchar x, uchar y)
{
	uchar tmp;
	tmp = x & 0x0f;
	y  &= 0x1;
	if(x > 15)  tmp |= 0x40;  //�ڵڶ�����ʾ
	if(y == 1)	tmp |= 0x40;
	tmp |= 0x80;			   //D7λ��1����ʾ״̬��æ
	lcd_write_command(tmp,0);
}

/*****ָ��λ����ʾ�ַ�********/
void disp_one_char(uchar x, uchar y, uchar ddata)
{
	disp_xy(x, y);
	lcd_write_data(ddata);
}

/***ָ��λ����ʾ�ַ���*******/
void disp_msg(uchar x, uchar y, uchar *ptr)
{
	uchar i,l=0;
	while(*ptr++ > 0) l++;
	ptr=ptr-(l+1);	//ptrָ�븴λ
	for(i=0;i<l;i++)
	{
		disp_one_char(x++, y, *ptr++);
		if(x == 31) {x = 0, y ^= 0;}
	}
}