#include "msp.h"
#include "lcdLib_432.h"
#include "stdlib.h"

int sine_mode, random_mode, random_amp_mode;
int stop_state;
int i;
int samples = 50;

void display(){
    TIMER_A0->CCR[1] = i;
    delay_ms(100);
    TIMER_A0->CCR[2] = i;
    delay_ms(100);
    TIMER_A0->CCR[3] = i;
    delay_ms(100);
    TIMER_A0->CCR[4] = i;
    delay_ms(100);
    TIMER_A1->CCR[1] = i;
    delay_ms(100);
}

void default_mode(){
    i = 0;
    TIMER_A0->CCR[1] = i;
    TIMER_A0->CCR[2] = i;
    TIMER_A0->CCR[3] = i;
    TIMER_A0->CCR[4] = i;
    TIMER_A1->CCR[1] = i;
    delay_ms(1000);
    i = 1000;
    TIMER_A0->CCR[1] = i;
    TIMER_A0->CCR[2] = i;
    TIMER_A0->CCR[3] = i;
    TIMER_A0->CCR[4] = i;
    TIMER_A1->CCR[1] = i;
    delay_ms(1000);
}
void sine(){
    if (sine_mode == 1){
        lcdSetText("Sine Mode", 0, 0);
        for(i = 0; i <= 1000; i+=samples){
            display();
            delay_ms(1);
        }
        for(i = 1000; i >= 0;i-=samples){
            display();
            delay_ms(1);
        }
    }
}

void random(){
    if (random_mode==1){
        lcdSetText("Random Mode", 0, 0);
    TIMER_A0->CCR[1] = (rand() % 900) + 1;
    TIMER_A0->CCR[2] =  (rand() % 500) + 1;
    TIMER_A0->CCR[3] = (rand() % 400) + 1;
    TIMER_A0->CCR[4] =  (rand() % 300) + 1;
    TIMER_A1->CCR[1] = (rand() % 200) + 1;
    delay_ms(1500);
    }
}
void random_amp(){
    if (random_amp_mode==1){

    lcdClear();
    int yes =  ((rand() % 700)+1);
    lcdSetInt(yes, 0, 1);
    lcdSetText("Random Amp Mode", 0, 0);
    delay_ms(10);
    //lcdClear();
    for(i = 0; i <=yes; i+=samples){
            display();
            delay_ms(1);
        }
        for(i = 1000; i >= 0;i-=samples){
            display();
            delay_ms(1);

        }
    }
}

void PWMsetup(){
    TIMER_A0->CCR[0] = 1000 - 1; // PWM Period
    TIMER_A1->CCR[0] = 1000 - 1; // PWM Period
    TIMER_A0->CCTL[1] |= 0x00E0; // setting reset/set mode
    TIMER_A0->CCTL[2] |= 0x00E0;
    TIMER_A0->CCTL[3] |= 0x00E0;
    TIMER_A0->CCTL[4] |= 0x00E0;
    TIMER_A1->CCTL[1] |= 0x00E0; // setting reset/set mode
    TIMER_A0->CTL |= 0x0214; //setting SMLCK, TACLR, and MC_Upmode
    TIMER_A1->CTL |= 0x0214; //setting SMLCK, TACLR, and MC_Upmode
}
int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
    // Configure GPIO
    P1->DIR = ~(uint8_t) BIT1;
    P2->DIR |= (BIT1 | BIT2);
    P2->OUT &= ~(BIT1 | BIT2);
    P1->REN |= (BIT1 | BIT4 | BIT5);

    P2->DIR |= 0xF0; // P2.4~5 set TA0.1~2
    P2->SEL0 |= 0xF0;

    P7->DIR |= BIT7; // P2.4~5 set TA0.1~2
    P7->SEL0 |= BIT7;

    PWMsetup();
    lcdInit();
    lcdClear();

    P1->IES |= (BIT1 | BIT4 | BIT5); // Interrupt on high-to-low transition
    P1->IFG = 0; // Clear all P1 interrupt flags
    P1->IE |= (BIT1 | BIT4 | BIT5); // Enable interrupt for P1.1
    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    // Enable global interrupt
    __enable_irq();
    //SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);
    //SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // Sleep on exit from ISR
    //__DSB(); // Ensure SLEEPONEXIT takes effect immediately
    //__sleep();
    while(1){
        default_mode();
        sine();
        random();
        random_amp();

}
}
void PORT1_IRQHandler(void)
{
    volatile uint32_t i;
    // Toggling the output on the LED
    if(P1->IFG & BIT1){
        sine_mode = 1;
        random_mode = 0;
        random_amp_mode = 0;
    }
    if(P1->IFG & BIT4){
        sine_mode = 0;
        random_mode = 1;
        random_amp_mode = 0;
    }
    if(P1->IFG & BIT5){
        sine_mode = 0;
        random_mode = 0;
        random_amp_mode = 1;
    }

    // Delay for switch debounce
    for(i = 0; i < 10000; i++)
        P1->IFG = 0; // Clear interrupt flag
}

