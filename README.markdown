This is the first implementation of the minx virtual machine.
For more information, have a look at the ByteCode.def file.

The first implementation of the VM will support 64 Bit addresses for both 
internal registers and length of the binary. Op codes will have 24 Bit 
(4096 possible op codes).

Version 2.3.3 of utilc is required.

Compile with DEBUG to be able to debug the VM, compile with DEBUGGING to be able 
to debug both the VM and the source its running.

Arguments, the VM takes:
first Argument _has to be_ the binary you want to run,
then:
	-v for verbosity (not supported yet)
	-d for debugging the source it is running
	--vm-debug for debugging the VM internals
