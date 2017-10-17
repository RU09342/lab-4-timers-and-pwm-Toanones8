#include <msp430.h>

unsigned int value=500;
unsigned int button=0;
unsigned int blink=0;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1DIR |= 0x01;                            // P1.0 output
  P1DIR |= BIT6;
  P1SEL |= 0x01;

  // Timer 0 Configuration
  TACTL = MC_0;// Stop the timer
  TACTL = TASSEL_1 + ID_1 + TAIE;

  CCR0 = 1000; // Count in CCR0 register
  CCR2 = value; // Count in CCR2 register
  CCTL0 |= CCIE; // Enable CCR0 interrupt
  CCTL2 |= CCIE + OUTMOD_7; // Enable CCR2 interrupt
  TACTL |= MC_1; // Start the timer continous  mode

  P1REN |= BIT1;                   // Enable internal pull-up/down resistors
    P1OUT |= BIT1;                   //Select pull-up mode for P1.3

    P1IE |= BIT1;   // P1.3 interrupt enabled
    P1IES |= BIT1;                     // P1.3 Hi/lo edge
    P1IFG &= ~BIT1;                  // P1.3 IFG cleared

while(1){
    if(button==1)
        P1OUT ^=BIT0;
    value+=100;
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
