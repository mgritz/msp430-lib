#include <msp430.h> 
#include <math.h>

#include "HD44780dispDriver.h"
#include "ADCTempLUT.h"
#include "uart.h"

#define NUM_SAMPLES 128

volatile float temperature;
volatile float millivolts;
volatile char once, updateLCM, sendToUART;
volatile int samples[NUM_SAMPLES];
volatile int tempAvg;

volatile char queuePosition;


void clockInit(void) {
	/* Setting clocks to:
	 * MCLK = SMCLK = 8MHz
	 * ACLK = 12kHz
	 */
    DCOCTL = CALDCO_8MHZ;		// set DCO to 8MHz
    BCSCTL1 = CALBC1_8MHZ;		// set DCO Range Sel correctly
    BCSCTL1 |= DIVA_0;			// ACLK-divider off
    BCSCTL2 = SELM_0 + DIVM_0 + DIVS_0; // MCLK <- DCO, MCLK divider and SMCLK divider off
    BCSCTL3 &= ~XTS;			// source ACKL from internal VLO @ 12kHz / divider
    BCSCTL3 = LFXT1S_2;
}

void adcInit(void) {
	/* setup ADC */
	P1SEL |= BIT3;										// ADC input pin P1.3
	ADC10CTL1 = INCH_3 + ADC10SSEL_1 + ADC10DIV_2 + CONSEQ_2;
		// Channel 3, ADC10CLK <- ACLK, divider /4, single channel repeated conv.
	ADC10CTL0 = SREF_1 + ADC10SHT_0 + MSC + ADC10ON + REFON + ADC10IE;
		// 1.5V & Vss as reference, Sample and hold for 64 Clock cycles, ASAP resampling, ADC on, ADC interrupt enable
	ADC10AE0 |= BIT3;

	__delay_cycles(1000);				// Wait for ADC Ref to settle
	ADC10CTL0 |= ENC + ADC10SC;			// Sampling and conversion start
}

void timerInit(void){
	/* Timer runs @ 6kHz */
	TA0CTL = TASSEL_1 + ID_1 + TAIE; // ACLK is source, divider /2, periodic interrupt on
	TA0CCR0 = 6000;	// generate a TAIFG (timer overflow interrupt) every second.
	TA0CTL |= MC_1; // timer running in up mode
}

/* on timer A overflow, trigger display and UART event on next main loop */
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A (void)
{
	if (once) {
		updateLCM = 1;
		sendToUART = 1;
	}
	TA0CTL &= ~TAIFG;
}

/* ADC10 interrupt service routine */
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
	long long rawValue = ADC10MEM;
	if (rawValue > 218 && rawValue < 615) {
		samples[queuePosition] = tempLUT[(int) rawValue - 218] + TEMP_OFFSET;
		queuePosition++;
		if (queuePosition == NUM_SAMPLES) {
			queuePosition = 0;
			once = 1;
		}
	}
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	clockInit();

	/* initialize */
    int i = 0;
    tempAvg = 0;
    for(i=0; i<NUM_SAMPLES; i++) samples[i] = 0;
	once = 0;
	updateLCM = 0;
	sendToUART = 0;
	queuePosition = 0;

	InitializeLcm();
	ClearLcmScreen();
//	UART_Init();

	/* get ADC and timer ready */
	adcInit();
	timerInit();

	__bis_SR_register(GIE);

	/* event loop */
	while (1) {
		/* update display */
		if (updateLCM) {
			updateLCM = 0;
			tempAvg = 0;
			for (i = 0; i < NUM_SAMPLES; i++)
				tempAvg += samples[i];
			tempAvg /= NUM_SAMPLES;

			ClearLcmScreen();
			char string[8] = "   degC ";
			tempString(tempAvg, string);
			PrintStr(string);
		}

		/* send to UART */
		if (sendToUART) {
			sendToUART = 0;
			//UartPutChar('x');
		}
	}
	return 0;
}
