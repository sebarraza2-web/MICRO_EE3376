
	.thumb
    .text
    .align 2

P4IN .field 0x40004C21,32 ; Port 4 Input
P4OUT .field 0x40004C23,32 ; Port 4 Output
P4DIR .field 0x40004C25,32 ; Port 4 Directions
P4REN .field 0x40004C27,32 ; Port 4 Resistor Enable

P5OUT .field 0x40004C42,32 ; Port 5 Output
P5DIR .field 0x40004C44,32 ; Port 5 Direction

DIET .equ 0x01 ; MACROS for outputs
COKE .equ 0x02
PEPSI .equ 0x04
SPRITE .equ 0x10
JARRITOS .equ 0x20
POWERADE .equ 0x40
ERROR .equ 0x80

swDIET .equ 0x7E ; MACROS for inputs
swCOKE .equ 0x7D
swPEPSI .equ 0x7B
swSPRITE .equ 0x77
swJARRITOS .equ 0x6F
swPOWERADE .equ 0x5F
swMD .equ 0x3F
NOinput .equ 0x7F

    .global main

main:
    BL Port4_Init ; initialize P4 & P5
    BL Port5_Init

mainloop
    BL Port4_Input ; read states from switches in P4

    CMP R0, #swDIET ; 0x3E = DIET Msk
    BEQ swDIETpressed

    CMP R0, #swCOKE ; 0x3D = COKE Msk
    BEQ swCOKEpressed

    CMP R0, #swPEPSI ; 0x3B = PEPSI Msk
    BEQ swPEPSIpressed

    CMP R0, #swSPRITE ; 0x37 = SPRITE Msk
    BEQ swSPRITEpressed

    CMP R0, #swJARRITOS ; 0x2F = JARRITOS Msk
    BEQ swJARRITOSpressed

    CMP R0, #swPOWERADE ; 0x1F = POWERADE Msk
    BEQ swPOWERADEpressed

    CMP R0, #swMD ; 0x1F = POWERADE Msk
    BEQ swMDpressed

    CMP R0, #NOinput ; 0x3F -> NO SWITCHES PRESSED
    BNE error	  ; else error signal
    MOV R0, #0

    BL Port5_Output
    B mainloop

Rand18: ; seed is stored in R6
	MOV R1, #0x660D ; Random function from Valvano PPt
	MUL R6, R6, R1
	MOV R1, #0xF3F5
	ADD R6, R1

	MOV R1, #18 ; Range from 1-18 selected

	UDIV R2, R6, R1 ; Modulo function from Valavano PPT
	MUL R3, R2, R1
	SUB R3, R6, R3

	ADD R6, R3, #1 ; Random value between 1-18 stays in R6
	BX LR

MD_loop: ; loop to process 1 through 18
	MOV R7, #1
	BL output_display
mdloop
	BL LED_toggle ; buzzer beeps as the count increments
	CMP R7, R3
	BEQ check ; once the count reaches random value in generated from Rand18
	BL delay
	ADD R7, R7, #1
	BL output_display
	MOV R0, #0
	BL error
	B mdloop
	BX LR

check          ; R3 is the random max value and will be sent
	CMP R3, #1 ; to the specific switch and lock it out to
	BEQ onlyDIET ; be the only allowed to be pressed
	
	CMP R3, #7
	BEQ onlyDIET
	
	CMP R3, #13
	BEQ onlyDIET
	
	CMP R3, #2
	BEQ onlyCOKE
	
	CMP R3, #8
	BEQ onlyCOKE
	
	CMP R3, #14
	BEQ onlyCOKE
	
	CMP R3, #3
	BEQ onlyPEPSI
	
	CMP R3, #9
	BEQ onlyPEPSI
	
	CMP R3, #15
	BEQ onlyPEPSI
	
	CMP R3, #4
	BEQ onlySPRITE
	
	CMP R3, #10
	BEQ onlySPRITE
	
	CMP R3, #16
	BEQ onlySPRITE
	
	CMP R3, #5
	BEQ onlyJARRITOS
	
	CMP R3, #11
	BEQ onlyJARRITOS
	
	CMP R3, #17
	BEQ onlyJARRITOS
	
	CMP R3, #6
	BEQ onlyPOWERADE
	
	CMP R3, #12
	BEQ onlyPOWERADE
	
	CMP R3, #18
	BEQ onlyPOWERADE

