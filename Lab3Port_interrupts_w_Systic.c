#include "msp.h"
#include "lcdLib_432.h"

int start_state = 0;
int counter;
int ms, sec;
int min;
int stop_state;
char Time[16];
char name[] = {'S','e','e','b','a','s'};

void update(){
    sprintf(Time, "%0.2d:%0.2d:%0.2d",  min, sec, ms);
    lcdSetText(Time, 0, 0);
}
void show_name(){
    //char yes;
    int i;
    for(i = 0; i <= sizeof(name); i++){
        sprintf(name, "%c", name[i]);
        lcdSetText(name, 10, 0);
        delay_ms(1000);
    }
    lcdClear();
}

void clear(){
    start_state = 0;
    stop_state = 1;
    ms = 0;
    sec = 0;
    min = 0;
    counter = 0;
    }


int main(void) {
 WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
 // Configure GPIO
 P1->DIR = ~(uint8_t) BIT1;
 P1->REN |= (BIT1 | BIT4 | BIT5);
 P1->OUT |=(BIT1 | BIT4 | BIT5);

lcdInit();
lcdClear();

//

    P1->IES = (BIT1 | BIT4 | BIT5); // Interrupt on high-to-low transition
    P1->IFG = 0; // Clear all P1 interrupt flags
    P1->IE = (BIT1 | BIT4 | BIT5); // Enable interrupt for P1.1
    show_name();
    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
    // Configure Port J
    PJ->DIR |= (BIT0| BIT1 | BIT2 | BIT3);
    PJ->OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);
    // Enable PCM rude mode to enter LPM3 without waiting for peripherals
    PCM->CTL1 = PCM_CTL0_KEY_VAL | PCM_CTL1_FORCE_LPM_ENTRY;
//
 // Enable & configure the SysTick Timer Module
 SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
 SysTick->LOAD =30000 -1; // Period = 0x60000
 SysTick->VAL = 0x01; // Clear value by writing dummy value
 SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // Enable interrupt


 // Enable global interrupt
 __enable_irq();
 SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // Sleep on exit from ISR
 __DSB(); // Ensure SLEEPONEXIT takes effect immediately
 __sleep();

     //while(1){
        // show_name();
        //
         //break();
     //}

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
        if(P1->IFG & BIT5){
           clear();

        }

        // Delay for switch debounce
        for(i = 0; i < 10000; i++)
            P1->IFG &= ~(BIT1 | BIT4 | BIT5); // Clear interrupt flag
    }

void SysTick_Handler(void){
    update();
    //show_name();
    if(stop_state == 0){
        if(start_state == 1){
            ms++;
            if (ms == 100){
                sec++;
                //show_name();
                ms = 0;
                if(sec == 60){
                    //show_name();
                    min++;
                    sec=0;
                }

                }
            }
        }
    }




