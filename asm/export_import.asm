;
; test export and import opcodes
;


MOVI 0xB0 0x01

EXPORT

MOVI 0xB1 0x00

IMPORT

ADDR 0xB0 0xB1 
; 0xB0 should be still 1
DEC 0xB0
; 0xB0 should be 0

; exit with zero
EXIT 0xB0
