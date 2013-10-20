
#include <AT89X51.H>
#include <delay.h>
#include <stdio.h>
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int



#define RS    P2_0       
#define RW    P2_1       
#define Elcm  P2_7       
#define SW	  P3_6

#define DataPort P0      
#define Busy    0x80
#define DIR P1_0

sbit 		B7281_CLK	= P1^5;		
sbit 		B7281_DAT	= P1^4;		
sbit 		B7281_KEY	= P3^3;		
sbit          BEEP = P1^7;


void WaitForEnable( void );
void LcdWriteData( char dataW );
void LcdWriteCommand( uchar CMD,uchar AttribC ) ;
void LcdReset( void ) ;
void DispOneChar(uchar x,uchar y,uchar Wdata)  ;
void ePutstr(uchar x,uchar y, uchar code *ptr) ;
void Disp_XY( char posx,char posy);
void flic(char x,char y, char aaaa);



void B8279_delay(unsigned char);				
void write728x(unsigned char, unsigned char);	
unsigned char read728x(unsigned char);		
void send_byte(unsigned char);			
unsigned char receive_byte(void);			


void disp_time();
void set();


unsigned char key_number=0xff;
 code char examp1[]="Time:    :  :   Alarm:   :";
 code char stopmo[]="Stop Mode:";
uchar i=21;
char sec=0,min=0,hour=0,ahour,amin;
char ssec,smsec,smin,shour;
char fflash,count,hourset,minset,secset,ahourset,aminset,setnext,alarm_flag,stop_mode,stop_start,stop_for;
char newhour1,newhour0,newmin1,newmin0,newsec1,newsec0,anewhour1,anewhour0,anewmin1,anewmin0;















void time0(void) interrupt 1
{ 
	TH0=0X53;
	TL0=0X33;
	
	if(stop_mode)
	{
		if(stop_start)
		{
					if(i%2 == 0)
					{
					 smsec++;
						 if(smsec ==10)
						 {
						 	smsec=0;
						 	ssec++;
							if(ssec == 60)
							{
								ssec=0;	
								smin++;
								if(smin == 60)
								{
									smin=0;
									shour++;
									if(shour == 24)
									{
										shour=0;
									}
								}
							}
						 }
					}
			}
	}
	i--;
	if(i==0)
	{
		i=21;
		sec++;
		fflash = (!fflash);
		if(sec == 60)
		{
			sec=0;	
			min++;
			if(min == 60)
			{
				min=0;
				hour++;
				if(hour == 24)
				{
					hour=0;
				}
			}
		}

	}
}



void delay1(int ms)
{
 unsigned char y;
  while(ms--)
 {
  for(y = 0; y<250; y++)
  {
   _nop_();
   _nop_();
   _nop_();
   _nop_();
  }
 }
}
void delay()
{
   uchar j;
   for(j=250;j>0;j--);
}

void beep()
  {
    unsigned char y;
    for (y=0;y<100;y++)
    {
      delay();
      BEEP=!BEEP;                
    } 
    BEEP=1;                      
	delay1(50);
  }







void  Set_Flash(uchar row,uchar col )
{

   if(fflash)
   { 
		DispOneChar(row,col,(count|0x30));                           
   }
   else
   {                            
     	DispOneChar(row,col,' ');                           
   }
}

void char_set(uchar row,uchar col, uchar num)
{
		switch (key_number)
		{
  		 	case 0:
				count = 0;
				break;
			case 1:
				count = 1;
				break;
			case 2:
				count = 2;
				break;
			case 3:
				count = 3;
				break;
			case 4:
				count = 4;
				break;
			case 5:
				count = 5;
				break;
			case 6:
				count = 6;
				break;
			case 7:
				count = 7;
				break;
			case 8:
				count = 8;
				break;
			case 9:
				count = 9;
				break;
		}
		if(count > num)	  count = 0;
	 Set_Flash(row,col);
}







void key0() interrupt 2
{
		key_number=read728x(0x13);	
		beep();
}







void beep_and_disp()
{
	beep();
	if(fflash)
	{
		DispOneChar(9,1,':');
		DispOneChar(10,1,((amin/10)|0x30));
		DispOneChar(11,1,((amin%10)|0x30));
		DispOneChar(7,1,((ahour/10)|0x30));
		DispOneChar(8,1,((ahour%10)|0x30));

	}
	else
	{
		DispOneChar(9,1,' ');
		DispOneChar(10,1,' ');
		DispOneChar(11,1,' ');
		DispOneChar(7,1,' ');
		DispOneChar(8,1,' ');

	}

		
}
void disp_alarm()
{
		DispOneChar(9,1,':');
		DispOneChar(10,1,((amin/10)|0x30));
		DispOneChar(11,1,((amin%10)|0x30));
		DispOneChar(7,1,((ahour/10)|0x30));
		DispOneChar(8,1,((ahour%10)|0x30));
}

void time_compare()
{
	if(alarm_flag)
	{
		if(hour == ahour)
		{
			if(min == amin)
			beep_and_disp(); else disp_alarm();
		}else disp_alarm();
	}else disp_alarm();
} 


