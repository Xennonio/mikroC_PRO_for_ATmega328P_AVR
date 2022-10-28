#define a PORTC.B3
#define b PORTC.B2
#define c PORTC.B1
#define d PORTC.B0

#define e PORTB.B3
#define f PORTB.B2
#define g PORTB.B1

#define BT0 PIND.B2
#define BT1 PIND.B3
#define BT2 PIND.B4

#define Led1 PORTB.B4
#define Led2 PORTB.B5

unsigned char Contagem[10]={0b0000001, 0b1001111, 0b0010010, 0b0000110, 0b1001100, 0b0100100, 0b0100000, 0b0001111, 0b0000000, 0b0000100};

unsigned char LEDs;

int r=0;

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
 DDRB.B4 = 1;
 DDRB.B5 = 1;

 Led1=0;
 Led2=0;

 while (1){

   if(BT0==1){
    r++;
    delay_ms(300);
   }

   if(BT1==1){
   r--;
    if(r<0){
      r=99;
    }
    delay_ms(300);
   }

   if(BT2==1){
    r=0;
    delay_ms(300);
   }

   Led1=1;
   Led2=0;

   cont(r%10);

   Led1=0;
   Led2=1;

   cont((r/10)%10);

   continue;
 }
}
