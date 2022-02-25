#include "msp.h"
#include "lcdLib_432.h"

int PWM[5] = {1500, 3000, 4500, 6000, 7500};
int degree[] = {-90,-45,0,45,90};
char buffer[];
int start_state;
int stop_state;
int i, j;

void stop(){
    lcdSetText("Stopped", 6, 1);
    lcdSetInt(degree[j], 0, 0);
}

void start(){
    while(i < 6){
        if(start_state == 1){
            P2->OUT |= BIT1;
            //j = i;
            TIMER_A0->CCR[1] = PWM[i];
            lcdSetText("Start", 6, 1);
            lcdSetInt(degree[i], 0, 0);
            delay_ms(1000);
            lcdClear();
        }
        i++;
    }
    i = 0;
}
void PWMsetup(){
    TIMER_A0->CCR[0] = 60000 - 1; // PWM Period
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // CCR1 reset/set
    //TIMER_A0->CCR[1] = 1000; // CCR1 PWM duty cycle
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK
            TIMER_A_CTL_MC__UP | // Up mode
            TIMER_A_CTL_CLR; // Clear TAR
}



int main(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
    // Configure GPIO
    P1->DIR = ~(uint8_t) BIT1;
    P2->DIR |= BIT1;
    P2->OUT &= ~BIT1;
    P1->REN |= (BIT1 | BIT4);
    P1->OUT |=(BIT1 | BIT4);

    P2->DIR |= BIT4; // P2.4~5 set TA0.1~2
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~(BIT4);
    PWMsetup();

lcdInit();
lcdClear();

//

    P1->IES |= (BIT1 | BIT4); // Interrupt on high-to-low transition
    P1->IFG = 0; // Clear all P1 interrupt flags
    P1->IE |= (BIT1 | BIT4); // Enable interrupt for P1.1
    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    // Enable PCM rude mode to enter LPM3 without waiting for peripherals
    //PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;

    //PJ->DIR |= (BIT0| BIT1 | BIT2 | BIT3);
     //PJ->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);

//
 // Enable & configure the SysTick Timer Module
 //SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
 //SysTick->LOAD =3000 -1; // Period = 0x60000
 //SysTick->VAL = 0x01; // Clear value by writing dummy value
 //SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // Enable interrupt


 // Enable global interrupt
 __enable_irq();
 //SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk);
 //SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // Sleep on exit from ISR
// __DSB(); // Ensure SLEEPONEXIT takes effect immediately
 //__sleep();

     while(1){
         if((start_state == 1) && (stop_state == 0)){

             start();
             //lcdClear();
         }
         if((start_state == 0) && (stop_state == 1)){
             P2->OUT &= ~BIT1;
             stop();
     }

}
}/// End of main
// Interrupt service routine (ISR) invoked when SysTick down counter reaches 0.

    void PORT1_IRQHandler(void)
    {
        volatile uint32_t i;
        // Toggling the output on the LED
        if(P1->IFG & BIT1){
            //SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
            start_state = 1;
            stop_state = 0;
        }
        if(P1->IFG & BIT4){
            //SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
            start_state = 0;
            stop_state = 1;
        }

        // Delay for switch debounce
        for(i = 0; i < 10000; i++)
            P1->IFG &= ~(BIT1 | BIT4); // Clear interrupt flag
    }

