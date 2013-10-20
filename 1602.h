/*****1602液晶所要使用的宏定义与函数声明******/

#ifndef _1602_H
#define _1602_H

/*[注]:AT89C51的晶振频率为11.0592MHz
====================================================================*/

/*            ACM1602A （16x2）接线图
=====================================================================
连接线图:   ACM-------51     ACM-------51     ACM--------51
            DB0-----P0.0     DB4-----P0.4     RS-------P2_0
            DB1-----P0.1     DB5-----P0.5     WR-------P2_1
            DB2-----P0.2     DB6-----P0.6     E--------P2_7
            DB3-----P0.3     DB7-----P0.7     BLA------GND
====================================================================*/

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

extern void lcd_wait_for_enable();
extern void lcd_write_data(char wdata);
extern void lcd_write_command(uchar cmd, uchar attrib);
extern void lcd_reset();
extern void disp_one_char(uchar x, uchar y, uchar ddata);
extern void disp_msg(uchar x, uchar y, uchar *ptr);

#endif
