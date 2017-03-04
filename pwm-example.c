#include <msp430.h> 

/*
 * main.c
 */

#define PERIODE 500 // now 500Hz, calculate: (f_Timer = 500kHz / f_out_des) / 2

void clockInit(void) {
	/* Setting clocks to:
	 * MCLK = SMCLK = 1MHz
	 * ACLK = 12kHz
	 */
    DCOCTL = CALDCO_1MHZ;		// set DCO to 1MHz
    BCSCTL1 = CALBC1_1MHZ;		// set DCO Range Sel correctly
    BCSCTL1 |= DIVA_0;			// ACLK-divider off
    BCSCTL2 = SELM_0 + DIVM_0 + DIVS_0; // MCLK <- DCO, MCLK divider and SMCLK divider off
    BCSCTL3 &= ~XTS;			// source ACKL from internal VLO @ 12kHz / divider
    BCSCTL3 = LFXT1S_2;
}

void timerInit(void){
	/* Timer runs @ 500kHz */
	TA0CTL = TASSEL_2 + ID_1 + MC_0; // SMCLK is source, divider /2, timer halted, periode interrupt off
	TA0CCTL1 = /* compare mode */ OUTMOD_3 ;	// Toggle/Reset mode
	TA0CCR0 = PERIODE;	// sets timer max, periode
	TA0CCR1 = PERIODE - (PERIODE * .8); // sets toggle point = duty cycle
	TA0CTL |= MC_3; // timer running in up/down mode
}

#pragma vector=PORT1_VECTOR // Button interrupt
__interrupt void PORT1_ISR(void)
{
  P1IFG = 0;
  P1IE &= ~BIT3;            /* Debounce */
  WDTCTL = WDT_ADLY_250;
  IFG1 &= ~WDTIFG;                 /* clear interrupt flag */
  IE1 |= WDTIE;

  // toggle P1.2 between full on and PWM
  P1SEL ^= BIT2;
}

#pragma vector=WDT_VECTOR	// Debounce using watchdog
__interrupt void WDT_ISR(void)
{
    IE1 &= ~WDTIE;                   /* disable interrupt */
    IFG1 &= ~WDTIFG;                 /* clear interrupt flag */
    WDTCTL = WDTPW + WDTHOLD;        /* put WDT back in hold state */
    P1IE |= BIT3;             /* Debouncing complete */
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    clockInit();
    timerInit();
    __enable_interrupt();

    // set PWM output
    P1DIR |= BIT2;
    //P1SEL |= BIT2;

    // initalize on/off button
    P1OUT = BIT2 + BIT3;
    P1REN |= BIT3;
    P1IES |= BIT3;
    P1IFG &= ~BIT3;
    P1IE |= BIT3;


    while(1);
	return 0;
}
