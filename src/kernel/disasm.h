#ifndef __MINX_DISASM_H__
#define __MINX_DISASM_H__

#include <inttypes.h>

#include "util/config.h"
#include "util/debug.h"

#include "def/sizes.h"
#include "def/protocol.h"

#include "kernel/opcodes.h"

#include "reader/binary_reader.h"

void minx_disasm_run(void);

#endif // __MINX_DISASM_H__
