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

LDSTRING 0xB0 0xB1 0xB2 0x02 /home/matthias/dev/minx-d-testlib/bin/minx_module_basic.so

;
; move memory id
;
MOV 0xC0 0x02

; load module
MLOAD 0xC0

; unload module
MUNLOAD 0x02

EXITI 0x00

