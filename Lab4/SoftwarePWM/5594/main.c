#include <msp430.h>

unsigned int value=500;
unsigned int button=0;
unsigned int blink=0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1DIR |= 0x01;                            // P1.0 output
  P1DIR |= BIT6;

  // Timer 0 Configuration
  TACTL = MC_0;// Stop the timer
  TACTL = TASSEL_1 + ID_1 + TAIE;

  CCR0 = 1000; // Count in CCR0 register
  CCR2 = value; // Count in CCR2 register
  CCTL0 |= CCIE; // Enable CCR0 interrupt
  CCTL2 |= CCIE + OUTMOD_7; // Enable CCR2 interrupt
  TACTL |= MC_1; // Start the timer continous  mode

  P5REN |= BIT6;                   // Enable internal pull-up/down resistors
    P5OUT |= BIT6;                   //Select pull-up mode for P1.3

    P5IE |= BIT6;   // P1.3 interrupt enabled
    P5IES |= BIT6;                     // P1.3 Hi/lo edge
    P5IFG &= ~BIT6;                  // P1.3 IFG cleared

while(1){
    if(blink==1)
        P1OUT ^=BIT0;
}
  _BIS_SR(LPM0_bits + GIE); // Sleep in LPM0 with interrupts enabled

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
  switch(button)       //if button is off turn on.
  {case 0: button=1;
  break;
  case 1: button=0;   //if button is on turn off
  break;
  }

}
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
   blink ^=blink;         //toggle the led.

  if(button==1)
  { value+=100;   //increase the pwm
  }
  if(button==0)     //iff button is not pressed, keep value the same
  {value=value;}
}
