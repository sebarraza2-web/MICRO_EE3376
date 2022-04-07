#include "msp.h"
#include <stdint.h>
#include <lcdLib.h>
#define Num_of_Results 5
volatile uint16_t A0results[Num_of_Results];
volatile uint16_t A1results[Num_of_Results];
volatile uint16_t A3results[Num_of_Results];
char buffer0[5];
char buffer1[5];
char buffer3[5];
float average0, average1, average3;
static uint8_t index;

void SysTick_timer(double x){
    SysTick->CTRL = 5;
    SysTick->VAL = 0;
    SysTick->LOAD = (x*3000000)-1;
    while(!(SysTick->CTRL & 0x00010000));
}

float voltage(int num){
    return  (3.3 * num) / (16383);
}

void average00(){
    int i;
    float sum;
    for(i=0; i<=5; i++){
        sum += A0results[i];
    }
    average0 = voltage(sum / 5);
}
void average01(){
    int i;
    float sum;
    for(i=0; i<=5; i++){
        sum += A1results[i];
    }
    average1 = voltage(sum / 5);
}
void average03(){
    int i;
    float sum;
    for(i=0; i<=5; i++){
        sum += A3results[i];
    }
    average3 = voltage(sum / 5);
}

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | // Stop watchdog timer
            WDT_A_CTL_HOLD;
    lcdInit();
    lcdClear();
    // Configure GPIO
    P5->SEL1 |= BIT5 | BIT4 |BIT2; // Enable A/D channel A0-A3
    P5->SEL0 |= BIT5 | BIT4 |BIT2;
    // Enable global interrupt
    __enable_irq();
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);// Enable ADC interrupt in NVIC module
    // Turn on ADC14, extend sampling time to avoid overflow of results
    ADC14->CTL0 = ADC14_CTL0_ON |
            ADC14_CTL0_MSC |
            ADC14_CTL0_SHP |
            ADC14_CTL0_CONSEQ_3;
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_0; // ref+=AVcc, channel = A0
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_1; // ref+=AVcc, channel = A1
    ADC14->MCTL[3] = ADC14_MCTLN_INCH_3| // ref+=AVcc, channel = A3, end seq.
            ADC14_MCTLN_EOS;
    ADC14->IER0 = ADC14_IER0_IE3; // Enable ADC14IFG.3
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk; // Wake up on exit from ISR
    // Ensures SLEEPONEXIT takes effect immediately
    __DSB();
    while(1)
    {

        // Start conversion-software trigger
        ADC14->CTL0 |= ADC14_CTL0_ENC |
                ADC14_CTL0_SC;
        __sleep();
        __no_operation(); // For debugger

    }
}
// ADC14 interrupt service routine
void ADC14_IRQHandler(void)
{
    if (ADC14->IFGR0 & ADC14_IFGR0_IFG3)
    {
        A0results[index++] = ADC14->MEM[0]; // Move A0 results, IFG is cleared
        A1results[index++] = ADC14->MEM[1]; // Move A1 results, IFG is cleared
        A3results[index++] = ADC14->MEM[3]; // Move A3 results, IFG is cleared
        //index = (index + 1) & 0x4; // Increment results index, modulo
        if(index == 5){
            average00();
            sprintf(buffer0, " %0.1f V", average0);
            lcdSetText(buffer0, 0, 0);
            average01();
            sprintf(buffer1, " %0.1f V", average1);
            lcdSetText(buffer1, 10, 0);
            average03();
            sprintf(buffer3, " %0.1f V", average3);
            lcdSetText(buffer3, 0, 1);
            SysTick_timer(1);
            index = 0;

        }
        __no_operation();
    }
}
