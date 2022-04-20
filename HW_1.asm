	.thumb

    .text
    .align 2

P4IN .field 0x40004C21,32 ; Port 1 Input
P4OUT .field 0x40004C23,32 ; Port 1 Output
P4DIR .field 0x40004C25,32 ; Port 1 Directions
P4REN .field 0x40004C27,32 ; Port 1 Resistor Enable

P5OUT .field 0x40004C42,32 ; Port 2 Output
P5DIR .field 0x40004C44,32 ; Port 2 Direction

COKE .equ 0x01 ; on the right side of the LaunchPad board
DIET .equ 0x02 ; on the right side of the LaunchPad board
SPRITE .equ 0x04 ; on the right side of the LaunchPad board
FANTA .equ 0x10 ; on the right side of the LaunchPad board
RB .equ 0x20 ; on the right side of the LaunchPad board
DP .equ 0x40 ; on the right side of the LaunchPad board

ERROR .equ 0x80 ; on the right side of the LaunchPad board

    .global main

main:
    BL Port4_Init ; initialize P1.1 and P1.4 and make them inputs
    BL Port5_Init ; initialize P2.2-P2.0 and make them outputs

loop
    BL Port4_Input ; read both of the switches on Port 1

    CMP R0, #62
    BEQ met_condition

    CMP R0, #61
    BEQ met_condition

    CMP R0, #59
    BEQ met_condition

    CMP R0, #55
    BEQ met_condition

    CMP R0, #47
    BEQ met_condition

    CMP R0, #31
    BEQ met_condition

    CMP R0, #0x3F
    BNE error
    MOV R0, #0

    BL Port5_Output ; turn all of the LEDs on
    B loop


met_condition
	BL Port4_Input ; read both of the switches on Port 1

	CMP R0, #0x3E ; R0 == 0x10?
    BEQ sw0pressed ; if so, switch 1 pressed

    CMP R0, #0x3D ; R0 == 0x02?
    BEQ sw1pressed ; if so, switch 2 pressed

    CMP R0, #0x3B ; R0 == 0x02?
    BEQ sw2pressed ; if so, switch 2 pressed

    CMP R0, #0x37 ; R0 == 0x02?
    BEQ sw3pressed ; if so, switch 2 pressed

    CMP R0, #0x2F ; R0 == 0x02?
    BEQ sw4pressed ; if so, switch 2 pressed

    CMP R0, #0x1F ; R0 == 0x02?
    BEQ sw5pressed ; if so, switch 2 pressed

    CMP R0, #0x1F ; R0 == 0x02?
    BEQ sw5pressed ; if so, switch 2 pressed

    BL Port5_Output ; turn all of the LEDs on
    B loop

sw0pressed
    MOV R0, #COKE ; R0 = BLUE (blue LED on)
    BL Port5_Output ; turn the blue LED on
    B loop
sw1pressed
    MOV R0, #DIET ; R0 = BLUE (blue LED on)
    BL Port5_Output ; turn the blue LED on
    B loop
sw2pressed
    MOV R0, #SPRITE ; R0 = BLUE (blue LED on)
    BL Port5_Output ; turn the blue LED on
    B loop
sw3pressed
    MOV R0, #FANTA ; R0 = BLUE (blue LED on)
    BL Port5_Output ; turn the blue LED on
    B loop
sw4pressed
    MOV R0, #RB ; R0 = BLUE (blue LED on)
    BL Port5_Output ; turn the blue LED on
    B loop
sw5pressed
    MOV R0, #DP ; R0 = BLUE (blue LED on)
    BL Port5_Output ; turn the blue LED on
    B loop

error
    MOV R0, #ERROR ; R0 = 0 (no LEDs on)
    BL Port5_Output ; turn all of the LEDs off
    B loop

Port4_Init:
    LDR R1, P4REN
    MOV R0, #0x3F ; enable pull resistors on P1.4 and P1.1
    STRB R0, [R1]

    LDR R1, P4OUT
    MOV R0, #0x3F ; P1.4 and P1.1 are pull-up
    STRB R0, [R1]
    BX LR

Port4_Input:
    LDR R1, P4IN
    LDRB R0, [R1] ; read all 8 bits of Port 1
    AND R0, R0, #0x3F ; select the input pins P1.1 and P1.4
    BX LR

Port5_Init:
    LDR R1, P5DIR
    MOV R0, #0xF7 ; make P2.2-P2.0 out
    STRB R0, [R1]
    BX LR

Port5_Output:
     LDR R1, P5OUT
     STRB R0, [R1] ; write to P2.7-P2.0
     BX LR
     .end
