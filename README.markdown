This is the first implementation of the minx virtual machine.
For more information, have a look at the ByteCode.def file.

This first implementation comes with the simple reader, which reads the complete
binary into ram. Later on there will be another reader, which will check the
binary for size and just read the actual required parts into ram (and so on).

Version 2.3.3 of utilc is required (see submodule).

Compile with DEBUG to be able to debug the VM, compile with DEBUGGING to be able 
to debug both the VM and the source its running.

Arguments, the VM takes:
first Argument _has to be_ the binary you want to run,
then:
	-v for verbosity (does nothing yet, requires VERBOSITY)
	-d for debugging the source it is running
	--vm-debug for debugging the VM internals
	--regs for printing registers at end of program (this is much output!, requires VERBOSITY)
