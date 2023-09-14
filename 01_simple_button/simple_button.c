#define BT PIND.B2 // sets the button to pin D2
#define LED PORTD.B5 // sets the LED to pin D5
#define ON 1
#define OFF 0

void main (void)
{
DDRD.B2 = 0; // sets pin D2 as input
DDRD.B5 = 1; // sets pin D5 as output
LED = OFF; // LED initial value = off

// main while loop
while (1)
{
  while(BT == 1)
  {
    LED = ON;
  }
  while(BT == 0)
  {
    LED = OFF;
  }
}
}