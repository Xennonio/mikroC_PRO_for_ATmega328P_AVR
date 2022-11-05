#define LED PORTD.B5
//T = (1 / 16) * 64 * 250 [MHz] = 1000μs
void ISR_Timer0 (void) iv IVT_ADDR_TIMER0_OVF{
    TCNT0 = 5; //Recarrega o valor do timer
    LED = !LED; //Alterna as saidas da PORTD
}

void Init_Timer0 (void)
{
    //Normal Mode
    WGM00_Bit = 0;
    WGM01_Bit = 0;
    WGM02_Bit = 0;
    //Configura prescaler 1:64
    TCCR0B.CS00 = 1;
    TCCR0B.CS01 = 1;
    TCCR0B.CS02 = 0;
    
    TIMSK0.TOIE0 = 1; //Habilita interrupção por overflow
    TCNT0 = 5; //Carrega valor do timer
    asm sei; //habilita int. global
}

void main (void)
{
    DDRD.B5 = 1; //PORTD como saida
    LED = 0;
    
    Init_Timer0();
    
    while (1){
       //Insira aqui seu código (laço principal)
    }
}
