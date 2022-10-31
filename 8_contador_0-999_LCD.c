#define BT0 PIND.B2
#define BT1 PIND.B3
#define BT2 PIND.B4
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

unsigned char Tab_Disp[10] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09},
Tab_CGRam[40] = {0x18, 0x18, 0x06, 0x09, 0x08, 0x09, 0x06, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00,
0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00, 0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00},
Tab_Conf[4] = {0x28, 0x06, 0x01, 0x0C},
Tab_4Bit[4] = {0x30, 0x30, 0x30, 0x20},
Uni, Dez, display;

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
  code unsigned char Message[2][17] = {"Contador: 0-999 ", "   Valor:       "};
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
  DDRD.B2 = 0;
  DDRD.B3 = 0;
}

int mostra(int i){
  Posi_Char(Linha2 + 10);
  Escreve_Char(48 + ((i / 100) % 10));
  Posi_Char(Linha2 + 11);
  Escreve_Char(48 + ((i / 10) % 10));
  Posi_Char(Linha2 + 12);
  Escreve_Char((i % 10) + 48);
}

void main(void){
  int i = 0;
  Config_Ports();
  Disp_4bits();
  PORTB.B0 = 0;
  PORTB.B1 = 0;
  Posi_Char(Linha1);
  Escreve_Frase(MSG1);
  Posi_Char(Linha2);
  Escreve_Frase(MSG2);
  mostra(i);
  while(1){
    if(BT0 == 1){
      i++;
      mostra(i);
      while(BT0 == 1) mostra(i);
    }
    if(BT1 == 1){
      i--;
      if(i < 0) i = 999;
      mostra(i);
      while(BT1 == 1) mostra(i);
    }
    if(BT2 == 1){
      i = 0;
      mostra(i);
      while(BT2 == 1) mostra(i);
    }
  }
}
