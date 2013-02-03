#ifndef __MINX_VPU_OPCODES_H__
#define __MINX_VPU_OPCODES_H__

#include <inttypes.h>

#ifdef DEBUGGING
#include "util/debug.h"
#endif //DEBUGGING

#include "def/sizes.h"
#include "def/statusbits.h"
#include "util/macros.h"
#include "util/config.h"
#include "kernel/registers.h"
#include "kernel/heap.h"
#include "reader/binary_reader.h"

#ifdef DEBUGGING
#define STACK_PRINTABLE
#endif
#include "stack/stack.h"
#undef STACK_PRINTABLE

extern Stack* stack;

extern void         minx_kernel_program_pointer_manipulate  (uint64_t);
extern uint64_t     minx_kernel_program_pointer_get         (void);
extern void         minx_kernel_unset_running_variable      (void);
extern void         minx_kernel_set_exit_status             (int);

#define         MINX_OPC_NOP			0x00
#define         MINX_OPC_CALL           0x01
#define         MINX_OPC_RET			0x02

#define         MINX_OPC_MOV		    0x03
#define         MINX_OPC_MOVI		    0x04

#define         MINX_OPC_NOT			0x05
#define         MINX_OPC_NOTR		    0x06

#define         MINX_OPC_AND			0x07
#define         MINX_OPC_ANDI		    0x08
#define         MINX_OPC_ANDR		    0x09
#define         MINX_OPC_ANDIR		    0x0A

#define         MINX_OPC_OR			    0x0B
#define         MINX_OPC_ORI			0x0C
#define         MINX_OPC_ORR			0x0D
#define         MINX_OPC_ORIR		    0x0E

#define         MINX_OPC_DEC			0x0F
#define         MINX_OPC_INC			0x10

#define         MINX_OPC_LSH			0x11
#define         MINX_OPC_RSH			0x12

#define         MINX_OPC_CMP            0x13
#define         MINX_OPC_CMPI           0x14
#define         MINX_OPC_EQL            0x15
#define         MINX_OPC_EQLI           0x16

#define         MINX_OPC_PUSH		    0x20
#define         MINX_OPC_POP			0x21
#define         MINX_OPC_DROP		    0x22

#define         MINX_OPC_ADD			0x30
#define         MINX_OPC_ADDI		    0x31
#define         MINX_OPC_ADDR		    0x32
#define         MINX_OPC_ADDIR		    0x33

#define         MINX_OPC_JMP			0x40
#define         MINX_OPC_JMPIZ		    0x41
#define         MINX_OPC_JMPNZ		    0x42
#define         MINX_OPC_IFZJMP		    0x43
#define         MINX_OPC_EXIT           0x44

#define         MINX_OPC_PSTACK         0x50
#define         MINX_OPC_PREGS          0x51
#define         MINX_OPC_PPROG          0x52
#define         MINX_OPC_PMEMS          0x53
#define         MINX_OPC_PMEM           0x54

#define         MINX_OPC_ALLOC          0x60
#define         MINX_OPC_ALLOCI         0x61
#define         MINX_OPC_RESIZE         0x62
#define         MINX_OPC_RESIZEI        0x63
#define         MINX_OPC_FREE           0x64
#define         MINX_OPC_PUT            0x65
#define         MINX_OPC_READ           0x66
#define         MINX_OPC_GETSIZE        0x67

/*
 * -----------------------------------------------------------------------------
 *
 *      Opcodes functions
 *
 * -----------------------------------------------------------------------------
 *
 * (Almost) each opcode has to modify the program pointer to the byte after 
 * the opcode.
 *
 * Explanation:
 *  mov 0x00FF, 0x00FE
 * takes 
 *    16 Bit 
 *  + 64 Bit
 *  + 64 Bit
 *  --------
 *   144 Bit = 18 Byte
 *
 * so the program pointer has to be set to 
 *  program_pointer = program_pointer + 18 Byte
 *
 * If the opcode does affect the program pointer, this has not to be done!
 *
 * Their prototypes:
 *
 */

void        minx_opc_nop_func            (uint64_t* params);
void        minx_opc_call_func           (uint64_t* params);
void        minx_opc_ret_func            (uint64_t* params);

void        minx_opc_mov_func            (uint64_t* params);
void        minx_opc_movi_func           (uint64_t* params);

void        minx_opc_not_func            (uint64_t* params);
void        minx_opc_notr_func           (uint64_t* params);

void        minx_opc_and_func            (uint64_t* params);
void        minx_opc_andi_func           (uint64_t* params);
void        minx_opc_andr_func           (uint64_t* params);
void        minx_opc_andir_func          (uint64_t* params);

void        minx_opc_or_func             (uint64_t* params);
void        minx_opc_ori_func            (uint64_t* params);
void        minx_opc_orr_func            (uint64_t* params);
void        minx_opc_orir_func           (uint64_t* params);

void        minx_opc_dec_func            (uint64_t* params);
void        minx_opc_inc_func            (uint64_t* params);

void        minx_opc_lsh_func            (uint64_t* params);
void        minx_opc_rsh_func            (uint64_t* params);

void        minx_opc_cmp_func            (uint64_t* params);
void        minx_opc_cmpi_func           (uint64_t* params);
void        minx_opc_eql_func            (uint64_t* params);
void        minx_opc_eqli_func           (uint64_t* params);

void        minx_opc_push_func           (uint64_t* params);
void        minx_opc_pop_func            (uint64_t* params);
void        minx_opc_drop_func           (uint64_t* params);

void        minx_opc_add_func            (uint64_t* params);
void        minx_opc_addi_func           (uint64_t* params);
void        minx_opc_addr_func           (uint64_t* params);
void        minx_opc_addir_func          (uint64_t* params);

void        minx_opc_jmp_func            (uint64_t* params);
void        minx_opc_jmpiz_func          (uint64_t* params);
void        minx_opc_jmpnz_func          (uint64_t* params);
void        minx_opc_ifzjmp_func         (uint64_t* params);
void        minx_opc_exit_func           (uint64_t* params);

void        minx_opc_pstack_func         (uint64_t* params);
void        minx_opc_pregs_func          (uint64_t* params);
void        minx_opc_pprog_func          (uint64_t* params);
void        minx_opc_pmems_func          (uint64_t* params);
void        minx_opc_pmem_func           (uint64_t* params);

void        minx_opc_alloc_func          (uint64_t* params);
void        minx_opc_alloci_func         (uint64_t* params);
void        minx_opc_resize_func         (uint64_t* params);
void        minx_opc_resizei_func        (uint64_t* params);
void        minx_opc_free_func           (uint64_t* params);
void        minx_opc_put_func            (uint64_t* params);
void        minx_opc_read_func           (uint64_t* params);
void        minx_opc_getsize_func        (uint64_t* params);

#endif // __MINX_VPU_OPCODES_H__