onlyDIET
    BL Port4_Input ; if the mystery drink is DIET then it

    CMP R0, #swDIET ; will only allow for DIET button to be pressed
    BEQ swDIETpressed ; once it is pressed go back to mainloop

	MOV R0, #DIET
    BL Port5_Output

    MOV R0, #0

    BL Port5_Output
    B onlyDIET

onlyCOKE
    BL Port4_Input ; if the mystery drink is COKE then it

    CMP R0, #swCOKE ; will only allow for COKE button to be pressed
    BEQ swCOKEpressed ; once it is pressed go back to mainloop

	MOV R0, #COKE
    BL Port5_Output

    MOV R0, #0

    BL Port5_Output
    B onlyCOKE
onlyPEPSI
    BL Port4_Input ; if the mystery drink is PEPSI then it

    CMP R0, #swPEPSI ; will only allow for PEPSI button to be pressed
    BEQ swPEPSIpressed ; once it is pressed go back to mainloop

	MOV R0, #PEPSI
    BL Port5_Output

    MOV R0, #0

    BL Port5_Output
    B onlyPEPSI
onlySPRITE
    BL Port4_Input ; if the mystery drink is SPRITE then it

    CMP R0, #swSPRITE; will only allow for SPRITE button to be pressed
    BEQ swSPRITEpressed ; once it is pressed go back to mainloop

	MOV R0, #SPRITE
    BL Port5_Output

    MOV R0, #0

    BL Port5_Output
    B onlySPRITE
onlyJARRITOS
    BL Port4_Input; if the mystery drink is JARRITOS then it

    CMP R0, #swJARRITOS ; will only allow for JARRITOS button to be pressed
    BEQ swJARRITOSpressed ; once it is pressed go back to mainloop

	MOV R0, #JARRITOS
    BL Port5_Output

    MOV R0, #0

    BL Port5_Output
    B onlyJARRITOS
onlyPOWERADE
    BL Port4_Input ; if the mystery drink is POWERADE then it

    CMP R0, #swPOWERADE ; will only allow for POWERADE button to be pressed
    BEQ swPOWERADEpressed ; once it is pressed go back to mainloop

	MOV R0, #POWERADE
    BL Port5_Output

    MOV R0, #0

    BL Port5_Output
    B onlyPOWERADE

swDIETpressed
    MOV R0, #DIET ; output-> DIET
    BL Port5_Output
    B mainloop
swCOKEpressed
    MOV R0, #COKE ; output-> COKE
    BL Port5_Output
    B mainloop
swPEPSIpressed
    MOV R0, #PEPSI ; output-> PEPSI
    BL Port5_Output
    B mainloop
swSPRITEpressed
    MOV R0, #SPRITE ; output-> SPRITE
    BL Port5_Output
    B mainloop
swJARRITOSpressed
    MOV R0, #JARRITOS ; output-> JARRITOS
    BL Port5_Output
    B mainloop
swPOWERADEpressed
    MOV R0, #POWERADE ; output-> POWERADE
    BL Port5_Output
    B mainloop
swMDpressed
	ADD R6, #0xDD ; output-> POWERADE
	BL Rand18
	BL MD_loop
	B mainloop
error
    MOV R0, #ERROR ; error will output
    BL Port5_Output
    B mainloop