void disp_stop()
{
	DispOneChar(15,1,(smsec|0x30));
	DispOneChar(12,1,((ssec/10)|0x30));
	DispOneChar(13,1,((ssec%10)|0x30));
	DispOneChar(9,1,((smin/10)|0x30));
	DispOneChar(10,1,((smin%10)|0x30));
	DispOneChar(6,1,((shour/10)|0x30));
	DispOneChar(7,1,((shour%10)|0x30));

}







void main(void)
{
	DelayMs(2000);                    
	SW=0;
	LcdReset();                          
	ePutstr(0,0,examp1); 

	TMOD=1;						
	TH0=0X53;
	TL0=0X33;					
	DIR=1;
	ET0=1;
	EA=1;						
	TR0=1;

	IT1=1;
	EX1=1;

	write728x(0x12,0x80);	



	while(1)                         
	{

		disp_time();

  		time_compare();



		if(key_number == 15)
		{
		  hourset=1;
		  count = (hour/10);
		}


	  while(hourset)
	  {
	  		char_set(7,0,2);
			if(key_number == 10)
			{
				newhour1=count;
				DispOneChar(7,0,(count|0x30));
				hourset=0;
				setnext = 1;
				count = (hour%10);
			
				key_number = 0xff;
			}
			while(setnext)
			{
				if(newhour1 == 2)
					 char_set(8,0,3);
				else
					 char_set(8,0,9);
				if(key_number == 10)
				{
				newhour0=count;
				DispOneChar(8,0,(count|0x30));
				hour=newhour1*10+newhour0;
				setnext = 0;
				minset = 1;
				count  = (min/10);
				key_number = 0xff;
				}
			} 			
	  }
	  while(minset)
	  {
	  		char_set(10,0,5);
			if(key_number == 10)
			{
				newmin1=count;
				DispOneChar(10,0,(count|0x30));
				minset=0;
				setnext = 1;
				count = (min%10);
			
				key_number = 0xff;
			}
			while(setnext)
			{
				char_set(11,0,9);
				if(key_number == 10)
				{
				newmin0=count;
				DispOneChar(11,0,(count|0x30));
				min=newmin1*10+newmin0;
				setnext = 0;
				secset = 1;
				count  = (sec/10);
				key_number = 0xff;
				}
			} 
	  }
	  while(secset)
	  {
	  		char_set(13,0,5);
			if(key_number == 10)
			{
				newsec1=count;
				DispOneChar(13,0,(count|0x30));
				secset=0;
				setnext = 1;
				count = (sec%10);
			
				key_number = 0xff;
			}
			while(setnext)
			{
				char_set(14,0,9);
				if(key_number == 10)
				{
				newsec0=count;
				DispOneChar(14,0,(count|0x30));
				sec=newsec1*10+newsec0;
				setnext = 0;
				count  = 0;
				key_number = 0xff;
				}
			} 
	  }





	  if(key_number == 14)
	  {
	  	if(alarm_flag == 0)
		{
		alarm_flag =1;
		DispOneChar(14,1,'"');


		  ahourset=1;
		  count = (ahour/10);}
		  else
		  {
		  alarm_flag = 0;
		  DispOneChar(14,1,' ');
		  key_number = 0xff;
		  }
	  }
	  while(ahourset)
	  {
	  		char_set(7,1,2);
			if(key_number == 10)
			{
				anewhour1=count;
				DispOneChar(7,1,(count|0x30));
				ahourset=0;
				setnext = 1;
				count = (ahour%10);
			
				key_number = 0xff;
			}
			while(setnext)
			{
				if(anewhour1 == 2)
					 char_set(8,1,3);
				else
					 char_set(8,1,9);
				if(key_number == 10)
				{
				anewhour0=count;
				DispOneChar(8,1,(count|0x30));
				ahour=anewhour1*10+anewhour0;
				setnext = 0;
				aminset = 1;
				count  = (amin/10);
				key_number = 0xff;
				}
			} 
	  }
	  while(aminset)
	  {
	  		char_set(10,1,5);
			if(key_number == 10)
			{
				anewmin1=count;
				DispOneChar(10,1,(count|0x30));
				aminset=0;
				setnext = 1;
				count = (amin%10);
			
				key_number = 0xff;
			}
			while(setnext)
			{
				char_set(11,1,9);
				if(key_number == 10)
				{
				anewmin0=count;
				DispOneChar(11,1,(count|0x30));
				amin=anewmin1*10+anewmin0;
				setnext = 0;
				count  = 0;
				key_number = 0xff;
				}
			} 
	  }



	  if(key_number == 13)
	  {
	  		key_number=0xff;
			stop_for =1;
			stop_mode=1;
			LcdWriteCommand(0x01,1) ;
			ePutstr(0,0,stopmo);
			DispOneChar(14,1,'.');
			DispOneChar(11,1,':'); 
			DispOneChar(8,1,':'); 

 
	  }
	  while(stop_for)
	  {
	  		while(key_number !=13)
			{
				if(key_number == 11)
				{
					key_number=0xff;
					smsec=0;ssec=0;smin=0;shour=0;
				}
				if(key_number == 12)
				{
					key_number=0xff;
					stop_start = (!stop_start);
				}
				disp_stop();
			}
			key_number=0xff;
			LcdWriteCommand(0x01,1) ;
			ePutstr(0,0,examp1);
			if(alarm_flag)
					DispOneChar(14,1,'"');	
			if(stop_start)
				stop_for=0;
			else
			{
				stop_for=0;
				stop_mode = 0; 
			}
	  }













	}

 }


