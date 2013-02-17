#include "help.h"

void minx_print_help(void) {
    printf(
            "usage: minx [program-binary | help] <flags...>\n"
            "\n"
            "   minx has to be compiled with certain options for some config flags.\n"
            "   They are indicated bracketed.\n"
            "\n"
            "flags are:\n"
            "   -v              verbosity (does nothing yet)                                [VERBOSITY]\n"
            "   -d              debug program                                               [DEBUGGING]\n"
            "   --regs          print registers at end of program                           [VERBOSITY or DEBUGGING]\n"
            "   --notzeroregs   (just with --regs) don't print registers with value zero    [see --regs]\n"
            "   --disasm        diassembly the program                                      [DISASSEMBLE]\n"
            "   --hex           print values in hex\n"
            "   --fast          try to be as fast as possible, don't care about memuse\n"
            "   --minx-debug    debug minx internals (just for debugging while development) [DEBUG or DEBUGGING]\n"
            "   --help | -h     print this help message and exit\n"
            "\n"
            "Licenced under BSD 2-clause licence. (c) 2013 Matthias Beyer <matthias@ib-fb.de>\n"
            );
}
