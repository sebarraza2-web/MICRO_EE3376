#include "msp.h"
#include "lcdLib_432.h"
#include "stdio.h"

int start_state = 0;
int ms, sec;
int min;
int stop_state;
char Time[1];
char boomerang[] = {'^','>','v','<'};
char blank[] = {' ',' ',' ',' '};
char store[1];
char store2[1];
char store_again[1];
int j, c, r;
int x = 5;
int letgo;
int strength;

void update(){
    sprintf(Time, "%d",  strength);
    lcdSetText(Time, 10, 1);

   // lcdSetText("Sebas", 10, 0);
}
void bump(){
    if(x == c){
        x++;
    }
}

void display(){
    if(r == 0){
        bump();
        lcdSetText("Sebas", x, 0);
        sprintf(store, "%c", boomerang[j]);
        lcdSetText(store, c, r);
        delay_ms(500);
        sprintf(store_again, "%c", blank[j]);
        lcdSetText(store_again, c, r);
    }
    if(r == 1){
        bump();
        lcdSetText("Sebas", x, 0);
        sprintf(store, "%c", boomerang[j]);
        lcdSetText(store, c, r);
        delay_ms(500);
        sprintf(store_again, "%c", blank[j]);
        lcdSetText(store_again, c, r);
        c--;
    }
}
void boom(){
    while(letgo == 1){
        display();
        j++;
        if(j == 4){
            j = 0;
        }
        if((c <= (strength-2)) && (r == 0)){
            c++;
        }
        else{
            r = 1;
        }
        if(c == -1){
            c = 0;
            letgo = 0;
            strength = 0;
        }
    }
    //lcdSetText("Sebas", 10, 0);
}
int main(void) {
 WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
 // Configure GPIO
 P1->DIR = ~(uint8_t) BIT1;
 P1->REN |= (BIT1 | BIT4);
 P1->OUT |=(BIT1 | BIT4);

lcdInit();
lcdClear();

//

    P1->IES = (BIT4 | BIT5); // Interrupt on high-to-low transition
    P1->IFG = 0; // Clear all P1 interrupt flags
    P1->IE = (BIT4 | BIT5); // Enable interrupt for P1.1
    //show_name();
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



}/// End of main
// Interrupt service routine (ISR) invoked when SysTick down counter reaches 0.

    void PORT1_IRQHandler(void)
    {
        volatile uint32_t i;
        // Toggling the output on the LED
        if(P1->IFG & BIT4){
            //SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
            letgo = 1;
        }

        // Delay for switch debounce
        for(i = 0; i < 10000; i++)
            P1->IFG &= ~(BIT4); // Clear interrupt flag
    }

void SysTick_Handler(void){
    boom();
    update();
    bump();
    if((P1->IN & BIT1) == 0){
        ms++;
        r = 0;
        c = 0;
        j = 0;
        if(ms == 100){
            strength++;
            ms = 0;
        }
    }
}

