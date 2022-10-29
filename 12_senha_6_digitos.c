/*******************************************************************************
*            Programa de referência para os alunos da Escola Liceu             *
*                      de Artes e Ofícios de São Paulo                         *
*                Algorítmo de Configuração de um Display LCD.                  *
*                     Autor: Milton Barreiro Junior                            *
*                      Data: 10 outubro de 2022                                *
*                                                                              *
*        Este exemplo tem por objetivo implementar um algorítmo de configuração*
* de um display LCD com barramento de dados em 8 bits.                         *
*******************************************************************************/

/*******************************************************************************
*     As diretivas abaixo estão relacionadas à conexão do hardware do LCD      *
*******************************************************************************/
#define  Data7     PORTB.B0  //Porto de conexão D7 do LCD
#define  Data6     PORTB.B1  //Porto de conexão D6 do LCD
#define  Data5     PORTB.B2  //Porto de conexão D5 do LCD
#define  Data4     PORTB.B3  //Porto de conexão D4 do LCD

#define  RS        PORTB.B4  //Bit RW do LCD
#define  EN        PORTB.B5  //Bit EN do LCD

/*******************************************************************************
*     As diretivas abaixo estão relacionadas ao controle de posicionamento de  *
* caracteres e strings no LCD.                                                 *
*******************************************************************************/
#define  NumCol    16       //Número da última coluna do LCD utilizado (16x2)
#define  Linha1    0x80     //1º endereço linha 1 do LCD
#define  Linha2    0xC0     //1º endereço linha 2 do LCD
#define  Linha3    0x94     //1º endereço linha 3 do LCD
#define  Linha4    0xD4     //1º endereço linha 4 do LCD
#define  MSG1      0        //Código da 1ª Mensagem
#define  MSG2      1        //Código da 2ª Mensagem
#define  MSG3      2        //Código da 3ª Mensagem
#define  MSG4      3        //Código da 4ª Mensagem
#define  MSG5      4        //Código da 5ª Mensagem
#define  MSG6      5        //Código da 6ª Mensagem
#define  MSG7      6        //Código da 7ª Mensagem
#define  MSG8      7        //Código da 8ª Mensagem
#define  MSG9      8        //Código da 9ª Mensagem
#define  MSG10     9        //Código da 10ª Mensagem

unsigned char Digitos[11], digito;

//------------------------- Aqui se inicia o código de varredura de teclado
#define col_1   PORTD.B4
#define col_2   PORTD.B0
#define col_3   PORTD.B1
#define row_A   PINC.B2
#define row_B   PINC.B1
#define row_C   PINC.B0
#define row_D   PIND.B7
/*******************************************************************************
*                                                                              *
*  A conexão do teclado, é                 col_1   col_2   col_3       +5Vcc   *
* mostrada na figura ao lado.               |       |       |            |     *
*                                           |       |       |    ___     |     *
*                            --row_A------- 1 ----- 2 ----- 3 --|___|----|     *
*                            --row_B------- 4 ----- 5 ----- 6 --|___|----|     *
*                            --row_C------- 7 ----- 8 ----- 9 --|___|----|     *
*                            --row_D------- * ----- 0 ----- # --|___|----|     *
+                                           |       |       |  4 x 10k         *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
*       Abaixo estão declaradas as variáveis globais que serão utilizadas.     *
*******************************************************************************/
char kscan = 1; //Controle de multiplexação de varredura do teclado
char Tecla = 0; //Variável que memoriza tecla pressionada (disp. LCD)
char cnt = 0;   //Variável que conta ciclos de 20ms para varredura de teclado
unsigned char Key_Ok;

/*******************************************************************************
*       Abaixo estão declaradas as variáveis globais que serão convertem as te-*
* clas pressionadas em seus respectivos códigos ASCII.                         *
*******************************************************************************/
unsigned char Tab_ASCII[12]={'0', '1', '2', '3',
                             '4', '5', '6', '7',
                             '8', '9', '*', '#'};

