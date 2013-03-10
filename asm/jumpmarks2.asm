;
; Jump test file with more jumps
;

MOVI 0xB0 0x0E

JMP __test1

__test1:
DEC 0xB0
JMP __test2

__test2:
DEC 0xB0
JMP __test3

__test3:
DEC 0xB0
JMP __test4

__test4:
DEC 0xB0
JMPNZ 0xB0 __test4

__exit:
EXITI 0x00
