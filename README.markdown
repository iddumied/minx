This is the first implementation of the minx virtual machine.
For more information, have a look at the ByteCode.def file.

This first implementation comes with the simple reader, which reads the complete
binary into ram. Later on there will be another reader, which will check the
binary for size and just read the actual required parts into ram (and so on).

Version 2.3.4 of utilc is required (see submodule).

Compile with DEBUG to be able to debug the VPU 
Compile with DEBUGGING to be able to debug both the VPU and the source its running.
Compile with VERBOSITY to be able to print registers at end of program
Compile with DISASSEMBLE to be able to disassemble the binary

Arguments, the VPU takes:
first Argument _has to be_ the binary you want to run, then:

	-v				verbosity (does nothing yet, requires VERBOSITY)
	-d				debugging the source it is running
	--minx-debug	debugging the VPU internals
	--regs			printing registers at end of program (much output!)
	--disasm		disassemble program and exit 
	--hex			just with --regs or --disasm -> values in hex, not in decimal


Branches 
--------

There is a branch called 'pre-stable', including all features tested by me via
.asm files. It does not ensure, the features it includes work completely
fail-save or even that they are tested. Currently, there is no testing except
the .asm files running!
