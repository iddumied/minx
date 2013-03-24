# minx 

This is the minx virtual machine. The bytecode it understands/interprets is 
documented in ByteCode.txt! Have a look at the tags, there are tags for 
protocol-stable commits.

This first implementation comes with the simple reader, which reads the complete
binary into ram. Later on there will be another reader, which will check the
binary for size and just read the actual required parts into ram (and so on).

## Important notes

Important notes for compiling, submitting patches or issues go here.

### Compiling minx

The source is written to compile on Linux. I use a Archlinux 64 Bit machine, 
gcc 4.7.2 to compile. If you want to compile on another platform and it does not
work, I cannot help you! But patches are really welcome!

### Submitting patches

I would love you to submit patches. If you do so, please do a `git rebase`, so I
don't have to click through all commits. Just squash the unimportant ones!

What I do _NOT_ merge: 

- uncommented code (just if it is self-explanatory)
- stupid opcodes (say, INC2 for incrementing by 2 or similar stuff)
- unreadable code (variable names should be explanatory)

### Submitting issues

Please list:

- OS
- architecture (32/64 Bit)
- compiler
- linker (if required) 

and all other stuff I may need. Backtrace naturally.

If you ran a binary on the machine and it crashed, please submit:

- the related `.asm` file 
- the backtrace of the program (running minx with all required arguments)

When I'm not able to reproduce a bug, I would love you to send pull requests!

## Arguments

Arguments, the VPU takes:
first Argument _has to be_ the binary you want to run, then:

	-v				verbosity (does nothing yet, requires VERBOSITY)
	-d				debugging the source it is running
	--sbs			(only with -d) debugging step by step
						use STRG+D for next step 
						use <number> ENTER for <number> steps

	--minx-debug	debugging the VPU internals (not convenient for normal
					users)
	--regs			printing registers at end of program (much output!)
	--notzeroregs	(just with --regs) do not print registers which have value 0.
	--disasm		disassemble program and exit 
	--hex			just with --regs or --disasm -> values in hex, not in decimal
	--fast			don't care about memuse, if you can be faster! This causes
					the VPU to preallocate memory in any way.
	--help | -h		print help message and exit

Note: You _can_ use -d with --minx-debug, BUT the output is really disturbed at
the moment. Maybe I'll fix this.

Plugins (modules)
-----------------

Plugins are called modules, because a plugins is a kernel-module. A plugin has
to implement the module-header located in src/module/module.h, NOT the header
from src/kernel/modules.h !

A module can implement serveral opcodes. But be careful! Your module has to
implement opcodes as stable, don't change them. Existing programs will crash if
the opcode-identifiers change!

A module needs the headers from ./module_headers/!

Modules/Plugins are under development at the moment. I have no working modules
yet to test it.

## Compile flags

* Compile with DEBUG to be able to debug the VPU 
* Compile with DEBUGGING to be able to debug both the VPU and the source its running.
* Compile with VERBOSITY to be able to print registers at end of program
* Compile with DISASSEMBLE to be able to disassemble the binary

If you compile with the Makefile shipped with the code, all these macros are
defined. You have to edit the Makefile if you want to undef one of the opcodes.

## Requirements

Version 2.3.4 of utilc is required (see submodule).

## Branches 

There is a branch called 'pre-stable', including all features tested by me via
.asm files. It does not ensure, the features it includes work completely
fail-save or even that they are tested. Currently, there is no testing except
the .asm files running!

There is a bugfix-commit for a jump-opcode tagged 'v0.0.3-jump-bugfix'. You
really should apply this commit if you use v0.0.3 !

## Additional notes

If you define the macro DESTROY_MY_APPLICATION when compiling, you use Sony-like 
random number generation. (For more information have a look at the RAND opcode).

If you want to talk about minx, send me an email. I'm on IRC (freenode), too,
but minx has no own channel yet. Just no need for it. 

And yeah, I know that my english sucks. 
