//Conexão do Hardware do LCD
#define Data7 PORTB.B0
#define Data6 PORTB.B1
#define Data5 PORTB.B2
#define Data4 PORTB.B3
#define RS PORTB.B4
#define EN PORTB.B5

//Controle de posicionamento de caracteres e strings no LCD
#define NumCol 17 //Número de colunas do LCD (17x4)

//Endereços das linhas do LCD
#define Linha1 0x80
#define Linha2 0xC0
#define Linha3 0x94
#define Linha4 0xD4

//Códigos das mensagens
#define MSG1 0
#define MSG2 1
#define MSG3 2
#define MSG4 3
#define MSG5 4
#define MSG6 5

//Caracteres especiais a serem carregados na CGRAM do display LCD
unsigned char Tab_CGRam[40] = {0x18, 0x18, 0x06, 0x09, 0x08, 0x09, 0x06, 0x00, //°C
                               0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00, //<
                               0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00, //>
                               0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, //Seta para cima
                               0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00},//Seta para baixo

//Parâmetros de configuração inicial do display LCD 4 Bits
Tab_Conf[4] = {0x28, 0x06, 0x01, 0x0C},
Tab_4Bit[4] = {0x30, 0x30, 0x30, 0x20},
Uni, Dez, Cen, display, setpoint[3];

//Declaração de variáveis globais
double aux1, aux2,
analog,
lastanalog,
lasterr = 255,
error,
properr,
interr,
derr,
PID,
input,
setpoint2[3],
kp = 0.2, ki = 0.2, kd = 0.03, ideal_value = 512.0;
int pwm = 128;

//------Começo do Código de Varredura de Teclado
#define col_1   PORTD.B4
#define col_2   PORTD.B0
#define col_3   PORTD.B1
#define row_A   PINC.B2
#define row_B   PINC.B1
#define row_C   PINC.B0
#define row_D   PIND.B7
/*******************************************************************************
*                                                                              *
*  Conexões do teclado:                   col_1   col_2   col_3       +5Vcc   *
*                                           |       |       |            |     *
*                                           |       |       |    ___     |     *
*                            --row_A------- 1 ----- 2 ----- 3 --|___|----|     *
*                            --row_B------- 4 ----- 5 ----- 6 --|___|----|     *
*                            --row_C------- 7 ----- 8 ----- 9 --|___|----|     *
*                            --row_D------- * ----- 0 ----- # --|___|----|     *
+                                           |       |       |  4 x 10k         *
*                                                                              *
*******************************************************************************/
unsigned char Digitos[11], digito, Key_Ok;
char kscan = 1; //Controle de multiplexação de varredura do teclado
char Tecla = 0; //Variável que memoriza tecla pressionada (disp. LCD)
char cnt = 0; //Variável que conta ciclos de 20ms para varredura de teclado

//Variável global para converter as teclas pressionadas em seus respectivos códigos ASCII
unsigned char Tab_ASCII[12]={'0', '1', '2', '3',
                             '4', '5', '6', '7',
                             '8', '9', '*', '#'};

