;
;
; This is a huge test asm file for testing the heap stuff!
;
;

; The size, how much to alloc 
; 2 + 2 + 8 Bytes
MOVI 0xBB 0x0F

; allocate it 
; 2 + 2 Byte
ALLOC 0xBB

; save the memoryID
; 2 + 2 + 2 Bytes 
MOV 0xFF 0xBB

; resize-maximum in 0xBC
; 2 + 2 + 8 Bytes
MOVI 0xBC 0x1F

; 
; now do a resize-loop until the size is 0x1F
; 

; 2 + 2 Bytes 
INC 0xBB

; 2 + 2 + 2 Bytes 
RESIZE 0xFF 0xBB

; 2 + 2 + 2 Bytes 
CMP 0xBC 0xBB

; jump back to the INC instruction, which is at byte 34 = 0x22
; 2 + 2 + 8 Bytes 
JMPNZ 0x02 0x22
JMP 0xFFFFFFFFFFFFFFFF
