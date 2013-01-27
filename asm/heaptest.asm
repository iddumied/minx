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
MOV 0xFF 0x02

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

;
; If the program arrives here, the memory with the memoryID zero has size 
; 0x1F which is 31 Bytes
;
; So lets put something there!
; 
; Do NOT override register 0xFF, it contains the memoryID!
;

; no offset please!
MOVI 0xBB 0x00

; but all 8 bytes 
MOVI 0xBC 0x08

; 8 Byte value (max) in 0xBB
MOVI 0xBD 0xFFFFFFFFFFFFFFFF

; write 8 byte to the beginning of the heap
PUT 0xFF 0xBB 0xBC 0xBD

; 
; some more?
;

; now at offset 8 write the next 8 bytes 
ADDR 0xBB 0xBC 
PUT 0xFF 0xBB 0xBC 0xBD

; and some more at offset 16+4
ADDR 0xBB 0xBC
; 4 bytes left 
ADDIR 0xBB 0x04 

; PUT!
PUT 0xFF 0xBB 0xBC 0xBD

; and now free the whole shit again!
FREE 0xFF

; EOP
JMP 0xFFFFFFFFFFFFFFFF
