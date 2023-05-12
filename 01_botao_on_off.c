#define BT PIND.B2
#define LED PORTD.B5

void main (void){
DDRD.B2 = 0;
DDRD.B5 = 1;
LED = 0;
while (1)
{
	while (BT == 0);
	while (BT == 1)
	{
		LED = !LED;
	}
}
}
