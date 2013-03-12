;
; fibonacci row.
; 

JMP __start

__fib:

	; pop backjump
	POP 0x22 

	; pop parameter
	POP 0x21

	; n in 0x21
	
	; if n < 2
	;  true -> 2 in akku
	;  false -> 0 or 1 in akku

	CMPI 0x21 0x02
	DEC 0x02
	DEC 0x02

	; akku should be 0 if n < 2
	; else != 0

	JMPIZ 0x02 __return_n
	
	; parameter for next call in 0x24, for next call in 0x25
	MOV 0x24 0x21
	MOV 0x25 0x21
	; fib(n-1)
	DEC 0x24
	; for fib(n-2)
	DEC 0x25
	DEC 0x25

	; push as new parameter for fib(n-2)
	PUSH 0x25
	; push as new parameter for fib(n-1)
	PUSH 0x24

	CALL __fib

	; swap the pushed values
	POP 0x24
	POP 0x25 
	PUSH 0x24 
	PUSH 0x25

	CALL __fib

	; add result: 0x02 = fib(n-1)+fib(n-2)
	POP 0x24
	POP 0x25
	ADD 0x24 0x25

	; push it back to stack
	PUSH 0x02
	PUSH 0x22
	RET

__return_n:

	;push return
	PUSH 0x21

	;push backjump
	PUSH 0x22

	RET

__start:

; call fib for 0x03 == 3
MOVI 0x21 0x03

; push parameter for fib(n) call
PUSH 0x21

; call
CALL __fib

POP 0x02

; result is in akku now

EXITI 0x00