/*******************************************************************************
*       A função a seguir realiza a varredura do teclado matricial do tipo 3x4 *
* no Atmega328p, solicitada por interrupção de timer a cada 20mS.              *
*******************************************************************************/
void Scan_KBD (void)
{
 TIMSK0.TOIE0 = 0; // Desliga o Timer0

 Tecla = 0;        // Limpa variável Tecla para display LCD

 delay_us(500);

 Key_Ok = Tecla;

 if(col_1 && kscan == 1) //Coluna 1 em nível high? kscan igual 1?
 {                       //Sim...
  kscan = 2;
  col_1 = 0;             //Apenas a coluna 1 em nível baixo
  col_2 = 1;
  col_3 = 1;
  delay_ms(20);          //Debounce de Tecla pressionada

  if(!row_A)      {Tecla = Tab_ASCII[1]; while(!row_A);} //Escreve '1' no display de 7 segmentos
  else if(!row_B) {Tecla = Tab_ASCII[4]; while(!row_B);} //Escreve '4' no display de 7 segmentos
  else if(!row_C) {Tecla = Tab_ASCII[7]; while(!row_C);} //Escreve '7' no display de 7 segmentos
  else if(!row_D) {Tecla = Tab_ASCII[10];while(!row_D);} //Escreve '*' no display de 7 segmentos
 }

       else if(col_2 && kscan == 2) //Coluna 2 em nível high? kscan igual 2?
       {                            //Sim...
        kscan = 3;
        col_1 = 1;                  //Apenas a coluna 2 em nível baixo
        col_2 = 0;
        col_3 = 1;
        delay_ms(20);               //Debounce de Tecla pressionada

        if(!row_A)      {Tecla = Tab_ASCII[2]; while(!row_A);} //Escreve '2' no display de 7 segmentos
        else if(!row_B) {Tecla = Tab_ASCII[5]; while(!row_B);} //Escreve '5' no display de 7 segmentos
        else if(!row_C) {Tecla = Tab_ASCII[8]; while(!row_C);} //Escreve '8' no display de 7 segmentos
        else if(!row_D) {Tecla = Tab_ASCII[0]; while(!row_D);} //Escreve '0' no display de 7 segmentos
       }

             else if(col_3 && kscan == 3) //Coluna 3 em nível high? kscan igual 3?
             {                            //Sim...
              kscan = 1;
              col_1 = 1;                  //Apenas a coluna 3 em nível baixo
              col_2 = 1;
              col_3 = 0;
              delay_ms(20);               //Debounce de Tecla pressionada

              if(!row_A)      {Tecla = Tab_ASCII[3]; while(!row_A);} //Escreve '3' no display de 7 segmentos
              else if(!row_B) {Tecla = Tab_ASCII[6]; while(!row_B);} //Escreve '6' no display de 7 segmentos
              else if(!row_C) {Tecla = Tab_ASCII[9]; while(!row_C);} //Escreve '9' no display de 7 segmentos
              else if(!row_D) {Tecla = Tab_ASCII[11];while(!row_D);} //Escreve '#' no display de 7 segmentos
             }

 TIMSK0.TOIE0 = 1; // Liga o Timer0
 Key_Ok = Tecla;
}

// A função abaixo, memoriza o código da tecla pressionada para uso futuro
char Le_Tecla (void)
{
 while (Key_Ok != 0);        // Para display LCD
 while (Key_Ok == 0);        // Para display LCD

 Key_Ok = Tecla;
 Tecla = 0;
 return (Key_Ok);
}
//------------------------- Aqui se finaliza o código de varredura de teclado

