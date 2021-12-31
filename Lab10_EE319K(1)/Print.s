; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
	PRESERVE8 
    THUMB
	
;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; R0=0,    then output "0"
; R0=3,    then output "3"
; R0=89,   then output "89"
; R0=123,  then output "123"
; R0=9999, then output "9999"
; R0=4294967295, then output "4294967295"
LCD_OutDec
	
	; if n< 10, print that character, return
	; n >= 10, quo equals divide by 10, remainder equals n - quo*ten
	; call lcd_outdec with quo
	; print remainder
	
N EQU 0		;Store N on stack
QuoT EQU 4 ;Store Quotient*10 on stack
CntD EQU 8

	PUSH {R11, LR}

	SUB SP, #12		  ;Reserve 12 spaces on the stack for the local variables
	MOV R11, SP		 ;R11 holds our intial stack pointer
	STR R0, [R11, #N] ;Store N on the stack
	MOV R0, #0
	STR R0, [R11, #QuoT]
	STR R0, [R11, #CntD]	
	
loopDec
	LDR R0, [R11,#N]
	MOV R2, #10
	UDIV R1, R0, R2
	STR R1, [R11,#N]	; N/10 is the new n
	MUL R2, R1
	STR R2, [R11, #QuoT]
	LDR R2, [R11, #QuoT]
	SUB R0, R0, R2  ; n - n/10*10
	PUSH {R0} ; Store value to print later
	LDR R0, [R11, #CntD]
	MOV R1, #1
	ADD R0, R1
	STR R0, [R11, #CntD]	; increment count
	LDR R0, [R11,#N]
	CMP R0, #0	;Check if N==0
	BNE loopDec ;if not 0, continue
DecPrint
	POP {R0}			;Restore digit to R0
	ADD R0, R0, #0x30	;Add 0x30 to turn the number into its ASCII code
	BL ST7735_OutChar
	LDR R0, [R11, #CntD]
	SUBS R0, #1
	STR R0, [R11, #CntD]	; Decrement count
	CMP R0, #0
	BNE DecPrint ;If there are more characters to output, continue
	ADD SP, #12
	POP {R11, LR}
    BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000"
;       R0=3,    then output "0.003"
;       R0=89,   then output "0.089"
;       R0=123,  then output "0.123"
;       R0=9999, then output "9.999"
;       R0>9999, then output "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	
	; if n< 10, print that character, return
	; if 9999 > n >= 10, quo equals divide by 10, remainder equals n - quo*ten
	; if n > 9999, output *.***
	; call lcd_outdec with quo
	; print remainder
	
NF EQU 0		;Store N on stack
QTF EQU 4 		;Store QuoTen
Cnt EQU 8 		;Keep count
Ast EQU 12
Per EQU 16
Zer EQU 20

	PUSH {R11, LR}

	SUB SP, #24		  ;Reserve 20 spaces on the stack for the local variables
	MOV R11, SP
	STR R0, [R11, #NF] ;Store N on the stack
	MOV R1, #0		 	;Store 0 as initial count
	STR R1, [R11, #Cnt]
	MOV R1, #0x2A		;Store asterisk ascii on stack
	STR R1, [R11, #Ast]
	MOV R1, #0x2E		;Store period ascii on stack
	STR R1, [R11, #Per]
	MOV R1, #0x30
	STR R1, [R11, #Zer]  ;Store zero ascii on stack


;CheckIfMax
	LDR R0, [R11, #NF]
	MOV R1, #9999
	SUBS R1, R0, R1		;Compare to 9999
	BHI MaxPrint		;If greater than 9999, we cannot output this as a fixed point number
	
loopFix
	LDR R1, [R11, #Cnt]
	ADD R1, #1
	STR R1, [R11, #Cnt]	;Increment count
	CMP R1, #4			;If we have already found three digits, we need to output .
	BEQ OutPeriod
	LDR R1, [R11, #NF]
	MOV R0, R1
	MOV R2, #10
	UDIV R1, R2			;Divide N by ten --- N/10
	STR R1,	[R11, #NF]	;N/10 is the next N
	MUL R1, R2			;N/10*10
	SUB R0, R1			;N/10*10 - N
	ADD R0, R0, #0x30	;Add 0x30 to turn into ASCII character
	PUSH {R0}
	LDR R0, [R11, #Cnt]
	CMP R0, #5			;If we have five digits, we need to output
	BEQ FixPrint
	B loopFix
	
FixPrint
	
	POP {R0}			;Restore digit to R0
	BL ST7735_OutChar		;Output character
	LDR R1, [R11, #Cnt]
	SUB R1, #1			;Decrement counter
	STR R1, [R11, #Cnt]
	CMP R1, #0			;If count is 0 we are done
	BNE FixPrint
	ADD SP, #24
	POP {R11, LR}
    BX   LR

OutPeriod

	LDR R1, [R11, #Per]
	PUSH {R1}
	B loopFix

MaxPrint
 	LDR R0, [R11,#Ast]	;Output *.***
	BL ST7735_OutChar	;Output character
	LDR R0, [R11,#Per]	
	BL ST7735_OutChar	;Output character
	LDR R0, [R11,#Ast]	
	BL ST7735_OutChar	;Output character
	LDR R0, [R11,#Ast]	
	BL ST7735_OutChar	;Output character
	LDR R0, [R11,#Ast]	
	BL ST7735_OutChar	;Output character
	ADD SP, #24
	POP {R11, LR}
	
    BX   LR
 
     ALIGN 
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
