#define __SFR_OFFSET 0
#include <avr/io.h>

#define temp r16
    
    .section .text.ASM
    .global UART_Init
UART_Init:
    push temp ;
	ldi	temp, 0x00	;
	sts	UBRR0H, temp ;
	ldi	temp, 0x67	;
	out	UBRR0L, temp	;  Set baudrate = 9600

	ldi	temp, (1 << TXEN0) | (1<<RXEN0)	;
	out	UCSR0B, temp	; TX & RX enable without interrupt

	ldi	temp, (1 << UCSZ01) | (1 << UCSZ00)	; Data8 Bit, 1 Stop  Bit, No parity
	sts	UCSR0C, temp	;

    pop temp ;
	ret ;

;******************************************************
;*  UART Transmit
;*  function : send a character to RS232C port
;*  input register : r24
;******************************************************
.global PutCh
PutCh:
	sbis    	UCSR0A, UDRE0 ;
	rjmp 		PutCh	;
	; put data into buffer, sends the data
	out	UDR0,   r24 ;
	ret		

;******************************************************
;*  UART Receive
;* function : receive a character from RS232C port
;*  output register : (r25):r24
;******************************************************
.global GetCh
GetCh:
	sbis	    UCSR0A, RXC0	;
	rjmp 	GetCh		;
	; Get and return received data from buffer 	;
	in	r24, UDR0	;
	ldi	r25, 0		;
	ret		

;****************************************************************************
;*  Print String in Data Memory to USART
;*  Input : Starting address should be in (R25:24)
;****************************************************************************
.global PutSt
PutSt:
    push    YH          ;
    push    YL          ;

    mov     YH, R25     ;
    mov     YL, R24     ;
PSD_loop:
    ld      r24, Y+     ;Get 1 byte data from program memory
    tst     r24         ;Check if the data is End of String
    breq    exit2       ;if so, go to exit
    rcall   PutCh       ;Print data in r0 to RS 232C
    rjmp    PSD_loop    ;Get additional data

exit2:
    pop     YL          ;
    pop     YH          ;
    ret
