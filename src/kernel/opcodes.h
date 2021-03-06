#ifndef __MINX_VPU_OPCODES_H__
#define __MINX_VPU_OPCODES_H__

#include <inttypes.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef DEBUGGING
#include "util/debug.h"
#endif //DEBUGGING

#include "def/sizes.h"
#include "def/statusbits.h"
#include "util/macros.h"
#include "util/config.h"
#include "util/overflowcheck.h"
#include "kernel/registers.h"
#include "kernel/heap.h"
#include "kernel/modules.h"
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

/*
 * -----------------------------------------------------------------------------
 *
 *                              ! Protocol-Stable !
 *
 * -----------------------------------------------------------------------------
 *
 *                          DON'T CHANGE THESE OPCODE NUMBERS, 
 *                              they are marked as stable in
 *                              v0.0.3-protocol-stable
 *
 * New opcodes can be defined, but none of the <v0.0.3> marked ones should be
 * changed!
 */

#define         MINX_OPC_NOP			0x00    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_CALL           0x01    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_RET			0x02    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_MOV		    0x03    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_MOVI		    0x04    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_NOT			0x05    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_NOTR		    0x06    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_AND			0x07    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ANDI		    0x08    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ANDR		    0x09    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ANDIR		    0x0A    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_OR			    0x0B    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ORI			0x0C    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ORR			0x0D    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ORIR		    0x0E    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_DEC			0x0F    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_INC			0x10    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_LSH			0x11    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_RSH			0x12    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_CMP            0x13    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_CMPI           0x14    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_EQL            0x15    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_EQLI           0x16    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_SETB           0x17    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_SETBI          0x18    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_CLRB           0x19    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_CLRBI          0x1A    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_ADD			0x20    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ADDI		    0x21    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ADDR		    0x22    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ADDIR		    0x23    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_SUB            0x24    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_SUBI           0x25    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_SUBR           0x26    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_SUBIR          0x27    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_MUL            0x28    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_MULI           0x29    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_MULR           0x2A    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_MULIR          0x2B    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_DIV            0x2C    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_DIVI           0x2D    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_DIVR           0x2E    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_DIVIR          0x2F    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_POW            0x30    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_POWI           0x31    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_POWR           0x32    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_POWIR          0x33    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_SQRT           0x34    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_SQRTR          0x35    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_MOD            0x36    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_MODI           0x37    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_MODR           0x38    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_MODIR          0x39    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_RAND           0x3A    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_URAND          0x3B    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_READSTACK      0x3C    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_PUSH		    0x3D    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_POP			0x3E    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_DROP		    0x3F    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_JMP			0x40    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_JMPIZ		    0x41    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_JMPNZ		    0x42    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_IFZJMP		    0x43    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_EXIT           0x44    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_XOR            0x45    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_XORI           0x46    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_XORR           0x47    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_XORIR          0x48    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_JMPR           0x49    /* <v0.0.4> */ 
#define         MINX_OPC_JMPIZR         0x4A    /* <v0.0.4> */
#define         MINX_OPC_JMPNZR         0x4B    /* <v0.0.4> */
#define         MINX_OPC_IFZJMPRR       0x4C    /* <v0.0.4> */
#define         MINX_OPC_IFZJMPRA       0x4D    /* <v0.0.4> */
#define         MINX_OPC_LDADDR         0x4E    /* <v0.0.4> */
#define         MINX_OPC_EXITI          0x4F    /* <v0.0.4> */

#define         MINX_OPC_PSTACK         0x50    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_PREGS          0x51    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_PPROG          0x52    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_PMEMS          0x53    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_PMEM           0x54    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_ALLOC          0x60    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_ALLOCI         0x61    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_RESIZE         0x62    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_RESIZEI        0x63    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_FREE           0x64    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_PUT            0x65    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_READ           0x66    /* <v0.0.3-protocol-stable> */
#define         MINX_OPC_GETSIZE        0x67    /* <v0.0.3-protocol-stable> */

#define         MINX_OPC_MLOAD          0x70    /* <v0.0.4> */
#define         MINX_OPC_MUNLOAD        0x71    /* <v0.0.4> */
#define         MINX_OPC_MHASPRMS       0x72    /* <v0.0.4> */
#define         MINX_OPC_MCALLNP        0x73    /* <v0.0.4> */
#define         MINX_OPC_MCALL          0x74    /* <v0.0.4> */
#define         MINX_OPC_MGETSTAT       0x75    /* <v0.0.4> */

