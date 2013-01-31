#ifndef __MINX_PROTOCOL_H__
#define __MINX_PROTOCOL_H__

#include "def/sizes.h" /* for MAX_PARAMETER_COUNT */ 
#include "kernel/opcodes.h"

typedef struct {
    void (*opc_func)(uint64_t*);
    char *strrep;
    unsigned int params[MAX_PARAMETER_COUNT];
} OpcodeInformation;

const OpcodeInformation opcodes[] = {

    [MINX_OPC_NOP]       = { 
        .opc_func = minx_opc_nop_func,
        .strrep = "NOP",
        .params = { },
    },

    [MINX_OPC_CALL]      = { 
        .opc_func = minx_opc_call_func,
        .strrep = "CALL",
        .params = { PROGRAM_ADDRESS_SIZE },
    },

    [MINX_OPC_RET]       = { 
        .opc_func = minx_opc_ret_func,
        .strrep = "RET",
        .params = { },
    },

    [MINX_OPC_MOV]       = { 
        .opc_func = minx_opc_mov_func,
        .strrep = "MOV",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_MOVI]      = { 
        .opc_func = minx_opc_movi_func,
        .strrep = "MOVI",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_NOT]       = { 
        .opc_func = minx_opc_not_func,
        .strrep = "NOT",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_NOTR]      = { 
        .opc_func = minx_opc_notr_func,
        .strrep = "NOTR",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_AND]       = { 
        .opc_func = minx_opc_and_func,
        .strrep = "AND",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_ANDI]      = { 
        .opc_func = minx_opc_andi_func,
        .strrep = "ANDI",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_ANDR]      = { 
        .opc_func = minx_opc_andr_func,
        .strrep = "ANDR",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_ANDIR]     = { 
        .opc_func = minx_opc_andir_func,
        .strrep = "ANDIR",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_OR]        = { 
        .opc_func = minx_opc_or_func,
        .strrep = "OR",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_ORI]       = { 
        .opc_func = minx_opc_ori_func,
        .strrep = "ORI",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_ORR]       = { 
        .opc_func = minx_opc_orr_func,
        .strrep = "ORR",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_ORIR]      = { 
        .opc_func = minx_opc_orir_func,
        .strrep = "ORIR",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_DEC]       = { 
        .opc_func = minx_opc_dec_func,
        .strrep = "DEC",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_INC]       = { 
        .opc_func = minx_opc_inc_func,
        .strrep = "INC",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_LSH]       = { 
        .opc_func = minx_opc_lsh_func,
        .strrep = "LSH",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_RSH]       = { 
        .opc_func = minx_opc_rsh_func,
        .strrep = "RSH",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_CMP]       = { 
        .opc_func = minx_opc_cmp_func,
        .strrep = "CMP",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_CMPI]      = { 
        .opc_func = minx_opc_cmpi_func, 
        .strrep = "CMPI",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_EQL]       = { 
        .opc_func = minx_opc_eql_func,
        .strrep = "EQL",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_EQLI]      = { 
        .opc_func = minx_opc_eqli_func,
        .strrep = "EQLI",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_PUSH]      = { 
        .opc_func = minx_opc_push_func,
        .strrep = "PUSH",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_POP]       = { 
        .opc_func = minx_opc_pop_func,
        .strrep = "POP",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_DROP]      = { 
        .opc_func = minx_opc_drop_func,
        .strrep = "DROP",
        .params = { },
    },

    [MINX_OPC_ADD]       = { 
        .opc_func = minx_opc_add_func,
        .strrep = "ADD",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_ADDI]      = { 
        .opc_func = minx_opc_addi_func,
        .strrep = "ADDI",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_ADDR]      = { 
        .opc_func = minx_opc_addr_func,
        .strrep = "ADDR",
        .params = { REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_ADDIR]     = { 
        .opc_func = minx_opc_addir_func,
        .strrep = "ADDIR",
        .params = { REGISTER_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_JMP]       = { 
        .opc_func = minx_opc_jmp_func,
        .strrep = "JMP",
        .params = { PROGRAM_ADDRESS_SIZE },
    },

    [MINX_OPC_JMPIZ]     = { 
        .opc_func = minx_opc_jmpiz_func,
        .strrep = "JMPIZ",
        .params = { REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE },
    },

    [MINX_OPC_JMPNZ]     = { 
        .opc_func = minx_opc_jmpnz_func,
        .strrep = "JMPNZ",
        .params = { REGISTER_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE },
    },

    [MINX_OPC_IFZJMP]    = { 
        .opc_func = minx_opc_ifzjmp_func,
        .strrep = "IFZJMP",
        .params = { PROGRAM_ADDRESS_SIZE, PROGRAM_ADDRESS_SIZE },
    },

    [MINX_OPC_EXIT]      = { 
        .opc_func = minx_opc_exit_func,
        .strrep = "EXIT",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_PSTACK]    = { 
        .opc_func = minx_opc_pstack_func,
        .strrep = "PSTACK",
        .params = { },
    },

    [MINX_OPC_PREGS]     = { 
        .opc_func = minx_opc_pregs_func,
        .strrep = "PREGS",
        .params = { },
    },

    [MINX_OPC_PPROG]     = { 
        .opc_func = minx_opc_pprog_func,
        .strrep = "PPROG",
        .params = { },
    },

    [MINX_OPC_PMEMS]     = { 
        .opc_func = minx_opc_pmems_func,
        .strrep = "PMEMS",
        .params = { },
    },

    [MINX_OPC_PMEM]      = { 
        .opc_func = minx_opc_pmem_func,
        .strrep = "PMEM",
        .params = { HEAP_ADDRESS_SIZE  },
    },

    [MINX_OPC_ALLOC]     = { 
        .opc_func = minx_opc_alloc_func,
        .strrep = "ALLOC",
        .params = { REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_ALLOCI]    = { 
        .opc_func = minx_opc_alloci_func,
        .strrep = "ALLOCI",
        .params = { VALUE_SIZE },
    },

    [MINX_OPC_RESIZE]    = { 
        .opc_func = minx_opc_resize_func,
        .strrep = "RESIZE",
        .params = { HEAP_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_RESIZEI]   = { 
        .opc_func = minx_opc_resizei_func,
        .strrep = "RESIZEI",
        .params = { HEAP_ADDRESS_SIZE, VALUE_SIZE },
    },

    [MINX_OPC_FREE]      = { 
        .opc_func = minx_opc_free_func,
        .strrep = "FREE",
        .params = { HEAP_ADDRESS_SIZE },
    },

    [MINX_OPC_PUT]       = { 
        .opc_func = minx_opc_put_func,
        .strrep = "PUT",
        .params = { HEAP_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_READ]      = { 
        .opc_func = minx_opc_read_func,
        .strrep = "READ",
        .params = { HEAP_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE, REGISTER_ADDRESS_SIZE },
    },

    [MINX_OPC_GETSIZE]   = { 
        .opc_func = minx_opc_getsize_func,
        .strrep = "GETSIZE",
        .params = { HEAP_ADDRESS_SIZE },
    },

};

#endif // __MINX_PROTOCOL_H__