//Função que realiza a varredura do teclado matricial do tipo 3x4 no Atmega328p, solicitada por interrupção de timer a cada 20mS
void Scan_KBD(void){
  TIMSK0.TOIE0 = 0; //Desliga o Timer0
  Tecla = 0; //Limpa variável Tecla para display LCD
  delay_us(500);
  Key_Ok = Tecla;
  if(col_1 && kscan == 1){ //Condição: coluna 1 em nível high ? kscan igual 1
    kscan = 2;
    col_1 = 0; //Apenas a coluna 1 em nível baixo
    col_2 = 1;
    col_3 = 1;
    delay_ms(20); //Debounce de Tecla pressionada
    if(!row_A) {Tecla = Tab_ASCII[1]; while(!row_A);}//Escreve '1' no display de 7 segmentos
    else if(!row_B) {Tecla = Tab_ASCII[4]; while(!row_B);}//Escreve '4' no display de 7 segmentos
    else if(!row_C) {Tecla = Tab_ASCII[7]; while(!row_C);}//Escreve '7' no display de 7 segmentos
    else if(!row_D) {Tecla = Tab_ASCII[10]; while(!row_D);}//Escreve '*' no display de 7 segmentos
  }
  else if(col_2 && kscan == 2){ //Condição: coluna 2 em nível high ? kscan igual 2
    kscan = 3;
    col_1 = 1; //Apenas a coluna 2 em nível baixo
    col_2 = 0;
    col_3 = 1;
    delay_ms(20); //Debounce de Tecla pressionada
    if(!row_A) {Tecla = Tab_ASCII[2]; while(!row_A);} //Escreve '2' no display de 7 segmentos
    else if(!row_B) {Tecla = Tab_ASCII[5]; while(!row_B);} //Escreve '5' no display de 7 segmentos
    else if(!row_C) {Tecla = Tab_ASCII[8]; while(!row_C);} //Escreve '8' no display de 7 segmentos
    else if(!row_D) {Tecla = Tab_ASCII[0]; while(!row_D);} //Escreve '0' no display de 7 segmentos
  }
  else if(col_3 && kscan == 3){ //Condição: coluna 3 em nível high ? kscan igual 3
    kscan = 1;
    col_1 = 1; //Apenas a coluna 3 em nível baixo
    col_2 = 1;
    col_3 = 0;
    delay_ms(20);//Debounce de Tecla pressionada
    if(!row_A) {Tecla = Tab_ASCII[3]; while(!row_A);} //Escreve '3' no display de 7 segmentos
    else if(!row_B) {Tecla = Tab_ASCII[6]; while(!row_B);} //Escreve '6' no display de 7 segmentos
    else if(!row_C) {Tecla = Tab_ASCII[9]; while(!row_C);} //Escreve '9' no display de 7 segmentos
    else if(!row_D) {Tecla = Tab_ASCII[11]; while(!row_D);} //Escreve '#' no display de 7 segmentos
  }
  TIMSK0.TOIE0 = 1; //Liga o Timer0
  Key_Ok = Tecla;
}

//Função que memoriza o código da tecla pressionada para uso futuro
char Le_Tecla(void){
  while(Key_Ok != 0); //Para display LCD
  while(Key_Ok == 0); //Para display LCD
  Key_Ok = Tecla;
  Tecla = 0;
  return(Key_Ok);
}
//------Fim do Código de Varredura de Teclado

//Função que realiza a interrupção do timer 0 a cada 10mS, parametrizada conforme datasheet do Atmega328p
void ISR_Timer0(void) iv IVT_ADDR_TIMER0_OVF{
  TCNT0 = 5;//Recarrega o valor do timer
  //Alterna as saidas da PORTD
  cnt++;
  if(cnt >= 19){
    cnt = 0;
    Scan_KBD(); //Solicita função de varredura do teclado
  }
}

//Procedimento para ajuste inicial do HD44780 considerando que o reset interno falhou durante a energização
void Disp_4bits(void){
 unsigned char y;
 display = 0xF0;
 delay_ms (100);
 for(y = 0; y <= 3; y++){
  display = Tab_4Bit[y];
  Data7 = display.B7;
  Data6 = display.B6;
  Data5 = display.B5;
  Data4 = display.B4;
  delay_ms(2);
  RS = 0;
  EN = 1;
  delay_ms(2);
  EN = 0;
 }
 for (y=0; y<=3; y++){
  display = (Tab_Conf[y] & 0xF0);
  Data7 = display.B7;
  Data6 = display.B6;
  Data5 = display.B5;
  Data4 = display.B4;
  delay_ms(2);
  RS = 0;
  EN = 1;
  delay_ms(2);
  EN = 0;
  display = ((Tab_Conf[y]) << 4);
  Data7 = display.B7;
  Data6 = display.B6;
  Data5 = display.B5;
  Data4 = display.B4;
  delay_ms(2);
  RS = 0;
  EN = 1;
  delay_ms(2);
  EN = 0;
 }
}

