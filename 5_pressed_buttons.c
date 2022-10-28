#define a PORTC.B3
#define b PORTC.B2
#define c PORTC.B1
#define d PORTC.B0

#define e PORTB.B3
#define f PORTB.B2
#define g PORTB.B1

#define BT0 PIND.B2
#define BT1 PIND.B3

#define Led PORTB.B4

unsigned char Contagem[2]={0b0000001, 0b1001111};

unsigned char LEDs;

cont(int n){
   LEDs = Contagem[n];

    a = LEDs.B6;
    b = LEDs.B5;
    c = LEDs.B4;
    d = LEDs.B3;

    e = LEDs.B2;
    f = LEDs.B1;
    g = LEDs.B0;

    delay_ms(10);
}

void main (void)
{
 DDRC.B0 = 1;
 DDRC.B1 = 1;
 DDRC.B2 = 1;
 DDRC.B3 = 1;

 DDRB.B1 = 1;
 DDRB.B2 = 1;
 DDRB.B3 = 1;

 DDRD.B2 = 0;
 DDRD.B3 = 0;

 DDRB.B4 = 1;
 DDRB.B5 = 1;
 DDRB.B7 = 1;

 Led=0;

 while (1){

   if(BT0==1){
     cont(1);
   } else cont(0);

   if(BT1==1){
     Led=1;
   } else Led=0;
 }
}
