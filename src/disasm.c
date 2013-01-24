#include "opcodes.h"
#include "debug.h"

static struct {
    unsigned int    opcode;
    char            *desc;
    unsigned int    params[];

} opcode_map[] = {
    { OPC_NOP,    "NOP",      { } }, 
    { OPC_CALL,   "CALL",     { } }, 
    { OPC_RET,    "RET",      { } }, 
    { OPC_MOV,    "MOV",      { } }, 
    { OPC_MOVI,   "MOVI",     { } }, 
    { OPC_NOT,    "NOT",      { } }, 
    { OPC_NOTR,   "NOTR",     { } }, 
    { OPC_AND,    "AND",      { } }, 
    { OPC_ANDI,   "ANDI",     { } }, 
    { OPC_ANDR,   "ANDR",     { } }, 
    { OPC_ANDIR,  "ANDIR",    { } }, 
    { OPC_OR,     "OR",       { } }, 
    { OPC_ORI,    "ORI",      { } }, 
    { OPC_ORR,    "ORR",      { } }, 
    { OPC_ORIR,   "ORIR",     { } }, 
    { OPC_DEC,    "DEC",      { } }, 
    { OPC_INC,    "INC",      { } }, 
    { OPC_LSH,    "LSH",      { } }, 
    { OPC_RSH,    "RSH",      { } }, 
    { OPC_CMP,    "CMP",      { } }, 
    { OPC_CMPI,   "CMPI",     { } }, 
    { OPC_EQL,    "EQL",      { } }, 
    { OPC_EQLI,   "EQLI",     { } }, 
    { OPC_PUSH,   "PUSH",     { } }, 
    { OPC_POP,    "POP",      { } }, 
    { OPC_DROP,   "DROP",     { } }, 
    { OPC_ADD,    "ADD",      { } }, 
    { OPC_ADDI,   "ADDI",     { } }, 
    { OPC_ADDR,   "ADDR",     { } }, 
    { OPC_ADDIR,  "ADDIR",    { } }, 
    { OPC_JMP,    "JMP",      { } }, 
    { OPC_JMPIZ,  "JMPIZ",    { } }, 
    { OPC_JMPNZ,  "JMPNZ",    { } }, 
    { OPC_IFZJMP, "IFZJMP",   { } }, 
    { OPC_PSTACK, "PSTACK",   { } }, 
    { OPC_PREGS,  "PREGS",    { } }, 
    { OPC_PPROG,  "PPROG",    { } }, 
    { OPC_PMEMS,  "PMEMS",    { } }, 
    { OPC_PMEM,   "PMEM",     { } }, 
    { OPC_ALLOC,  "ALLOC",    { } }, 
    { OPC_ALLOCI, "ALLOCI",   { } }, 
    { OPC_RESIZE, "RESIZE",   { } }, 
    { OPC_RESIZEI,"RESIZEI",  { } }, 
    { OPC_FREE,   "FREE",     { } }, 
    { OPC_PUT,    "PUT",      { } }, 
    { OPC_READ,   "READ",     { } }, 
    { OPC_GETSIZE,"GETSIZE",  { } }, 
}; 


