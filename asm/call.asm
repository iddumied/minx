;
; call tester
;

JMP __start

__foo:
	MOVI 0x21 0x55
	RET

__bar:
	MOVI 0x22 0x55
	RET

__start:

	CALL __foo
	CALL __bar

	SUB 0x21 0x22

	EXIT 0x02
