#include "msp.h"
#include "stdlib.h"

#define DIET 0x01;
#define COKE 0x02;
#define PEPSI 0x04;
#define SPRITE 0x10;
#define JARRITOS 0x20;
#define POWERADE 0x40;
#define ERROR 0x80;

#define swDIET 0x7E; // MACROS for inputs
#define swCOKE 0x7D;
#define swPEPSI 0x7B;
#define swSPRITE 0x77;
#define swJARRITOS 0x6F;
#define swPOWERADE 0x5F;
#define swMD 0x3F;
#define NOinput 0x7F;

uint8_t music_state = 0;;

uint32_t Port4_Input(){
    return (P4->IN & 0xFF);
}
void SysTick_Wait(float seconds){
    SysTick->CTRL = 5;
    SysTick->LOAD = (seconds*300000) -1;
    SysTick->VAL = 0x01;
    while((SysTick->CTRL & 0x00010000) == 0){
    }
}
int Rand18(){
    int random;
    random = (rand() % 18) + 1;
    return random;
}
void LED_toggle(){
    P5->OUT ^= ERROR;
}
void output_display(int i){
    switch(i){
    case 1: P5->OUT |= DIET; break;
    case 2: P5->OUT |= COKE; break;
    case 3: P5->OUT |= PEPSI; break;
    case 4: P5->OUT |= SPRITE; break;
    case 5: P5->OUT |= JARRITOS; break;
    case 6: P5->OUT |= POWERADE; break;

    case 7: P5->OUT |= DIET; break;
    case 8: P5->OUT |= COKE; break;
    case 9: P5->OUT |= PEPSI; break;
    case 10: P5->OUT |= SPRITE; break;
    case 11: P5->OUT |= JARRITOS; break;
    case 12: P5->OUT |= POWERADE; break;

    case 13: P5->OUT |= DIET; break;
    case 14: P5->OUT |= COKE; break;
    case 15: P5->OUT |= PEPSI; break;
    case 16: P5->OUT |= SPRITE; break;
    case 17: P5->OUT |= JARRITOS; break;
    case 18: P5->OUT |= POWERADE; break;
    default: P5->OUT = 0;
    }
}
void only_diet(){
    while(1){
        P5->OUT |= DIET;
        int mode = Port4_Input();
        switch(mode){
        case 0xFE:P5->OUT |= DIET;return;
        case 0xFF: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_coke(){
    while(1){
        P5->OUT |= COKE;
        int mode = Port4_Input();
        switch(mode){
        case 0xFD:P5->OUT |= COKE;return;
        case 0xFF: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_pepsi(){
    while(1){
        P5->OUT |= PEPSI;
        int mode = Port4_Input();
        switch(mode){
        case 0xFB:P5->OUT |= PEPSI; return;
        case 0xFF: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_sprite(){
    while(1){
        P5->OUT |= SPRITE;
        int mode = Port4_Input();
        switch(mode){
        case 0xF7:P5->OUT |= SPRITE; return;
        case 0xFF: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_jarritos(){
    while(1){
        P5->OUT |= JARRITOS;
        int mode = Port4_Input();
        switch(mode){
        case 0xEF:P5->OUT |= JARRITOS;return;
        case 0xFF: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_powerade(){
    while(1){
        P5->OUT |= POWERADE;
        int mode = Port4_Input();
        switch(mode){
        case 0xDF:P5->OUT |= POWERADE;return;
        case 0xFF: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void check(int random){
    switch(random){
    case 1: only_diet(); break;
    case 2: only_coke(); break;
    case 3: only_pepsi(); break;
    case 4: only_sprite(); break;
    case 5: only_jarritos(); break;
    case 6: only_powerade(); break;

    case 7: only_diet(); break;
    case 8: only_coke(); break;
    case 9: only_pepsi(); break;
    case 10: only_sprite(); break;
    case 11: only_jarritos(); break;
    case 12: only_powerade();; break;

    case 13: only_diet(); break;
    case 14: only_coke(); break;
    case 15: only_pepsi(); break;
    case 16: only_sprite(); break;
    case 17: only_jarritos(); break;
    case 18: only_powerade(); break;
    default: return;
    }
}
void beat_song(){
    int i = 0;
    while(i < 15){
    SysTick_Wait(1);
    LED_toggle();
    i++;
    }
}
void beat_song_forever(){
    int i = 0;
    while(i < 40){
    SysTick_Wait(1);
    LED_toggle();
    i++;
    }
    music_state = 0;
}
void stop_beat(){
    LED_toggle();
    SysTick_Wait(1);
}
void song(){
    int j = 0;
    int i;
    while(music_state == 1){
        while(j <= 3){
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
void enable_music(){
    int mode;
    mode = Port4_Input();
    switch(mode){
    case 0xFE: P5->OUT |= DIET; song(); return;
    case 0xFD: P5->OUT |= COKE; song();  return;
    case 0xFB: P5->OUT |= PEPSI; song();  return;
    case 0xF7: P5->OUT |= SPRITE; song();  return;
    case 0xEF: P5->OUT |= JARRITOS; song();  return;
    case 0xDF: P5->OUT |= POWERADE; song();  return;
    }
    P5->OUT = 0;
}
void MD_loop(){
    int i, j;
    int random = Rand18();
    for(i = 1; i <= random; i++){
        SysTick_Wait(1);
        LED_toggle();
        output_display(i);
        SysTick_Wait(1);
        j = 0;
        output_display(j);
    }
    check(random);
}
void Port4_Init(){
    P4->REN |= 0xFF;
    P4->OUT |= 0xFF;
}
void Port5_Init(){
    P5->DIR |= 0xF7;
    P5->OUT &= ~0xF7;
}

int main(void) {
    Port4_Init();
    Port5_Init();
    uint8_t mode;

    while(1){
        if(music_state){
            enable_music();
        }
        mode = Port4_Input();
        switch(mode){
        case 0xFE: P5->OUT |= DIET; P5->OUT = 0; break;
        case 0xFD: P5->OUT |= COKE;P5->OUT = 0; break;
        case 0xFB: P5->OUT |= PEPSI;P5->OUT = 0; break;
        case 0xF7: P5->OUT |= SPRITE;P5->OUT = 0; break;
        case 0xEF: P5->OUT |= JARRITOS;P5->OUT = 0; break;
        case 0xDF: P5->OUT |= POWERADE;P5->OUT = 0; break;
        case 0xBF: MD_loop(); break;
        case 0x7F: music_state = 1; break;
        case 0xFF: P5->OUT = 0; break;
        default:P5->OUT |= ERROR; break;
        }

    }
}
