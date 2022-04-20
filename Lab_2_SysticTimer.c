#include "msp.h"

#define RED 0x01;
#define GREEN 0x02;
#define BLUE 0x04;

int seconds = 0;
int x = 0;
int start_state = 0;
int display_seconds = 0;
int counter = 0;
int counter_2;
int stop_state;

void set(){
    if((P1->IN & BIT4) == 0){
        x++;
        P2->OUT |= GREEN;
        __delay_cycles(3000000);
    }
    else{
        P2->OUT &= ~GREEN;
    }
}
void stop_counting(){
    if(seconds == x){
        start_state = 0;
        P2->OUT |= BLUE;
    }
    else{
        P2-> OUT &= ~BLUE
    }
}
void display(){
    P4->OUT |= seconds;
}
void start(){
    if((P1->IN & BIT1)==0){
                 stop_state = 0;
                 start_state = 1;
             }
}
void stop(){
    if((P4->IN & BIT4) == 0){
        stop_state = 1;
    }
}
void clear(){
    if((P4->IN & BIT5) == 0){
        P1->OUT &= ~BIT0;
        P2->OUT &= ~0x07;
        start_state = 0;
        stop_state = 1;
        x = 0;
        counter = 0;
        counter_2 = 0;
        seconds = 0;

    }
}

    int main(void) {
 WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
 // Configure GPIO
 P1->DIR |= BIT0;
 P1->OUT &= ~BIT0;

 P1->REN |= (BIT1 | BIT4);
 P1->OUT |= (BIT1 | BIT4);

 P2->DIR |= 0x07;
 P2->OUT &= ~0x07;

 P2->DIR |= BIT7;
 P2->OUT &= ~BIT7;

 P2->DIR |= BIT3;
 P2->OUT &= ~BIT3;

 P4->DIR |= 0x0F;
 P4->OUT &= ~0x0F;

 P4->REN |= (BIT4 | BIT5);
 P4->OUT |= (BIT4 | BIT5);

 // Enable & configure the SysTick Timer Module
 SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
 SysTick->LOAD =300000 -1; // Period = 0x60000
 SysTick->VAL = 0x01; // Clear value by writing dummy value
 SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // Enable interrupt
 // Enable global interrupt
 __enable_irq();
 SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // Sleep on exit from ISR
 __DSB(); // Ensure SLEEPONEXIT takes effect immediately
     while (1){
     __sleep();
    }
}/// End of main
// Interrupt service routine (ISR) invoked when SysTick down counter reaches 0.

void SysTick_Handler(void){

    start();
    set();
    stop_counting();
    stop();
    clear();
    if(stop_state == 0){
        if(start_state == 1){
            counter++;
            if (counter == 10){
                counter_2++;
                counter = 0;
                if((counter_2 % 2) == 0){
                    P1->OUT |= BIT0;
                    seconds++;
                }
                else{
                    P1->OUT &= ~BIT0;
                }
            }
        }
    }







}

