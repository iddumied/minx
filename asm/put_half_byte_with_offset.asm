; size, how many bytes are allocated is in 0xBB
MOVI 0xBB 0x0F
ALLOC 0xBB
; value to put is in 0xFF
MOVI 0xFF 0xFF
; size, how many bytes are put is in 0xCC
MOVI 0xCC 0x04
; some offset 
MOVI 0xBC 0x02
PUT	0x02 0xBC 0xCC 0xFF
FREE 0x02
JMP	0xFFFFFFFFFFFFFFFF
