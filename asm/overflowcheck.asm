;
; This file contains simple checks for overflow
; ---------------------------------------------
;
; This test checks for _all_ statusbits. Only the first one indicates an 
; overflow.
; Statusbits are reset after the test, to ensure they are 0x00 when testing the 
; next operation.
;
; Operations result are in 0x02 (akku), so no need to care about the initialized 
; values
;

;
; initialize values 
; -----------------
;

; init statusregister 
MOVI 0x03 0x00

; exit code in 0xB0
MOVI 0xB0 0x00

; one
MOVI 0xB1 0x01

; two
MOVI 0xB2 0x02

; UINT64_MAX
MOVI 0xB3 0xFFFFFFFFFFFFFFFF
MOV 0xB4 0xB3
MOV 0xB5 0xB3

; zero
MOVI 0xB6 0x00


; 
; testing
; -------
;


; increment test
INC 0xB5 
; if overflow? then exit-code++
JMPIZ 0x03 __after_inc_test_exit_increment
INC 0xB0
; reset statusbits
MOVI 0x03 0x00
__after_inc_test_exit_increment:


; decrement test

DEC 0xB6
; if overflow then exit-code++
JMPIZ 0x03 __after_dec_test_exit_increment
INC 0xB0
; reset statusbits
MOVI 0x03 0x00
__after_dec_test_exit_increment:


; addition test
ADD 0xB3 0xB3
; if overflow then exit-code++
JMPIZ 0x03 __after_addition_test_exit_increment
INC 0xB0
; reset statusbits
MOVI 0x03 0x00
__after_addition_test_exit_increment:


; subtraction test
SUB 0xB2 0xB3
; if overflow the exit-code++
JMPIZ 0x03 __after_subtraction_test_exit_increment
INC 0xB0
; reset statusbits
MOVI 0x03 0x00
__after_subtraction_test_exit_increment:


; multiplication testing
MUL 0xB2 0xB3
; if overflow then exit-code++
JMPIZ 0x03 __after_multiplication_test_exit_increment
INC 0xB0
; reset statusbits
MOVI 0x03 0x00
__after_multiplication_test_exit_increment:


; expotentation test
POW 0xB2 0xB3
; if overflow then exit-code++
JMPIZ 0x03 __after_exponentation_test_exit_increment
INC 0xB0
; reset statusbits 
MOVI 0x03 0x00 
__after_exponentation_test_exit_increment:

; exit with statuscode
EXIT 0xB0
