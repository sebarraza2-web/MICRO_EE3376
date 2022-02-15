#include "msp.h"
int a1, b1, a2, b2, s1, s2, c2; // global variables being used

/**
 * Manual Mode Module
 * //P2.6 - A1 - Input
    //P2.5 - B1 - Input
    //P2.4 - A2 - Input
    //P2.3 - B2 - Input

    //P4.0 - S1 - Output
    //P4.1 - S2 - Output
    //P4.2 - C_0 - Output

 */

void output_memory(){ //Output display
    if (s1 == 1){ //will only turn ON LED if variable is 1
        P4->OUT |= BIT0;
          }
    else if (s2 == 1){
        P4->OUT |= BIT1;
    }
    else if (c2 == 1){
            P4->OUT |= BIT2;
        }
    return;
}
void adder_logic(){ // routine to do computation and store in global variables
        s1 = a1 ^ b1;
        s2 = (a1 & b1) ^ (a2 ^ b2);
        c2 = ((a2 & b2) | ((
                a1 & b1) & (a2 ^ b2)));
}
void input_memory(){ // Input display
    if (a1 == 1){ //will only turn ON LED if variable is 1
              P3->OUT |= BIT2;
          }
    else if (b1 == 1){
        P3->OUT |= BIT3;
    }
    else if (a2 == 1){
            P3->OUT |= BIT0;
        }
    else if (b2 == 1){
            P3->OUT |= BIT5;
        }
    return;
}
void delete_memory(){ // Routine to clear variables and displays
     P3->OUT &= ~0x2D;
     P4 -> OUT &= ~07;
     a1 = 0;
     b1 = 0;
     a2 = 0;
     b2 = 0;
     s1 = 0;
     s2 = 0;
     c2 = 0;
}
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P3->DIR |= 0x2D;  // STORED LED VALUE INDICATOR
	P3 -> OUT &= ~0x2D;

	P4->DIR |= 0x07; // SUM OUTPUT
	P4 -> OUT &= ~0x07;

	P1->REN |= BIT1; // Enabling Resistor
	P1->OUT |= BIT1; // Pullup Resistor

	while(1){
	  if(P2->IN & BIT6){ // will switch corresponding global variable if either..
	      a1 = 1;        // ..switch is pressed
	  }
	  else if(P2->IN & BIT5){
	      b1 = 1;
	  }
	  else if(P2->IN & BIT4){
	      a2 = 1;
	  }
	  else if(P2->IN & BIT3){
	      b2 = 1;
	  }
	  else{
	      a1 = 0; b1 = 0; a2 = 0; b2 = 0;
	  }

	  input_memory(); // will keep running as long as device has power
	  adder_logic(); // will do math and update global variables in case any inputs are changed
	  output_memory(); // will output display

	  if ((P1->IN & BIT1) == 0){ // clears any memory displays and resets to do more calculations
	      delete_memory();
	  }
	}


}