#define         MINX_OPC_MGETPC         0x76
#define         MINX_OPC_MCALLPP        0x77

#define         MINX_OPC_LBSH           0x80    /* <v0.0.4> */
#define         MINX_OPC_RBSH           0x81    /* <v0.0.4> */
#define         MINX_OPC_EXPORT         0x82    /* <v0.0.4> */
#define         MINX_OPC_IMPORT         0x83    /* <v0.0.4> */


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

void        minx_opc_setb_func           (uint64_t* params);
void        minx_opc_setbi_func          (uint64_t* params);
void        minx_opc_clrb_func           (uint64_t* params);
void        minx_opc_clrbi_func          (uint64_t* params);

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

void        minx_opc_add_func            (uint64_t* params);
void        minx_opc_addi_func           (uint64_t* params);
void        minx_opc_addr_func           (uint64_t* params);
void        minx_opc_addir_func          (uint64_t* params);

void        minx_opc_sub_func           (uint64_t* params);
void        minx_opc_subi_func          (uint64_t* params);
void        minx_opc_subr_func          (uint64_t* params);
void        minx_opc_subir_func         (uint64_t* params);

void        minx_opc_mul_func           (uint64_t* params);
void        minx_opc_muli_func          (uint64_t* params);
void        minx_opc_mulr_func          (uint64_t* params);
void        minx_opc_mulir_func         (uint64_t* params);

void        minx_opc_div_func           (uint64_t* params);
void        minx_opc_divi_func          (uint64_t* params);
void        minx_opc_divr_func          (uint64_t* params);
void        minx_opc_divir_func         (uint64_t* params);

void        minx_opc_pow_func           (uint64_t* params);
void        minx_opc_powi_func          (uint64_t* params);
void        minx_opc_powr_func          (uint64_t* params);
void        minx_opc_powir_func         (uint64_t* params);

void        minx_opc_sqrt_func          (uint64_t* params);
void        minx_opc_sqrtr_func         (uint64_t* params);

void        minx_opc_mod_func           (uint64_t* params);
void        minx_opc_modi_func          (uint64_t* params);
void        minx_opc_modr_func          (uint64_t* params);
void        minx_opc_modir_func         (uint64_t* params);

void        minx_opc_rand_func          (uint64_t* params);
void        minx_opc_urand_func         (uint64_t* params);

void        minx_opc_readstack_func     (uint64_t* params);
void        minx_opc_push_func           (uint64_t* params);
void        minx_opc_pop_func            (uint64_t* params);
void        minx_opc_drop_func           (uint64_t* params);

void        minx_opc_jmp_func            (uint64_t* params);
void        minx_opc_jmpiz_func          (uint64_t* params);
void        minx_opc_jmpnz_func          (uint64_t* params);
void        minx_opc_ifzjmp_func         (uint64_t* params);
void        minx_opc_exit_func           (uint64_t* params);

void        minx_opc_xor_func           (uint64_t* params);
void        minx_opc_xori_func          (uint64_t* params);
void        minx_opc_xorr_func          (uint64_t* params);
void        minx_opc_xorir_func         (uint64_t* params);

void        minx_opc_jmpr_func          (uint64_t* params);
void        minx_opc_jmpizr_func        (uint64_t* params);
void        minx_opc_jmpnzr_func        (uint64_t* params);
void        minx_opc_ifzjmprr_func      (uint64_t* params);
void        minx_opc_ifzjmpra_func      (uint64_t* params);
void        minx_opc_ldaddr_func        (uint64_t* params);
void        minx_opc_exiti_func         (uint64_t* params);

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

void        minx_opc_mload_func         (uint64_t* params);
void        minx_opc_munload_func       (uint64_t* params);
void        minx_opc_mhasprms_func      (uint64_t* params);
void        minx_opc_mcallnp_func       (uint64_t* params);
void        minx_opc_mcall_func         (uint64_t* params);
void        minx_opc_mgetstat_func      (uint64_t* params);

void        minx_opc_mgetpc_func        (uint64_t* params);
void        minx_opc_mcallpp_func       (uint64_t* params);

void        minx_opc_lbsh_func          (uint64_t* params);
void        minx_opc_rbsh_func          (uint64_t* params);
void        minx_opc_export_func        (uint64_t* params);
void        minx_opc_import_func        (uint64_t* params);

#endif // __MINX_VPU_OPCODES_H__