void disp_time()
{
	DispOneChar(13,0,((sec/10)|0x30));
	DispOneChar(14,0,((sec%10)|0x30));
	DispOneChar(10,0,((min/10)|0x30));
	DispOneChar(11,0,((min%10)|0x30));
	DispOneChar(7,0,((hour/10)|0x30));
	DispOneChar(8,0,((hour%10)|0x30));
}























void ePutstr(uchar x,uchar y, uchar code *ptr) {
uchar i,l=0;
    while (*ptr++ >0){l++;}; 
	ptr=ptr-(l+1);            
    for (i=0;i<l;i++)     { DispOneChar(x++,y,*ptr++);
                            if ( x == 31 )  { x = 0; y ^= 0; }
                           }
                                                }


void Disp_XY( char posx,char posy) {

uchar temp;

    temp = posx & 0x0f;
    posy &= 0x1;
    if ( posx>15 )   temp |= 0x40;
	if (posy==1)   temp |=0x40;
    temp |= 0x80;
    LcdWriteCommand(temp,0);
}

void DispOneChar(uchar x,uchar y,uchar Wdata)  {

    Disp_XY( x, y );                
    LcdWriteData( Wdata );          
}

void LcdReset( void )  {

       LcdWriteCommand( 0x38, 0);           
        DelayMs(10);
    LcdWriteCommand( 0x38, 0);            
        DelayMs(10);
    LcdWriteCommand( 0x38, 0);
        DelayMs(10);

    LcdWriteCommand( 0x38, 1);            
    LcdWriteCommand( 0x08, 1);            
    LcdWriteCommand( 0x01, 1);           
    LcdWriteCommand( 0x06, 1);           
    LcdWriteCommand( 0x0c, 1);           
}

void LcdWriteCommand( uchar CMD,uchar AttribC )  {

    if (AttribC) WaitForEnable();                
    RS=0; RW=0; _nop_();Elcm=1;
    DataPort=CMD; _nop_();
    Elcm=0;_nop_();_nop_();Elcm=1;           
}

void LcdWriteData( char dataW )  {

    WaitForEnable();

    RS=1; RW=0; _nop_();Elcm=1;

    DataPort=dataW; _nop_();

    Elcm=0; _nop_(); _nop_(); Elcm=1;        
}

void WaitForEnable( void ) {

    DataPort=0xff;
	RS=0;
	RW=1; 
	_nop_();
	Elcm=0; 
	_nop_();
	_nop_();

    while( DataPort & Busy );
    Elcm=1;
}


























void write728x(unsigned char reg_add, unsigned char write_data)
{
	send_byte(reg_add);		
	send_byte(write_data);	
}

unsigned char read728x(unsigned char reg_add)
{
	send_byte(reg_add|0x80);	
	return(receive_byte());	
}

void send_byte(unsigned char send_byte)
{
	unsigned char bit_counter;
	B7281_CLK=0;		
	B7281_CLK=1;
	do
	{
		B7281_CLK=0;			
		B7281_CLK=1;
	}while (B7281_DAT);	
	B7281_CLK=0;
	B7281_CLK=1;
	while (!B7281_DAT);		
	for (bit_counter=0;bit_counter<8;bit_counter++)
	{			
		if ((send_byte&0x80)==0)
 		{
  			B7281_DAT=0;
  	 	}
  	 	else
  	 	{
  	 		B7281_DAT=1;	
  	  	}
 		send_byte=send_byte*2;	
		B7281_CLK=0;			
		B7281_CLK=0;
		B7281_CLK=1;
		B8279_delay(1);	
	}
	B7281_DAT=1;				
}

unsigned char receive_byte(void)
{
	unsigned char bit_counter, in_byte;
	B7281_CLK=0;			
	B7281_CLK=1;

	while (B7281_DAT);		

	B7281_CLK=0;		
	B7281_CLK=1;
	for (bit_counter=0;bit_counter<8;bit_counter++)
	{			
		B8279_delay(1);	
		in_byte=in_byte*2;	
		if (B7281_DAT)	
		{
			in_byte=in_byte|0x01;
		}
		B7281_CLK=0;		
		B7281_CLK=0;
		B7281_CLK=1;
	}
	return(in_byte);
}

void B8279_delay(unsigned char time)
{
	while (time!=0)
	{
		time--;
	}
}
