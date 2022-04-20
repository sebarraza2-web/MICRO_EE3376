#include "msp.h"
#include "lcdLib_432.h"
void transmit();
void transmit1(void);
char buffer[16];
char buffer1[16];
char voltage_display[] = " Voltage: \r \n";

void SysTick_timer(double x){
    SysTick->CTRL = 5;
    SysTick->VAL = 0;
    SysTick->LOAD = (x*3000000)-1;
    while(!(SysTick->CTRL & 0x00010000));
}

void uart_init(void){
    P1->SEL0 |= BIT2 | BIT3;
    EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
    EUSCI_A0->CTLW0  = 0x00C1;
    EUSCI_A0->BRW = 312;
    EUSCI_A0->MCTLW &= ~0xFFF1;
    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);
}

int main(void) {
    volatile unsigned int i;
    uart_init();
    transmit1();
    WDT_A->CTL = WDT_A_CTL_PW | // Stop WDT
            WDT_A_CTL_HOLD;
    // GPIO Setup
    P1->OUT &= ~BIT0; // Clear LED to start
    P1->DIR |= BIT0; // Set P1.0/LED to output
    P5->SEL1 |= BIT4; // Configure P5.4 for ADC
    P5->SEL0 |= BIT4;
    lcdInit();
    lcdClear();
    // Enable global interrupt
    __enable_irq();
    // Enable ADC interrupt in NVIC module
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
    // Sampling time, S&H=16, ADC14 on
    ADC14->CTL0 = ADC14_CTL0_SHT0_2 | ADC14_CTL0_SHP | ADC14_CTL0_ON;
    ADC14->CTL1 = ADC14_CTL1_RES_2; // Use sampling timer, 12-bit conv. results
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_1; // A1 ADC input select; Vref=AVCC
    ADC14->IER0 |= ADC14_IER0_IE0; // Enable ADC conv complete interrupt
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk; // Wake up on exit from ISR
    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    while (1)
    {
        for (i = 20000; i > 0; i--); // Delay
        // Start sampling/conversion
        ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;
        __sleep();
        __no_operation(); // For debugger
    }
}
// ADC14 interrupt service routine
void ADC14_IRQHandler(void) {
        float volt = ADC14->MEM[0];
        float ans;
        ans = (3.3 * volt) / (4095);
        sprintf(buffer, " %0.1f V\r", ans);
        transmit();
        lcdSetText("Voltage:", 4, 0);
        sprintf(buffer1, " %0.1f V", ans);
        lcdSetText(buffer1, 4, 1);
}
void transmit1(void){
    int i;
    for(i = 0; i <= sizeof(voltage_display); i++){
        SysTick_timer(0.01);
        if(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG){
            EUSCI_A0->TXBUF = voltage_display[i];
        }
    }
}
void transmit(void){
    int i;
    for(i = 0; i <= sizeof(buffer); i++){
        SysTick_timer(0.01);
        if(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG){
            EUSCI_A0->TXBUF = buffer[i];
        }
    }
}


