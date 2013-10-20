void DelayMs(unsigned int number) {
unsigned char temp;
 for(;number!=0;number--) {
  for(temp=112;temp!=0;temp--) {
  }
 }
}  
void Delay_us(unsigned char nUs)
{
	TH1=0;
	TL1=0;
	TR1=1;
	while(TL1<nUs)
	{
	}
	TR1=0;
}

void Delay_ms(unsigned int nMs)
{
	unsigned int n=0;
	TR1=1;
	while(n<nMs)
	{
		TH1=0;
		TL1=20;
		while(TH1<4)
		{
		}
		n++;
	}
	TR1=0;
}
