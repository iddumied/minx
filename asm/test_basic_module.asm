;
; The basic module, which I try to load here, is located in
;
; /home/matthias/dev/minx-d-testlib/bin/minx_module_basic.so 
; => length: 58 bytes
;
; so it is not testable for others, if they don't have this path. This asm file 
; must be adapted to test the loading of this plugin.
;

; alloc 58 bytes

ALLOCI 0x3A

; This is not a opcode. It is a "macro"
LDSTRING 0xB0 0xB1 0xB2 0x02 /home/matthias/dev/minx-d-testlib/bin/minx_module_basic.so

;
; move memory id
;
MOV 0xC0 0x02

; load module
MLOAD 0xC0

; save moduleID
MOV 0xC1 0x02

; write opcodes for module in registers
MOVI 0xE0 0x00
MOVI 0xE1 0x01
MOVI 0xE2 0x02

; call opcodes
MCALLNP 0xC1 0xE0
MCALLNP 0xC1 0xE1
MCALLNP 0xC1 0xE2

MHASPRMS 0xC1 0xE0
MHASPRMS 0xC1 0xE1
MHASPRMS 0xC1 0xE2

MGETSTAT 0xC1
; unload module
MUNLOAD 0xC1

EXITI 0x00