/*******************************************************************************
*       A função abaixo realiza a interrupção do timer 0 a cada 10mS, parame-  *
* trizada conforme datasheet do Atmega328p.                                    *
*******************************************************************************/
void ISR_Timer0 (void) iv IVT_ADDR_TIMER0_OVF
{
    //Recarrega o valor do timer
    TCNT0 = 5;
    //Alterna as saidas da PORTD
    cnt++;
    if (cnt >= 19)
    {
     cnt = 0;
     Scan_KBD();              //Solicita função de varredura do teclado
    }
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

/*******************************************************************************
*     As diretivas abaixo estão relacionadas aos códigos dos caracteres especi-*
* ais a serem carregados no LCD.                                               *
*******************************************************************************/
#define  Grau_C    0x00     //Código Grau Célsius
#define  Seta_E    0x01     //Código Seta para Esquerda
#define  Seta_D    0x02     //Código Seta para Direita
#define  Seta_C    0x03     //Código Seta para Cima
#define  Seta_B    0x04     //Código Seta para Baixo
#define  Grau_F    0x05     //Código Grau Fahrenheit

/*******************************************************************************
*     As variáveis abaixo estão relacionadas as funções a serem executadas pela*
*aplicação proposta.                                                           *
*******************************************************************************/
//Tabela de valores correspondentes aos números amostrados nos displays de 7 seg
/*******************************************************************************
*       Abaixo estão declaradas as variáveis globais que serão convertem as te-*
* clas pressionadas em seus respectivos códigos 7 segmentos Anodo Comum.       *
*******************************************************************************/
unsigned char Tab_Disp[12]={0x03, 0x9F, 0x25, 0x0D,
                            0x99, 0x49, 0x41, 0x1F,
                            0x01, 0x09, 0x11, 0xC1};


unsigned char Uni, Dez, display;

/*******************************************************************************
*   A tabela abaixo contem caracteres especiais a serem carregados na CGRAM do *
* display LCD, conforme solicitação via software.                              *
*******************************************************************************/
unsigned char
Tab_CGRam[40] = {0x18, 0x18, 0x06, 0x09, 0x08, 0x09, 0x06, 0x00, //Graus Célsius
                 0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00, //Seta Esquerda
                 0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00, //Seta Direita
                 0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00, //Seta Cima
                 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00};//Seta Baixo

/*******************************************************************************
*      A tabela abaixo contem os parâmetros de configuração inicial do display *
* LCD 4 Bits, a qual deverá ser executada após configuração prévia em 8 Bits.  *
*******************************************************************************/
unsigned char Tab_Conf[4] = {0x28,
                             0x06,
                             0x01,
                             0x0C};


/*******************************************************************************
*      A tabela abaixo contem os parâmetros de configuração inicial do display *
* LCD em 4 bits, a qual deverá ser executada uma única vez no inicio.          *
*******************************************************************************/
unsigned char Tab_4Bit[4] = {0x30,
                             0x30,
                             0x30,
                             0x20};

/*******************************************************************************
*      Procedimento para ajuste inicial do HD44780 considerando que o reset    *
* interno falhou durante a energização. Assim, deve-se enviar três vezes o     *
* comando para configurar a interface para 8 bits (em intervalos conforme o    *
* código abaixo) para então configurá-la para 4 bits.          *
*******************************************************************************/
void Disp_4bits (void)
{
 unsigned char y;
 display = 0xF0;
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms (100);

 for (y=0; y<=3; y++)
    {
      display = Tab_4Bit[y];
      Data7 = display.B7;
      Data6 = display.B6;
      Data5 = display.B5;
      Data4 = display.B4;
      delay_ms (2);
      RS = 0;
      EN = 1;
      delay_ms (2);
      EN = 0;
    }

    for (y=0; y<=3; y++)
    {
/*******************************************************************************
*     Função de configuração do LCD, conectado com via de dados de 8 bits, e   *
* matriz do tipo 7x5 pixels.                                                   *
*******************************************************************************/
/*******************************************************************************
*     Escreve o nibble mais significativo do barramento de dados do LCD.       *
*******************************************************************************/
      display = (Tab_Conf[y] & 0xF0);
      Data7 = display.B7;
      Data6 = display.B6;
      Data5 = display.B5;
      Data4 = display.B4;
      delay_ms (2);
      RS = 0;
      EN = 1;
      delay_ms (2);
      EN = 0;

/*******************************************************************************
*     Escreve o nibble menos significativo do barramento de dados do LCD.      *
*******************************************************************************/
      display = ((Tab_Conf[y]) << 4);
      Data7 = display.B7;
      Data6 = display.B6;
      Data5 = display.B5;
      Data4 = display.B4;
      delay_ms (2);
      RS = 0;
      EN = 1;
      delay_ms (2);
      EN = 0;
    }
}

/*******************************************************************************
*     Função que carrega caracteres especiais na CGRAM do LCD, quando solici-  *
* tadas por software.                                                          *
*******************************************************************************/
void Load_GraphChar (unsigned char codigo)
{
 unsigned char z;
 display = ((0x40 + (codigo * 8)) & 0xF0);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms (2);
 RS = 0;
 EN = 1;
 delay_ms (2);
 EN = 0;

 display = ((0x40 + (codigo * 8)) << 4);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms (2);
 RS = 0;
 EN = 1;
 delay_ms (2);
 EN = 0;

 for (z=(codigo * 8); z<=(7 + (codigo * 8)); z++)
  {
   display = (Tab_CGRam[z] & 0xF0);
   Data7 = display.B7;
   Data6 = display.B6;
   Data5 = display.B5;
   Data4 = display.B4;
   delay_ms (2);
   RS = 1;
   EN = 1;
   delay_ms (2);
   EN = 0;

   display = ((Tab_CGRam[z]) << 4);
   Data7 = display.B7;
   Data6 = display.B6;
   Data5 = display.B5;
   Data4 = display.B4;
   delay_ms (2);
   RS = 1;
   EN = 1;
   delay_ms (2);
   EN = 0;
  }
}

/*******************************************************************************
*     Função que posiciona caractere a ser escrito no LCD, em endereço especi- *
* ficado por software.                                                         *
*******************************************************************************/
void Posi_Char (unsigned char Position)
{
 display = (Position & 0xF0);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms (2);
 RS = 0;
 EN = 1;
 delay_ms (2);
 EN = 0;

 display = ((Position) << 4);
 Data7 = display.B7;
 Data6 = display.B6;
 Data5 = display.B5;
 Data4 = display.B4;
 delay_ms (2);
 RS = 0;
 EN = 1;
 delay_ms (2);
 EN = 0;
}

/*******************************************************************************
*     Função que escreve caractere a ser escrito no LCD, em endereço especi-   *
* ficado por software.                                                         *
*******************************************************************************/
void Escreve_Char (unsigned char Num_CGRAM)
{
  display = (Num_CGRAM & 0xF0);
  Data7 = display.B7;
  Data6 = display.B6;
  Data5 = display.B5;
  Data4 = display.B4;
  delay_ms (2);
  RS = 1;
  EN = 1;
  delay_ms (2);
  EN = 0;

  display = ((Num_CGRAM) << 4);
  Data7 = display.B7;
  Data6 = display.B6;
  Data5 = display.B5;
  Data4 = display.B4;
  delay_ms (2);
  RS = 1;
  EN = 1;
  delay_ms (2);
  EN = 0;
}

/*******************************************************************************
*     Função que escreve string no LCD, em endereço especificado por software. *
*******************************************************************************/
void Escreve_Frase (unsigned char Local)
{
 unsigned char i;
//                                      0123456789ABCDEF
 code unsigned char Message[10][17] = {"  Liceu --- SP  ",  //Mensagem MSG1
                                       "KIT -Atmega328p-",  //Mensagem MSG2
                                       " -Teste da CPU- ",  //Mensagem MSG3
                                       "Em funcionamento",  //Mensagem MSG4
                                       "Teste do Teclado",  //Mensagem MSG5
                                       "    Matricial   ",  //Mensagem MSG6
                                       "Digite Sua Senha",  //Mensagem MSG7
                                       "De Seis Digitos:",  //Mensagem MSG8
                                       " Aguardando...  ",  //Mensagem MSG9
                                       ">>>>>      <<<<<"}; //Mensagem MSG10
 for (i=0; i<=(NumCol-1); i++)
  {
   display = ((Message[Local][i]) & 0xF0);
   Data7 = display.B7;
   Data6 = display.B6;
   Data5 = display.B5;
   Data4 = display.B4;
   delay_ms (2);
   RS = 1;
   EN = 1;
   delay_ms (2);
   EN = 0;

   display = ((Message[Local][i]) << 4);
   Data7 = display.B7;
   Data6 = display.B6;
   Data5 = display.B5;
   Data4 = display.B4;
   delay_ms (2);
   RS = 1;
   EN = 1;
   delay_ms (2);
   EN = 0;
  }
}

/*******************************************************************************
*   Função que habilita o cursor piscando no LCD para indicar posição de ajuste*
* de horário quando solicitada.                                                *
*******************************************************************************/
void Blink (void)
{
 Posi_Char (0x0F);
}

/*******************************************************************************
*    Função que desabilita o cursor do LCD após uso de indicação de posição de *
* ajuste de horário quando solicitada.                                         *
*******************************************************************************/
void Cursor_Off (void)
{
 Posi_Char (0x0C);
}

/*******************************************************************************
*     Função que configura os PORT´s do microcontrolador quanto à sua aplicação*
* (seja entrada ou saída).                                                     *
*******************************************************************************/
void Config_Ports (void)
{
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

 //PORTC = 0b11111111;
 Key_Ok = Tecla;
}

int main (void)
{
 unsigned char Senha[6], j;
 int Senha_Real[6], n, k, i, m;
 i = 0;
 m = 0;

 DDRD.B6 = 1;
 OCR0A = 0;

 for(n = 0; n < 6; n++){
  Senha_Real[n] = n;
 }

 Config_Ports ( );          // Configura os portos de comunicação
                            //e direção da informação
 Disp_4bits ( );            //Configura display LCD no modo 8 bits
 /*
 Posi_Char (Linha1);
 Escreve_Frase (MSG1);
 Posi_Char (Linha2);
 Escreve_Frase (MSG2);
 delay_ms (2000);

 Posi_Char (Linha1);
 Escreve_Frase (MSG3);
 Posi_Char (Linha2);
 Escreve_Frase (MSG4);
 delay_ms (2000);

 Posi_Char (Linha1);
 Escreve_Frase (MSG5);
 Posi_Char (Linha2);
 Escreve_Frase (MSG6);
 delay_ms (2000);

 Posi_Char (Linha1);
 Escreve_Frase (MSG7);
 Posi_Char (Linha2);
 Escreve_Frase (MSG8);
 delay_ms (2000);

 Posi_Char (Linha1);
 Escreve_Frase (MSG9);
 Posi_Char (Linha2);
 Escreve_Frase (MSG10);
 delay_ms (2000);
 */
 Init_Timer0( );

 while (1)
 {
  Posi_Char (Linha2);
  Escreve_Frase (MSG10);

  for (j = 0; j < 6; j++)
  {
   Le_Tecla( );
   if(Key_Ok != 0){
   i++;
   Posi_Char (Linha2+4+i);
   Escreve_Char ('*');
   i = i%6;
   }
   }
  }

  if(m == 6){
  OCR0A = 1;
  }

  delay_ms(500);
  Posi_Char (Linha2);
  Escreve_Frase (MSG10);
 }