output_display: ; the output will be on what part of the count it is in
	MOV R6, #0xFD ; change seed to increase random

	cmp R7, #1
	BEQ md_show_diet

    MOV R6, #0x37; change seed to increase random

	cmp R7, #2
	BEQ md_show_coke

	MOV R6, #0x78; change seed to increase random

	cmp R7, #3
	BEQ md_show_pepsi

	MOV R6, #0x98; change seed to increase random

	cmp R7, #4
	BEQ md_show_sprite

	MOV R6, #0x11; change seed to increase random

	cmp R7, #5
	BEQ md_show_jarritos

	MOV R6, #0x36; change seed to increase random

	cmp R7, #6
	BEQ md_show_powerade

	MOV R6, #0xAB; change seed to increase random

	cmp R7, #7
	BEQ md_show_diet
	
	MOV R6, #0xAB; change seed to increase random

	cmp R7, #8
	BEQ md_show_coke

	MOV R6, #0x34; change seed to increase random

	cmp R7, #9
	BEQ md_show_pepsi

	MOV R6, #0xDD; change seed to increase random

	cmp R7, #10
	BEQ md_show_sprite

	MOV R6, #0xFF; change seed to increase random

	cmp R7, #11
	BEQ md_show_jarritos

	MOV R6, #0xBF; change seed to increase random

	cmp R7, #12
	BEQ md_show_powerade

	MOV R6, #0x01; change seed to increase random

	cmp R7, #13
	BEQ md_show_diet

	MOV R6, #0x23; change seed to increase random

	cmp R7, #14
	BEQ md_show_coke

	MOV R6, #0x09; change seed to increase random

	cmp R7, #15
	BEQ md_show_pepsi

	MOV R6, #0x82; change seed to increase random

	cmp R7, #16
	BEQ md_show_sprite

	MOV R6, #0x69; change seed to increase random

	cmp R7, #17
	BEQ md_show_jarritos

	MOV R6, #0x10; change seed to increase random

	cmp R7, #18
	BEQ md_show_powerade
	BX LR
md_show_diet
	MOV R0, #DIET ; output-> DIET
    BL delay	
    BL Port5_Output
    BL delay
    B mdloop
md_show_coke
	MOV R0, #COKE ; output-> COKE 
	BL delay
    BL Port5_Output
    BL delay
    B mdloop
md_show_pepsi
	MOV R0, #PEPSI; output-> PEPSI
    BL delay
    BL Port5_Output
    BL delay
    B mdloop
md_show_sprite
	MOV R0, #SPRITE; output-> SPRITE
    BL delay
    BL Port5_Output
    BL delay
    B mdloop
md_show_jarritos
	MOV R0, #JARRITOS ; output-> JARRITOS
    BL delay
    BL Port5_Output
    BL delay
    B mdloop
md_show_powerade
	MOV R0, #POWERADE ; output-> POWERADE
    BL delay
    BL Port5_Output
    BL delay
    B mdloop
Port4_Init:
    LDR R1, P4REN ; enable internal resitors
    MOV R0, #0x7F
    STRB R0, [R1]

    LDR R1, P4OUT ; configure active low-logic
    MOV R0, #0x7F
    STRB R0, [R1]
    BX LR

Port4_Input:
    LDR R1, P4IN ; reading the input from switches
    LDRB R0, [R1]
    AND R0, R0, #0x7F
    BX LR
    
delay: ; software delay so the count can be seen by the human eye
	MOV R5, #0
	MOV R8, #0xFFFF
delayloop
	CMP R5, R8
	BHS leave
	ADD R5, #1
	B delayloop
leave
	BX LR

Port5_Init:
    LDR R1, P5DIR ; declare outputs
    MOV R0, #0xF7
    STRB R0, [R1]
    BX LR

LED_toggle:
     LDR R1, P5OUT ; setting outputs
     EOR R0, #ERROR
     STRB R0, [R1]
     BX LR

Port5_Output:
     LDR R1, P5OUT ; setting outputs
     STRB R0, [R1]
     BX LR
     .end

