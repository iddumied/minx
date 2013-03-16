;
; Fibonacci without using the stack to store values
;

;
; - Argument register for fib() is 0xB0
; - Heapsize in 0xB2 (0xA0 = 160 bytes)
; - Heap offset pointer on 0xB3
; - Heap addr in 0xB4
;
; - 0xC0 for result of the current fib()

; CONST
MOVI 0xAA 0x01

MOVI 0xB2 0xA0 
MOVI 0xB3 0x00
MOVI 0xB4 0x00

JMP __start

;
; fibonacci function
; 
__fib:
	CMPI 0xB0 0x02
	DEC 0x02
	DEC 0x02
	JMPIZ 0x02 __pre_endfib

	CALL __checkheapsize
	
	DEC 0xB0
	CALL __fib 

	DEC 0xB0
	CALL __fib

	DEC 0xB3
	READ 0xB4 0xB3 0xAA 0xB5

	DEC 0xB3
	READ 0xB4 0xB3 0xAA 0xB6

	ADD 0xB5 0xB6
	MOV 0xC0 0x02
	JMP __endfib

__pre_endfib:
	MOV 0xC0 0xB0

__endfib:
	PUT 0xB4 0xB3 0xAA 0xC0
	INC 0xB3
	RET

;
; checkheap function,
; - checks if the heappointer points to the last heap address 
; - if yes, it doubles the heapsize by calling the __resizeheap function
;
__checkheapsize:
	CMP 0xB2 0xB3
	JMPNZ 0x02 __dont_resize
	CALL __resizeheap
__dont_resize:
	RET

;
; resize the heap (double its size)
;
__resizeheap:
	MULIR 0xB1 0x02
	RESIZE 0xB4 0xB1
	RET


__start: 
	ALLOCI 0xB2
	MOV 0xB4 0x02
	MOVI 0xB0 0x05
	CALL __fib

	FREE 0xB4
	EXITI 0x00
