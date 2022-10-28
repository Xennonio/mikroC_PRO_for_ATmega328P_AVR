#define        LED        PORTD.B5

//ATMEGA328
//T = (1 / 16MHz) * 64 * 250
//T = 1000us

void ISR_Timer0 (void) iv IVT_ADDR_TIMER0_OVF
{
    //Recarrega o valor do timer
    TCNT0 = 5;
    //Alterna as saidas da PORTD
    LED = !LED;
}

void Init_Timer0 (void)
{
    //Normal Mode
    WGM00_Bit = 0;
    WGM01_Bit = 0;
    WGM02_Bit = 0;

    //configura prescaler 1:64
    TCCR0B.CS00 = 1;
    TCCR0B.CS01 = 1;
    TCCR0B.CS02 = 0;

    // habilita interrupção por overflow
    TIMSK0.TOIE0 = 1;
    //Carrega valor do timer
    TCNT0 = 5;
    //habilita int. global
    asm sei;
}

void main (void)
{
      DDRD.B5 = 1; //PORTD como saida
      LED = 0;

      Init_Timer0();

      while (1)
      {
       //Insira aqui seu código (laço principal)
      }
}
