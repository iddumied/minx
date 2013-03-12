;
; fibonacci row.
; 

JMP __start

__fib:

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
	
	; fib(n-1)
	DEC 0x21
	CALL __fib

	; fib(n-2)
	DEC 0x21
	CALL __fib

__return_n:

	; stash backjump
	POP 0x22
	; push the current value to stack
	PUSH 0x21

	; push backjump
	PUSH 0x22

	RET

__start:

; call fib for 0x05 == 5
MOVI 0x21 0x05

; copy this value
MOV	0x50 0x21

; call
CALL __fib

; and now sum the result values from the stack

MOVI 0x51 0x00
__sumres:
	
	POP 0x52
	ADD 0x51 0x52

	DEC 0x50
	JMPNZR 0x50 __sumres

; SUM is in Register 0x51 now.
