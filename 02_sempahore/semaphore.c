#define Red1 PORTC.B1 // red1 = port C1
#define Yellow1 PORTC.B0 // yellow1 = port C0
#define Green1 PORTB.B3 // green1 = port B3

#define Red2 PORTB.B2 // red2 = port B2
#define Yellow2 PORTB.B1 // yellow2 = port B1
#define Green2 PORTB.B0 // green2 = port B0

/* there are two traffic lights at an intersection, the first
3 bits represent the second, and the other 3 the first semaphore */

// color sequence
unsigned char Semaforo[4] = {0b00100001, // Red, Green
                             0b00100010, // Red, Yellow
                             0b00001100, // Green, Red
                             0b00010100  // Yellow, Red
                             },
LEDs;

void turn_on_semaphore(void)
{
  Red1 = LEDs.B5;
  Yellow1 = LEDs.B4;
  Green1 = LEDs.B3;

  Red2 = LEDs.B2;
  Yellow2 = LEDs.B1;
  Green2 = LEDs.B0;
}

void main (void)
{
// set as outputs
DDRC.B1 = 1;
DDRC.B0 = 1;
DDRB.B3 = 1;
DDRB.B2 = 1;
DDRB.B1 = 1;
DDRB.B0 = 1;

LEDs = 0b11111111;
turn_on_semaphore(); // turn on all the lights of both traffic lights
delay_ms(1000); // wait 1 second before starting 

while (1)
{
  int i;
  for (i = 0; i <= 3; i++)
  {
    // keep changing semaphore after 3 seconds
    LEDs = Semaforo[i];
    turn_on_semaphore();
    delay_ms(1000);
  }
}
}