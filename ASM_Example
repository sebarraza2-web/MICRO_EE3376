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

uint32_t Port4_Input(){
    return (P4->IN & 0x7F);
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
        case 0x7E:P5->OUT |= DIET;return;
        case 0x7F: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_coke(){
    while(1){
        P5->OUT |= COKE;
        int mode = Port4_Input();
        switch(mode){
        case 0x7D:P5->OUT |= COKE;return;
        case 0x7F: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_pepsi(){
    while(1){
        P5->OUT |= PEPSI;
        int mode = Port4_Input();
        switch(mode){
        case 0x7B:P5->OUT |= PEPSI; return;
        case 0x7F: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_sprite(){
    while(1){
        P5->OUT |= SPRITE;
        int mode = Port4_Input();
        switch(mode){
        case 0x77:P5->OUT |= SPRITE; return;
        case 0x7F: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_jarritos(){
    while(1){
        P5->OUT |= JARRITOS;
        int mode = Port4_Input();
        switch(mode){
        case 0x6F:P5->OUT |= JARRITOS;return;
        case 0x7F: P5->OUT = 0; break;
        default: P5->OUT |= ERROR; break;
        }
    }
}
void only_powerade(){
    while(1){
        P5->OUT |= JARRITOS;
        int mode = Port4_Input();
        switch(mode){
        case 0x5F:P5->OUT |= POWERADE;return;
        case 0x7F: P5->OUT = 0; break;
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
void MD_loop(){
    int i, j;
    int random = Rand18();
    for(i = 1; i <= random; i++){
        LED_toggle();
        __delay_cycles(3000000);
        output_display(i);
        __delay_cycles(300000);
        j = 0;
        output_display(j);
    }
    check(random);
}
void Port4_Init(){
    P4->REN |= 0x7F;
    P4->OUT |= 0x7F;
}
void Port5_Init(){
    P5->DIR |= 0xF7;
    P5->OUT &= ~0xF7;
}

int main(void) {
    Port4_Init();
    Port5_Init();
    uint32_t mode;

    while(1){
        mode = Port4_Input();

        if(mode == 0x7E){
            P5->OUT |= DIET;
        }
        else if(mode == 0x7D){
            P5->OUT |= COKE;
        }
        else if(mode == 0x7B){
            P5->OUT |= PEPSI;
        }
        else if(mode == 0x77){
            P5->OUT |= SPRITE;
        }
        else if(mode == 0x6F){
            P5->OUT |= JARRITOS;
        }
        else if(mode == 0x5F){
            P5->OUT |= POWERADE;
        }
        else if (mode == 0x3F){
            MD_loop();
        }
        else if (mode == 0x7F){
            P5->OUT = 0;
        }
        else{
            P5->OUT |= ERROR;
        }
        P5->OUT = 0;

    }
}

