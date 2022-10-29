#define Vm1 PORTC.B1
#define Am1 PORTC.B0
#define Vd1 PORTB.B3

#define Vm2 PORTB.B2
#define Am2 PORTB.B1
#define Vd2 PORTB.B0

unsigned char Semaforo[4] = {0b00100001, 0b00100010, 0b00001100, 0b00010100}, LEDs, i;

void main (void){
 DDRC.B1 = 1;
 DDRC.B0 = 1;
 DDRB.B3 = 1;
 DDRB.B2 = 1;
 DDRB.B1 = 1;
 DDRB.B0 = 1;
 
 LEDs = 0xFF;
 Vm1 = LEDs.B5;
 Am1 = LEDs.B4;
 Vd1 = LEDs.B3;

 Vm2 = LEDs.B2;
 Am2 = LEDs.B1;
 Vd2 = LEDs.B0;
 delay_ms(1000);
 
 while (1){
  for (i = 0; i <= 3; i++)
   LEDs = Semaforo[i];
 }

 Vm1 = LEDs.B5;
 Am1 = LEDs.B4;
 Vd1 = LEDs.B3;

 Vm2 = LEDs.B2;
 Am2 = LEDs.B1;
 Vd2 = LEDs.B0;
 delay_ms(3000);
}
