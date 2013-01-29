#ifndef __MINX_MAIN_H__
#define __MINX_MAIN_H__

#include "util/error.h"
#include "util/config.h"
#include "reader/binary_reader.h"
#include "kernel/kernel.h"

#ifdef DISASSEMBLE
#include "kernel/disasm.h"
#endif // DISASSEMBLE

#include <stdlib.h>
#include <stdio.h>
#include <mcheck.h>
#include <signal.h>

void handle_signal(int sig);

#endif // __MINX_MAIN_H__
