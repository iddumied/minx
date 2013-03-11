;
; test export and import opcodes with more export/import calls
;

MOVI 0xB0 0x01
MOVI 0xB1 0x01
MOVI 0xB2 0x01
MOVI 0xB3 0x01
MOVI 0xB4 0x01
MOVI 0xB5 0x01
MOVI 0xB6 0x01
MOVI 0xB7 0x01
MOVI 0xB8 0x01
MOVI 0xB9 0x01
MOVI 0xBA 0x01
MOVI 0xBB 0x01
MOVI 0xBC 0x01
MOVI 0xBD 0x01
MOVI 0xBE 0x01
MOVI 0xBF 0x01

EXPORT

MOVI 0xB0 0x01
MOVI 0xB1 0x01
MOVI 0xB2 0x01
MOVI 0xB3 0x01
MOVI 0xB4 0x01
MOVI 0xB5 0x01
MOVI 0xB6 0x01
MOVI 0xB7 0x01
MOVI 0xB8 0x01
MOVI 0xB9 0x01
MOVI 0xBA 0x01
MOVI 0xBB 0x01
MOVI 0xBC 0x01
MOVI 0xBD 0x01
MOVI 0xBE 0x01
MOVI 0xBF 0x01

EXPORT

MOVI 0xB0 0x01
MOVI 0xB1 0x01
MOVI 0xB2 0x01
MOVI 0xB3 0x01
MOVI 0xB4 0x01
MOVI 0xB5 0x01
MOVI 0xB6 0x01
MOVI 0xB7 0x01
MOVI 0xB8 0x01
MOVI 0xB9 0x01
MOVI 0xBA 0x01
MOVI 0xBB 0x01
MOVI 0xBC 0x01
MOVI 0xBD 0x01
MOVI 0xBE 0x01
MOVI 0xBF 0x01

EXPORT

MOVI 0xB0 0x01
MOVI 0xB1 0x01
MOVI 0xB2 0x01
MOVI 0xB3 0x01
MOVI 0xB4 0x01
MOVI 0xB5 0x01
MOVI 0xB6 0x01
MOVI 0xB7 0x01
MOVI 0xB8 0x01
MOVI 0xB9 0x01
MOVI 0xBA 0x01
MOVI 0xBB 0x01
MOVI 0xBC 0x01
MOVI 0xBD 0x01
MOVI 0xBE 0x01
MOVI 0xBF 0x01

EXPORT

JMPNZ 0xB0 __exit_err
JMPNZ 0xB1 __exit_err
JMPNZ 0xB2 __exit_err
JMPNZ 0xB3 __exit_err
JMPNZ 0xB4 __exit_err
JMPNZ 0xB5 __exit_err
JMPNZ 0xB6 __exit_err
JMPNZ 0xB7 __exit_err
JMPNZ 0xB8 __exit_err
JMPNZ 0xB9 __exit_err
JMPNZ 0xBA __exit_err
JMPNZ 0xBB __exit_err
JMPNZ 0xBC __exit_err
JMPNZ 0xBD __exit_err
JMPNZ 0xBE __exit_err
JMPNZ 0xBF __exit_err

IMPORT

JMPIZ 0xB0 __exit_err
JMPIZ 0xB1 __exit_err
JMPIZ 0xB2 __exit_err
JMPIZ 0xB3 __exit_err
JMPIZ 0xB4 __exit_err
JMPIZ 0xB5 __exit_err
JMPIZ 0xB6 __exit_err
JMPIZ 0xB7 __exit_err
JMPIZ 0xB8 __exit_err
JMPIZ 0xB9 __exit_err
JMPIZ 0xBA __exit_err
JMPIZ 0xBB __exit_err
JMPIZ 0xBC __exit_err
JMPIZ 0xBD __exit_err
JMPIZ 0xBE __exit_err
JMPIZ 0xBF __exit_err

IMPORT

JMPIZ 0xB0 __exit_err
JMPIZ 0xB1 __exit_err
JMPIZ 0xB2 __exit_err
JMPIZ 0xB3 __exit_err
JMPIZ 0xB4 __exit_err
JMPIZ 0xB5 __exit_err
JMPIZ 0xB6 __exit_err
JMPIZ 0xB7 __exit_err
JMPIZ 0xB8 __exit_err
JMPIZ 0xB9 __exit_err
JMPIZ 0xBA __exit_err
JMPIZ 0xBB __exit_err
JMPIZ 0xBC __exit_err
JMPIZ 0xBD __exit_err
JMPIZ 0xBE __exit_err
JMPIZ 0xBF __exit_err

IMPORT

JMPIZ 0xB0 __exit_err
JMPIZ 0xB1 __exit_err
JMPIZ 0xB2 __exit_err
JMPIZ 0xB3 __exit_err
JMPIZ 0xB4 __exit_err
JMPIZ 0xB5 __exit_err
JMPIZ 0xB6 __exit_err
JMPIZ 0xB7 __exit_err
JMPIZ 0xB8 __exit_err
JMPIZ 0xB9 __exit_err
JMPIZ 0xBA __exit_err
JMPIZ 0xBB __exit_err
JMPIZ 0xBC __exit_err
JMPIZ 0xBD __exit_err
JMPIZ 0xBE __exit_err
JMPIZ 0xBF __exit_err

IMPORT

JMPIZ 0xB0 __exit_err
JMPIZ 0xB1 __exit_err
JMPIZ 0xB2 __exit_err
JMPIZ 0xB3 __exit_err
JMPIZ 0xB4 __exit_err
JMPIZ 0xB5 __exit_err
JMPIZ 0xB6 __exit_err
JMPIZ 0xB7 __exit_err
JMPIZ 0xB8 __exit_err
JMPIZ 0xB9 __exit_err
JMPIZ 0xBA __exit_err
JMPIZ 0xBB __exit_err
JMPIZ 0xBC __exit_err
JMPIZ 0xBD __exit_err
JMPIZ 0xBE __exit_err
JMPIZ 0xBF __exit_err

EXITI 0x00

__exit_err:
EXITI 0x01
