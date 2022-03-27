#include "msp.h"
#include "lcdLib_432.h"

void transmit(char *str);
char enter[] = "1-RED 2- BLUE\r\n";
char enter_new[] = "1-RED 2- BLUE\r\n";
char letters[1];
uint8_t RXByteCounter = 0;

void uart_init(void){
     //Change pins to use EUSCI peripheral
     P1->SEL0 |= BIT2 | BIT3;
     //Hold EUSCI in reset while we configure it
     EUSCI_A0->CTLW0 |= EUSCI_A_CTLW0_SWRST;
     //Set to use SMCLK, 8 bits, 1 stop bit, no parity and LSB first
     EUSCI_A0->CTLW0  = 0x00C1;
     //Baud = 9600, 3x10^6/9600 = 312.5
     EUSCI_A0->BRW = 312;
     //UCOS16 = 0
     EUSCI_A0->MCTLW &= ~0xFFF1;
     //Start EUSCI after configuration is done
     EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);
     //Set TX interrupt flag to send data
     //Enable TX and RX interrupts
     EUSCI_A0->IE |= EUSCI_A_IE_RXIE;
    //Enable global interrupts and UART interrupt in NVIC
     __enable_irq();
     NVIC->ISER[0] = 1 << ((EUSCIA0_IRQn) & 31);
}
void main(void){
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
    P2->DIR |= (BIT0 | BIT2);
    lcdInit();
    lcdClear();
    uart_init();
    while(1){
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
         __sleep();
        RXByteCounter = 0;
        P2->OUT = 0;
        switch(letters[0]){
        case '1': P2->OUT |= BIT0; break;
        case '2': P2->OUT |= BIT2; break;
        default: transmit(enter); break;
        }

    }
}
void EUSCIA0_IRQHandler(void){
    uint8_t RXData;
    //transmit(enter_new);
    if(EUSCI_A0->IFG & EUSCI_A_IFG_RXIFG){
        RXData = EUSCI_A0->RXBUF;
        if((RXData != 0x0D)&&(RXByteCounter <=1)){
            letters[RXByteCounter++] = RXData;
            lcdSetText(letters, 0, 0);
        }
        else SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    }
}
void transmit(char *str){
    lcdClear();
    letters[1] = 0;
    while(*str != 0){
            if(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG){
                EUSCI_A0->TXBUF = *str++;
            }
    }
}
