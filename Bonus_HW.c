#include "msp.h"

int i;

void beat_song(){
    while(i < 15){
    __delay_cycles(300000);
    P5->OUT ^= BIT7;
    i++;
    }
}
void beat_song_forever(){
    while(i < 40){
    __delay_cycles(300000);
    P5->OUT ^= BIT7;
    i++;
    }
}
void stop_beat(){
    P5->OUT &= ~BIT7;
    __delay_cycles(300000);
}

int main(void) {
    int j;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // Stop WDT
    // Configure GPIO
    P5->DIR |= BIT7;


    while(1){
        while(j <= 4){
            beat_song();
            i = 0;
            stop_beat();
            j++;
        }
        j = 0;
        i = 0;
        beat_song_forever();
    }
}
