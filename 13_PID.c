#define  Data7     PORTB.B0
#define  Data6     PORTB.B1
#define  Data5     PORTB.B2
#define  Data4     PORTB.B3

#define  RS        PORTB.B4
#define  EN        PORTB.B5

#define  NumCol    16
#define  Linha1    0x80
#define  Linha2    0xC0
#define  MSG1      0
#define  MSG2      1

unsigned char Uni, Dez, Cen, display;

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
kp = 0.5, ki = 0.3, kd = 0.05, ideal_value = 512.0;

int pwm = 128;

unsigned char
Tab_CGRam[40] = {0x18, 0x18, 0x06, 0x09, 0x08, 0x09, 0x06, 0x00,
                 0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01, 0x00,
                 0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08, 0x00,
                 0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00},
                 
Tab_Conf[4] = {0x28,
               0x06,
               0x01,
               0x0C},
               
Tab_4Bit[4] = {0x30,
               0x30,
               0x30,
               0x20};

void Disp_4bits (void)
{
 unsigned char y;
 display = 0xF0;
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

void Escreve_Frase (unsigned char Local)
{
 unsigned char i;

 code unsigned char Message[2][17] = {" S-P  V-A  PID  ",
                                       "              % "};
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

void Config_Ports (void)
{
 DDRD.B6 = 1;
 PORTD.B6 = 0;

 DDRD.B5 = 1;

 DDRB.B0 = 1;
 DDRB.B1 = 1;
 DDRB.B2 = 1;
 DDRB.B3 = 1;

 DDRB.B4 = 1;
 DDRB.B5 = 1;

 ADCSRA  = 0x93;
}

int AD_Conv (unsigned char canalAD)
{
   static int analogH, analogL;

   ADMUX   = 0x40 | canalAD;

   ADCSRA |= (1<<ADSC);

   while(!(ADCSRA&=~(1<<ADIF)));

   ADCSRA |= (1<<ADIF);

   analogL = ADCL;
   analogH = ADCH;

   analog  = (analogH<<8) | analogL;

   return analog;
}

void mostra (int n, double x){
 aux1 = x/100;
 aux2 = x - 100*floor(aux1);

 Uni = aux2 - 10*floor(aux2/10);
 Dez = aux2/10;
 Cen = aux1 - 10*floor(aux1/10);
 
 Posi_Char(Linha2 + n);
 Escreve_Char(Cen + '0');
 Posi_Char(Linha2 + n + 1);
 Escreve_Char(Dez + '0');
 Posi_Char(Linha2 + n + 2);
 Escreve_Char(Uni + '0');
 Posi_Char(Linha2 + n + 3);
}

void Init_Timer0 (void)
{
    WGM00_Bit = 1;
    WGM01_Bit = 1;
    WGM02_Bit = 0;
    COM0A1_Bit = 1;
    COM0A0_Bit = 0;
    TCCR0B.CS00 = 1;
    TCCR0B.CS01 = 1;
    TCCR0B.CS02 = 0;
}

void pid_control(double setpoint)
{
    error = setpoint - input;

    properr = error * kp;

    interr += error * ki;

    derr = (input - lasterr) * kd;
    
    lasterr = input;

    PID = properr + interr + derr;
    input = PID;

    if(PID >= analog) pwm = 100*setpoint/PID;
    else pwm = 100*PID/setpoint;
}

void main (void)
{
 Config_Ports ( );
 Disp_4bits ( );

 Init_Timer0();

 Posi_Char (Linha1);
 Escreve_Frase (MSG1);
 Posi_Char (Linha2);
 Escreve_Frase (MSG2);

 while (1)
 {
  AD_Conv(4);
  mostra(1, 255*analog/1022);
  if(lastanalog == analog){
  } else{
  AD_Conv(4);
  input = analog;
  }
  lastanalog = analog;

  AD_Conv(5);
  mostra(6, 255*analog/1022);
  
  pid_control(analog);
  mostra(11, pwm);
 }
}
