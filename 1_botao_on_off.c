#define BT	PIND.B2
#define LED	PORTD.B5

#define off	0
#define on	1

void main (void)
{
DDRD.B2 = 0;
DDRD.B5 = 1;

LED = off;

 while (1)
 {
	while (BT == off);
	while (BT == on);
	LED = !LED;
 }
}
