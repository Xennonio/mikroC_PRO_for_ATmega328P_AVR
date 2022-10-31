#define Time_PID PORTD.B5
#define Data7 PORTB.B0
#define Data6 PORTB.B1
#define Data5 PORTB.B2
#define Data4 PORTB.B3
#define RS PORTB.B4
#define EN PORTB.B5
#define NumCol 16
#define Linha1 0x80
#define Linha2 0xC0
#define Linha3 0x90
#define Linha4 0xD0
#define MSG1 0
#define MSG2 1
#define MSG3 2
#define MSG4 3
#define MSG5 4
#define MSG6 5

unsigned char Tab_Disp[10] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09},
Tab_CGRam[40] = {0x18, 0x18, 0x06, 0x09, 0x08, 0x09, 0x06, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00,
0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00},
Tab_Conf[4] = {0x28, 0x06, 0x01, 0x0C},
Tab_4Bit[4] = {0x30, 0x30, 0x30, 0x20},
Uni, Dez, Cen, Dec, display, Digitos[11], digito, cnt;
double aux1, aux2, analog, F, aux1F, aux2F, DecF, CenF, DezF, UniF;
void pid_control();

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
   code unsigned char Message[6][17] = {" Liceu de Artes ", " e Oficios - SP ", "  Leitura dos   ", "  Canais 4 e 5  ", "Canal-5  Canal-4",
"                "};
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
  DDRD.B6 = 1;
  DDRD.B5 = 1;
  DDRB.B0 = 1;
  DDRB.B1 = 1;
  DDRB.B2 = 1;
  DDRB.B3 = 1;
  DDRB.B4 = 1;
  DDRB.B5 = 1;
  ADCSRA  = 0x93;
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

void converte(double x){
  aux1 = x/100;
  aux2 = x - 100*floor(aux1);

  Dec = 10*(x - floor(x));
  Uni = aux2 - (10 * floor(aux2 / 10));
  Dez = aux2 / 10;
  Cen = aux1 - (10 * floor(aux1 / 10));
  
  F = 9*x/5 + 32;
    
  aux1F = F/100;
  aux2F = F - 100*floor(F/100);

  DecF = 10*(F - floor(F));
  UniF = aux2F - 10*floor(aux2F/10);
  DezF = aux2F/10;
  CenF = aux1F - 10*floor(aux1F/10);
}

void mostra1(){
 Posi_Char(Linha2 + 0);
 Escreve_Char(Cen + '0');
 Posi_Char(Linha2 + 1);
 Escreve_Char(Dez + '0');
 Posi_Char(Linha2 + 2);
 Escreve_Char(Uni + '0');
 Posi_Char(Linha2 + 3);
 Escreve_Char(',');
 Posi_Char(Linha2 + 4);
 Escreve_Char(Dec + '0');
 Posi_Char(Linha2 + 6);
 Escreve_Char('C');
}

void mostra2(){
 Posi_Char(Linha2 + 9);
 Escreve_Char(CenF + '0');
 Posi_Char(Linha2 + 10);
 Escreve_Char(DezF + '0');
 Posi_Char(Linha2 + 11);
 Escreve_Char(UniF + '0');
 Posi_Char(Linha2 + 12);
 Escreve_Char(',');
 Posi_Char(Linha2 + 13);
 Escreve_Char(DecF + '0');
 Posi_Char(Linha2 + 15);
 Escreve_Char('F');
}

void main(void){
  Config_Ports();
  Disp_4bits();
  
  Posi_Char(Linha1);
  Escreve_Frase(MSG1);
  Posi_Char(Linha2);
  Escreve_Frase(MSG2);
  delay_ms(4000);
  
  Posi_Char(Linha1);
  Escreve_Frase(MSG3);
  Posi_Char(Linha2);
  Escreve_Frase(MSG4);
  delay_ms(4000);

  Posi_Char(Linha1);
  Escreve_Frase(MSG5);
  Posi_Char(Linha2);
  Escreve_Frase(MSG6);
  
  while(1){
  AD_Conv(5);
  converte(150*analog/1022);
  mostra1();

  AD_Conv(4);
  converte(150*analog/1022);
  mostra2();
  }
}