void Load_GraphChar(unsigned char codigo){
 unsigned char z;
 display = ((0x40 + (codigo * 8)) & 0xF0);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms(2);
 RS = 0;
 EN = 1;
 delay_ms(2);
 EN = 0;
 display = ((0x40 + (codigo * 8)) << 4);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms(2);
 RS = 0;
 EN = 1;
 delay_ms(2);
 EN = 0;
 for (z=(codigo * 8); z<=(7 + (codigo * 8)); z++){
  display = (Tab_CGRam[z] & 0xF0);
  Data7 = display.B7;
  Data6 = display.B6;
  Data5 = display.B5;
  Data4 = display.B4;
  delay_ms(2);
  RS = 1;
  EN = 1;
  delay_ms(2);
  EN = 0;
  display = ((Tab_CGRam[z]) << 4);
  Data7 = display.B7;
  Data6 = display.B6;
  Data5 = display.B5;
  Data4 = display.B4;
  delay_ms(2);
  RS = 1;
  EN = 1;
  delay_ms(2);
  EN = 0;
 }
}

void Posi_Char(unsigned char Position){
 display = (Position & 0xF0);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms(2);
 RS = 0;
 EN = 1;
 delay_ms(2);
 EN = 0;

 display = ((Position) << 4);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms(2);
 RS = 0;
 EN = 1;
 delay_ms(2);
 EN = 0;
}

void Escreve_Char(unsigned char Num_CGRAM){
 display = (Num_CGRAM & 0xF0);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms(2);
 RS = 1;
 EN = 1;
 delay_ms(2);
 EN = 0;

 display = ((Num_CGRAM) << 4);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms(2);
 RS = 1;
 EN = 1;
 delay_ms(2);
 EN = 0;
}

void Escreve_Frase(unsigned char Local){
  unsigned char i;
 code unsigned char Message[6][18] = {" V-I   S-P   DIF ",
                                      "    C     C     C",
                                      "    F     F     F",
                                      " V-A:     C   ON ",
                                      " V-A:     C  OFF ",
                                      "Escolha Setpoint:"};
  for(i = 0; i <= (NumCol-1); i++){
    display = ((Message[Local][i]) & 0xF0);
    Data7 = display.B7;
    Data6 = display.B6;
    Data5 = display.B5;
    Data4 = display.B4;
    delay_ms(2);
    RS = 1;
    EN = 1;
    delay_ms(2);
    EN = 0;
    display = ((Message[Local][i]) << 4);
    Data7 = display.B7;
    Data6 = display.B6;
    Data5 = display.B5;
    Data4 = display.B4;
    delay_ms(2);
    RS = 1;
    EN = 1;
    delay_ms(2);
    EN = 0;
  }
}

void Blink(void){
  Posi_Char (0x0F);
}

void Cursor_Off(void){
  Posi_Char (0x0C);
}


void Config_Ports(void){
  DDRB.B0 = 1;
  DDRB.B1 = 1;
  DDRB.B2 = 1;
  DDRB.B3 = 1;
  DDRB.B4 = 1;
  DDRB.B5 = 1;
  DDRC.B0 = 0;
  DDRC.B1 = 0;
  DDRC.B2 = 0;
  DDRD.B7 = 0;
  DDRD.B0 = 1;
  DDRD.B1 = 1;
  DDRD.B4 = 1;
  PORTC.B0 = 1;
  PORTC.B1 = 1;
  PORTC.B2 = 1;
  PORTD.B7 = 1;
  PORTD.B0 = 1;
  PORTD.B1 = 1;
  PORTD.B4 = 1;
  Key_Ok = Tecla; //PORTC = 0b11111111;
  ADCSRA = 0x93;
}

