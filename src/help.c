#include "help.h"

void minx_print_help(void) {
    printf(
            "minx [program-binary | help] <flags...>\n"
            "\n"
            "flags are:\n"
            "   -v              verbosity (does nothing yet)\n"
            "   -d              debug program\n"
            "   --minx-debug    debug minx internals\n"
            "   --regs          print registers at end of program"
            "   --notzeroregs   (just with --regs) don't print registers with value zero\n"
            "   --disasm        diassembly the program\n"
            "   --hex           print values in hex\n"
            "   --fast          try to be as fast as possible, don't care about memuse\n"
            "   --help | -h     print this help message and exit\n"
            "\n"
            );
}
