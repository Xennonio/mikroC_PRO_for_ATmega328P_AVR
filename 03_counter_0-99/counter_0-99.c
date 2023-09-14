/* program to count from 0 to 99.
The first button adds 1 to the result shown on the display;
the second button subtracts 1;
and the third resets.
*/

#define a PORTC.B3 // a = C3
#define b PORTC.B2 // b = C2
#define c PORTC.B1 // c = C1
#define d PORTC.B0 // d = C0
#define e PORTB.B3 // e = B3
#define f PORTB.B2 // f = B2
#define g PORTB.B1 // g = B1

#define BT0 PIND.B2 // button0 = D2
#define BT1 PIND.B3 // button1 = D3
#define BT2 PIND.B4 // button2 = D4

// define whether we are using the first or second display
#define Led1 PORTB.B5 // led1 = B5
#define Led2 PORTB.B4 // led2 = B4

/*
7-segments display common anode inputs

  a
f   b
  g
e   c
  d
*/

// numbers from 0 to 9 on the display (inverted, since it is a common anode type)
unsigned char numbers[10] = {0b0000001, 0b1001111, 0b0010010, 0b0000110, 0b1001100, 0b0100100, 0b0100000, 0b0001111, 0b0000000, 0b0000100},
LEDs;
int count = 0;

// function to show the number n on the display
void show_display (int n)
{
  LEDs = numbers[n];

  a = LEDs.B6;
  b = LEDs.B5;
  c = LEDs.B4;
  d = LEDs.B3;
  e = LEDs.B2;
  f = LEDs.B1;
  g = LEDs.B0;
  delay_ms(10);
}

// shows the number n on the display
void switch_display (int n)
{
  // show the first number on the first display
  Led1 = 1;
  Led2 = 0;

  show_display((n / 10) % 10);

  // show the last number on the second display
  Led1 = 0;
  Led2 = 1;
  
  show_display(n % 10);
}

// interrupt the while loop if the button is still pressed
void interrupt (int count)
{
  while (BT0 == 1 || BT1 == 1 || BT2 == 1)
  {
    switch_display(count);
  }
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
  
  // set no display to use
  Led1 = 0;
  Led2 = 0;
  
  while (1)
  {
    // if button 1 is pressed, add 1 to counter
    if(BT0 == 1)
    {
      count++;
      interrupt(count);
    }
    
    // if button 2 is pressed and the counter is not 0, subtract 1 from the counter, otherwise change it to 99
    if(BT1 == 1)
    {
      count--;
      if(count < 0)
      {
        count = 99;
      }
      interrupt(count);
    }
    
    // if button 3 is pressed change the counter to 0
    if(BT2 == 1)
    {
      count = 0;
      interrupt(count);
    }
    
    // show count on display
    switch_display(count);

    continue;
  }
}