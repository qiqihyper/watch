/***关于1602相关函数实现*******/

#include "1602.h"


/***液晶初始化*****/
void lcd_wait_for_enable()
{
	DATAPORT = 0xff;

	RS=0; RW=1; _nop_();	E=0; _nop_(); _nop_();
	while(DATAPORT & BUSY);
	E=1;
}

/*****写控制字符****attrib控制是否检测忙信号***/
void lcd_write_command(uchar cmd, uchar attrib)
{
	if(attrib)	lcd_wait_for_enable();	//检测是否繁忙

	RS=0; RW=0; _nop_(); E=1;
	DATAPORT = cmd;  _nop_();

	E=0; _nop_(); _nop(); E=1;
}

/*****当前位置写入字符**********/
void lcd_write_data(char wdata)
{
	lcd_wait_for_enable();

	RS=1; RW=0; _nop_(); E= 1;
	DATAPORT = wdata;  _nop_();

	E=0; _nop_(); _nop_(); E=1;
}

/****液晶初始化程序*******/
void lcd_reset()
{		  //按照手册相关步骤初始化

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

/****光标定位******/
void disp_xy(uchar x, uchar y)
{
	uchar tmp;
	tmp = x & 0x0f;
	y  &= 0x1;
	if(x > 15)  tmp |= 0x40;  //在第二行显示
	if(y == 1)	tmp |= 0x40;
	tmp |= 0x80;			   //D7位置1，表示状态繁忙
	lcd_write_command(tmp,0);
}

/*****指定位置显示字符********/
void disp_one_char(uchar x, uchar y, uchar ddata)
{
	disp_xy(x, y);
	lcd_write_data(ddata);
}

/***指定位置显示字符串*******/
void disp_msg(uchar x, uchar y, uchar *ptr)
{
	uchar i,l=0;
	while(*ptr++ > 0) l++;
	ptr=ptr-(l+1);	//ptr指针复位
	for(i=0;i<l;i++)
	{
		disp_one_char(x++, y, *ptr++);
		if(x == 31) {x = 0, y ^= 0;}
	}
}