int AD_Conv(unsigned char canalAD){
  static int analogH, analogL;
  ADMUX = 0x40 | canalAD;
  ADCSRA |= (1<<ADSC);
  while(!(ADCSRA&=~(1<<ADIF)));
  ADCSRA |= (1<<ADIF);
  analogL = ADCL;
  analogH = ADCH;
  analog  = (analogH<<8) | analogL;
  return analog;
}

void mostra(int h, int n, double x){
  aux1 = x/100;
  aux2 = x - 100*floor(aux1);

  Uni = aux2 - 10*floor(aux2/10);
  Dez = aux2/10;
  Cen = aux1 - 10*floor(aux1/10);

  Posi_Char(h + n);
  Escreve_Char(Cen + '0');
  Posi_Char(h + n + 1);
  Escreve_Char(Dez + '0');
  Posi_Char(h + n + 2);
  Escreve_Char(Uni + '0');
}

void Init_Timer0(void){
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
  asm sei; //Habilita int. global
}

void pid_control(double setpoint){
  error = setpoint - input;
  properr = error * kp;
  interr += error * ki * 100e-3;
  derr = (input - lasterr) * kd / 100e-3;
  lasterr = input;
  PID = properr + interr + derr;
  input = PID;
  pwm = 255*PID/1022;
}

int setpointconv(int n){
  if(setpoint[n] == '1') setpoint2[n] = 1;
  if(setpoint[n] == '2') setpoint2[n] = 2;
  if(setpoint[n] == '3') setpoint2[n] = 3;
  if(setpoint[n] == '4') setpoint2[n] = 4;
  if(setpoint[n] == '5') setpoint2[n] = 5;
  if(setpoint[n] == '6') setpoint2[n] = 6;
  if(setpoint[n] == '7') setpoint2[n] = 7;
  if(setpoint[n] == '8') setpoint2[n] = 8;
  if(setpoint[n] == '9') setpoint2[n] = 9;
}

void main(void){
  int j, i;
  i = 0;
  Config_Ports();
  Disp_4bits();
  Init_Timer0();

  while(1){
    Posi_Char(Linha1);
    Escreve_Frase(MSG6);
    for(j = 0; j < 3; j++){
      Le_Tecla();
      if(Key_Ok != 0){
        setpoint[i] = Key_Ok;
        i++;
        Posi_Char (Linha2 + 6 + i);
        Escreve_Char(Key_Ok);
        if(i == 3) goto A;
      }
    }
  }
  A:
  Posi_Char(Linha1);
  Escreve_Frase(MSG1);
  Posi_Char(Linha2);
  Escreve_Frase(MSG2);
  Posi_Char(Linha3);
  Escreve_Frase(MSG3);

  while(1){

    AD_Conv(4);
    mostra(Linha2, 0, 255*analog/1022);
    mostra(Linha3, 0, 459*analog/1022 + 32);
    if(lastanalog != analog){
      input = analog;
      error = analog;
      properr = analog;
      interr = analog;
      derr = analog;
    }
    lastanalog = analog;

    setpointconv(0);
    setpointconv(1);
    setpointconv(2);
    analog = 1022*(setpoint2[0]*100 + setpoint2[1]*10 + setpoint2[2])/255;
    mostra(Linha2, 6, 255*analog/1022);
    mostra(Linha3, 6, 459*analog/1022 + 32);
    pid_control(analog);
    if(abs(pwm - 255*analog/1022) < 100e-3){
      Posi_Char(Linha4);
      Escreve_Frase(MSG5);
      mostra(Linha4, 6, pwm);
    } else{
      Posi_Char(Linha4);
      Escreve_Frase(MSG4);
      mostra(Linha4, 6, pwm);
    }
    mostra(Linha2, 12, abs(pwm - 255*analog/1022));
    mostra(Linha3, 12, abs(9*(pwm/5 - 51*analog/1022)));
    mostra(Linha4, 6, pwm);
  }
}
