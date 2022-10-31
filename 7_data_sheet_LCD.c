//Display de LCD com barramento de dados com 4 bits

//Conexão do hardware do LCD
#define Data7 PORTB.B0  //Porto de conexão D7 do LCD
#define Data6 PORTB.B1  //Porto de conexão D6 do LCD
#define Data5 PORTB.B2  //Porto de conexão D5 do LCD
#define Data4 PORTB.B3  //Porto de conexão D4 do LCD
#define RS PORTB.B4  //Bit RW do LCD
#define EN PORTB.B5  //Bit EN do LCD
#define BT1 PIND.B2
#define BT2 PIND.B3

//Controle de posicionamento de caracteres e strings no LCD
#define NumCol 16 //Número de colunas do LCD (16x2)
#define Linha1 0x80 //1º endereço linha 1 do LCD
#define Linha2 0xC0 //1º endereço linha 2 do LCD
#define Linha3 0x90 //1º endereço linha 3 do LCD
#define Linha4 0xD0 //1º endereço linha 4 do LCD
#define MSG1 0 //Código da 1ª Mensagem
#define MSG2 1 //Código da 2ª Mensagem
#define MSG3 2 //Código da 3ª Mensagem
#define MSG4 3 //Código da 4ª Mensagem
#define MSG5 4 //Código da 5ª Mensagem
#define MSG6 5 //Código da 6ª Mensagem
#define MSG7 6 //Código da 7ª Mensagem
#define MSG8 7 //Código da 8ª Mensagem
#define MSG9 8 //Código da 9ª Mensagem
#define MSG10 9 //Código da 10ª Mensagem

unsigned char display;

//Caracteres especiais a serem carregados na CGRAM do display LCD
unsigned char Tab_CGRam[40] = {0x18, 0x18, 0x06, 0x09, 0x08, 0x09, 0x06, 0x00, //°C
                               0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00, //<
                               0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00, //>
                               0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, //Seta para cima
                               0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00};//Seta para baixo

//Parâmetros de configuração inicial do display LCD 4 Bits, a qual deverá ser executada após configuração prévia em 8 Bits
unsigned char Tab_Conf[4] = {0x28,
                             0x06,
                             0x01,
                             0x0C};

//Parâmetros de configuração inicial do display LCD em 4 bits, a qual deverá ser executada uma única vez no inicio
unsigned char Tab_4Bit[4] = {0x30,
                             0x30,
                             0x30,
                             0x20};

/*Procedimento para ajuste inicial do HD44780 considerando que o reset interno falhou durante a energização. Assim, deve-se enviar três vezes o
comando para configurar a interface para 8 bits (em intervalos conforme o código abaixo) para então configurá-la para 4 bits*/
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
//Função de configuração do LCD, conectado com via de dados de 8 bits, e matriz do tipo 7x5 pixels
//Escreve o nibble mais significativo do barramento de dados do LCD
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
//Escreve o nibble menos significativo do barramento de dados do LCD
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
//Função que carrega caracteres especiais na CGRAM do LCD
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

//Função que posiciona caractere a ser escrito no LCD, em endereço especificado por software
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

//Função que escreve caractere a ser escrito no LCD, em endereço especiicado por software
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

//Função que escreve string no LCD, em endereço especificado por software
void Escreve_Frase(unsigned char Local){
 unsigned char i;
//                                      0123456789ABCDEF
 code unsigned char Message[10][17] = {" Mensagem -- 01 ",  /*Mensagem MSG1*/
                                       " Mensagem -- 02 ",  /*Mensagem MSG2*/
                                       " Mensagem -- 03 ",  /*Mensagem MSG3*/
                                       " Mensagem -- 04 ",  /*Mensagem MSG4*/
                                       " Mensagem -- 05 ",  /*Mensagem MSG5*/
                                       " Mensagem -- 06 ",  /*Mensagem MSG6*/
                                       " Mensagem -- 07 ",  /*Mensagem MSG7*/
                                       " Mensagem -- 08 ",  /*Mensagem MSG8*/
                                       " Mensagem -- 09 ",  /*Mensagem MSG9*/
                                       " Mensagem -- 10 "}; /*Mensagem MSG10*/
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

//Função que habilita o cursor piscando no LCD para indicar posição de ajuste de horário quando solicitada
void Blink(void){
 Posi_Char (0x0F);
}

//Função que desabilita o cursor do LCD após uso de indicação de posição de ajuste de horário quando solicitada
void Cursor_Off(void){
 Posi_Char (0x0C);
}

//Função que configura os PORT´s do microcontrolador quanto à sua aplicação (seja entrada ou saída)
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

//Função main para organização da chamada de funções
void main(void){
 Config_Ports(); //Configura os portos de comunicação e direção da informação
 Disp_4bits(); //Configura display LCD no modo 8 bits

 PORTB.B0 = 0;
 PORTB.B1 = 0;

 Posi_Char(Linha1); 
 Escreve_Frase(MSG1);
 Posi_Char(Linha2);
 Escreve_Frase(MSG2);
 delay_ms(2000);

 Posi_Char(Linha1);
 Escreve_Frase(MSG3);
 Posi_Char(Linha2);
 Escreve_Frase(MSG4);
 delay_ms(2000);

 Posi_Char(Linha1);
 Escreve_Frase(MSG5);
 Posi_Char(Linha2);
 Escreve_Frase(MSG6);
 delay_ms(2000);

 Posi_Char(Linha1);
 Escreve_Frase(MSG7);
 Posi_Char(Linha2);
 Escreve_Frase(MSG8);
 delay_ms(2000);

 Posi_Char(Linha1);
 Escreve_Frase(MSG9);
 Posi_Char(Linha2);
 Escreve_Frase(MSG10);
 delay_ms(2000);
 while(1){
  //Laço principal
 }
}
