#include <AT89X51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int

#define RS	P2_0
#define RW	P2_1
#define E	P2_7
#define SW	P3_6

#define DATAPORT P0
#define BUSY	 0x80

char init[] = "Wellcom to play    MCU Board";

/***微秒级延时，time<255****/
void delay_us(unsigned char time)
{
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;

	while(TL0 < time);//机器周期为1us，执行一次计时1us

	TR0 = 0;
}

/****毫秒级延时，time<65535****/
void delay_ms(unsigned int time)
{
	unsigned int n = 0;
	TR0 = 1;
	
	while(n < time)
	{
		TH0 = 0;		//赋初值20(十进制)
		TL0 = 20;
		while(TH0 < 4);	   //4x255-20 = 1000;(即1ms)
		n++;
	}
	TR0 = 0;		
}

/***不精确ms延时*****/
void delay_mms(unsigned int number)
{
	unsigned char temp;

	for(;number!=0;number--)
	{
		for(temp=112;temp!=0;temp--);
	}
}






/***液晶闲忙*****/
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
	lcd_write_command(0x38,0);
	delay_ms(10);  
	lcd_write_command(0x38,0);
	delay_ms(10);
	lcd_write_command(0x38,0);
	delay_ms(10);

	lcd_write_command(0x38,1);
	lcd_write_command(0x08,1);
	lcd_write_command(0x01,1);
	lcd_write_command(0x06,1);
	lcd_write_command(0x0c,1);
}

/****光标定位******/
void disp_xy(char x, char y)
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



int main()
{
	delay_mms(2000);
	SW = 0;
	lcd_reset();
	disp_msg(0, 0, init);
	while(1);
}

 